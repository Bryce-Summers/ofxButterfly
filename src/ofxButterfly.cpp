/*
 * ofxButterfly.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: Bryce Summers
 */

#include "ofxButterfly.h"
#include "mesh.hpp"

// Transforms an ofMesh to a gfx:: WindgedEdge.
// ENSURES : The index orderings should not have been changed.
gfx::WingedEdge toWingedEdge(ofMesh mesh, std::map<gfx::Vertex, int> &map_vip, std::map<int, gfx::Vertex> &map_ivp)
{
    
    gfx::WingedEdge WE_Output;
    
    // -- Extract and add all of the vertices.
    ofVec3f* vertices = mesh.getVerticesPointer();
    int len_vertices  = mesh.getNumVertices();
    
    std::vector<gfx::Vertex> dataVertices;
    
    for(int i = 0; i < len_vertices; i++)
    {
        ofVec3f& vert = vertices[i];
        gfx::Vertex dataVert = WE_Output.AddVertex(vert.x, vert.y, vert.z);
        
        dataVertices.push_back(dataVert);
        map_vip.insert(std::pair<gfx::Vertex, int>(dataVert, i));
        map_ivp.insert(std::pair<int, gfx::Vertex>(i, dataVert));
    }
    
    // Construct the tripartite relationship (Add the triangular faces.)
    ofIndexType * indexes = mesh.getIndexPointer();
    int len_indexes       = mesh.getNumIndices();
    
    for(int i = 0; i < len_indexes; i+=3)
    {
        ofIndexType i1, i2, i3;
        
        i1 = indexes[i + 0];
        i2 = indexes[i + 1];
        i3 = indexes[i + 2];
        
        gfx::Vertex v1, v2, v3;
        
        v1 = dataVertices[i1];
        v2 = dataVertices[i2];
        v3 = dataVertices[i3];
        
        gfx::Edge e1, e2, e3;
        
        e1 = WE_Output.AddEdge(v1, v2);
        e2 = WE_Output.AddEdge(v2, v3);
        e3 = WE_Output.AddEdge(v3, v1);
        
        WE_Output.AddFace(e1, e2, e3);
    }
    
    return WE_Output;
}

// Converts from a gfx::WingedEdge class to an ofMesh.
// ENSURES : The indices of the original vertices have not been mutated.
ofMesh fromWingedEdge(gfx::WingedEdge WE, std::map<gfx::Vertex, int> &index_map, std::map<int, gfx::Vertex> &map_iv)
{
    
    ofMesh output;
    
    // Extract useful data from the Winged edge structure.
    std::map<gfx::Vertex, std::set<gfx::Edge> > vertMap = WE.vertexList;
    std::map<gfx::Edge, gfx::EdgeList> edgeListMap 		= WE.edgeListMap;
    std::map<gfx::Face, std::set<gfx::Edge> > faceMap   = WE.faceList;
    
    // We need to compute the vertices, and the list of triangular
    // faces in the subdivision to reconstruct an ofMesh.
    
    int len = map_iv.size();
    
    // Add the original vertices to the new ofMesh.
    for(int i = 0; i < len; i++)
    {
        gfx::Vertex v = map_iv.find(i) -> second;
        ofVec3f ofVector(v.X(), v.Y(), v.Z());
        output.addVertex(ofVector);
    }
    
    // Add the new vertices to the ofMesh and add them to the index_map;
    for(std::map<gfx::Vertex, std::set<gfx::Edge> >::iterator iter = vertMap.begin(); iter != vertMap.end(); ++iter)
    {
        gfx::Vertex v = iter -> first;
        
        // Avoid reproccessing the original vertices that have already been added to the mesh.
        if(index_map.find(v) != index_map.end())
        {
            continue;
        }
        
        ofVec3f ofVector(v.X(), v.Y(), v.Z());
        output.addVertex(ofVector);
            
        // The next index will always be equal to the length.
        index_map[v] = len;
        len++;
    }
    
    // Compute the Triangles.
    for(std::map<gfx::Face, std::set<gfx::Edge> >::iterator iter = faceMap.begin(); iter != faceMap.end(); ++iter)
    {
        gfx::Face f = iter -> first;
        
        // Extract vertice information from the face.
        gfx::Edge e1 = f.E1();
        gfx::Edge e2 = f.E2();
        gfx::Vertex v1, v2, v3;
        
        v1 = e1.V1();
        v2 = e1.V2();
        v3 = e2.V1();
        
        // FIXME : Make sure this is a vector equality comparison, not a pointer comparison.
        if(v3 == v1 || v3 == v2)
        {
            v3 = e2.V2();
        }
        
        // Translate these vertices into ofMesh indices.
        int i1, i2, i3;
        i1 = index_map.find(v1) -> second;
        i2 = index_map.find(v2) -> second;
        i3 = index_map.find(v3) -> second;
        
        // Insert the indices to specify a triangle in the ofMesh.
        output.ofMesh::addIndex(i1);
        output.ofMesh::addIndex(i2);
        output.ofMesh::addIndex(i3);
    }
    
    return output;
}

ofxButterfly::ofxButterfly()
{
	// TODO Auto-generated constructor stub
}

ofxButterfly::~ofxButterfly()
{
	// TODO Auto-generated destructor stub
}

// -- Public interface functions.

ofMesh ofxButterfly::subdivideButterfly(ofMesh mesh, int iterations)
{
    return subdivide(mesh, iterations, BUTTERFLY);
}

ofMesh ofxButterfly::subdivideLinear(ofMesh mesh, int iterations)
{
    return subdivide(mesh, iterations, LINEAR);
}

ofMesh ofxButterfly::subdividePascal(ofMesh mesh, int iterations)
{
    return subdivide(mesh, iterations, PASCAL);
}

ofMesh ofxButterfly::subdivideBoundary(ofMesh mesh, float min_len_edges, int iterations)
{
    return subdivide(mesh, iterations, BOUNDARY, min_len_edges);
}


// -- Private work functions.


/*
 * REQUIRES: ofMesh must be in mode OF_PRIMITIVE_TRIANGLES.
 * 			iterations should be a positive number that specifies how many times the algorithm should be performed.
 * ENSURES : Returns a subdivided ofMesh,
 * 			 all vertices in the original mesh must retain their positions
 * 			 and indices as given in the original mesh.
 *          The original mesh should not be mutated.
 */
ofMesh ofxButterfly::subdivide(ofMesh mesh, int iterations, subdivision_type type, float min_len_edges)
{
    ofMesh output = mesh;
    for(int i = 0; i < iterations; i++)
    {
        output = subdivide(output, type, min_len_edges);
    }
    
    return output;
}


// Performs one iteration of the subdivision.
ofMesh ofxButterfly::subdivide(ofMesh mesh, subdivision_type type, float min_len_edges)
{

	std::map<gfx::Vertex, int> map_vertice_index;
    std::map<int, gfx::Vertex> map_index_vertice;

	// Convert to winged edge.
	gfx::WingedEdge WE_original = toWingedEdge(mesh, map_vertice_index, map_index_vertice);

	// Subdivide. (Do I need to free memory???)

    gfx::WingedEdge WE_Output;
    
    switch(type)
    {
        case BUTTERFLY:
            WE_Output = WE_original.ButterflySubdivide();
            break;
        case LINEAR:
            WE_Output = WE_original.LinearSubdivide();
            break;
        case BOUNDARY:
            WE_Output = WE_original.BoundaryTrianglularSubdivide(min_len_edges);
            break;
        case PASCAL:
            WE_Output = WE_original.SillyPascalSubdivide();
            break;
        default:
            throw new RuntimeError("Malformed type. We do not know how to subdivide the mesh in the given way.");
    }
 	
    // Extract the fresh linear subdivided mesh.
    ofMesh output = fromWingedEdge(WE_Output, map_vertice_index, map_index_vertice);
 
	return output;

}