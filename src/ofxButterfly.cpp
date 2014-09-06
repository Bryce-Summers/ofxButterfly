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
gfx::WingedEdge toWingedEdge(ofMesh mesh, std::map<gfx::Vertex, int> map)
{
    gfx::WingedEdge WE_Output;
    
    // -- Extract and add all of the vertices.
    ofVec3f* vertices = mesh.getVerticesPointer();
    int len_vertices = mesh.getNumVertices();
    
    std::vector<gfx::Vertex> dataVertices;
    
    for(int i = 0; i < len_vertices; i++)
    {
        ofVec3f& vert = vertices[i];
        gfx::Vertex dataVert = WE_Output.AddVertex(vert.x, vert.y, vert.z);
        
        dataVertices.push_back(dataVert);
        map.insert(std::pair<gfx::Vertex, int>(dataVert, i));
    }
    
    // Construct the tripartite relationship (Add the triangular faces.)
    ofIndexType * indexes = mesh.getIndexPointer();
    int len_indexes = mesh.getNumIndices();
    
    for(int i = 0; i < len_indexes; i+=3)
    {
        ofIndexType i1, i2, i3;
        
        i1 = indexes[i1];
        i2 = indexes[i2];
        i3 = indexes[i3];
        
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
ofMesh fromWingedEdge(gfx::WingedEdge WE, std::map<gfx::Vertex, int> index_map)
{
    
    ofMesh output;
    
    // Extract useful data from the Winged edge structure.
    std::map<gfx::Vertex, std::set<gfx::Edge> > vertMap = WE.vertexList;
    std::map<gfx::Edge, gfx::EdgeList> edgeListMap 		= WE.edgeListMap;
    std::map<gfx::Face, std::set<gfx::Edge> > faceMap   = WE.faceList;
    
    // We need to compute the vertices, and the list of triangular
    // faces in the subdivision to reconstruct an ofMesh.
    
    int index = index_map.size();
    
    // Compute the Vertices.
    for(std::map<gfx::Vertex, std::set<gfx::Edge> >::iterator iter = vertMap.begin(); iter != vertMap.end(); ++iter)
    {
        gfx::Vertex v = iter -> first;
        
        ofVec3f ofVector(v.X(), v.Y(), v.Z());
        
        output.addVertex(ofVector);
        
        // Insert v --> index into the map, if it is not already contained therin..
        if(index_map.find(v) == index_map.end())
        {
            index_map[v] = index;
            index++;
        }
    }
    
    // Compute the Triangles.
    // Compute the Vertices.
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

void ofxButterfly::setup()
{
	// So far It does not appear than any setup is necessary...
}

/*
 * REQUIRES: ofMesh must be in mode OF_PRIMITIVE_TRIANGLES.
 * 			iterations should be a positive number that specifies how many times the algorithm should be performed.
 * ENSURES : Returns a subdivided ofMesh,
 * 			 all vertices in the original mesh must retain their positions
 * 			 and indices as given in the original mesh.
 */
ofMesh ofxButterfly::subdivide(ofMesh mesh, int iterations)
{
	std::map<gfx::Vertex, int> verticeMap;

	// Convert to winged edge.
	gfx::WingedEdge WE_Output = toWingedEdge(mesh, verticeMap);

	// Subdivide.
	for(int i = 0; i < iterations; i++)
	{
		// FIXME : Do I need to worry about freeing memory?
		WE_Output = WE_Output.ButterflySubdivide();
	}

	// Convert back.
	return fromWingedEdge(WE_Output, verticeMap);

}

/*
// Use BFS to find all of the faces in the new structure.
std::set<gfx::Face> getFaceList(gfx::WingedEdge WE_initial)
{
	std::set<gfx::Face> output;
	std::set<gfx::WingedEdge> visited;
	std::queue<gfx::WingedEdge> frontier;

	// Push the first winged edge.
	frontier.push(WE_initial);

	while(!frontier.empty())
	{
		gfx::WingedEdge WE = frontier.pop();

		// Do not revisit nodes.
		if(visited.find(WE) != std::set::end)
		{
			continue;
		}

		gfx::Face f1, f2;
		f1 = WE.
	}

}

// Use BFS to find all of the vertices in the new structure.
std::vector<gfx::Vertex> getVertexList(std::vector<gfx::Face>)
{

}
*/
