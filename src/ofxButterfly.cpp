/*
 * ofxButterfly.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: Bryce Summers
 *
 */

#include "ofxButterfly.h"
#include "error.hpp"


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

/*
 *
 * -- Public interface functions.
 *
 */


// -- Single mesh subdivision functions.
ofMesh ofxButterfly::subdivideButterfly(ofMesh &mesh, int iterations)
{
    subdivide_start(mesh);
    subdivideButterfly(iterations);
    return subdivide_end();
}

ofMesh ofxButterfly::subdivideLinear(ofMesh &mesh, int iterations)
{
    subdivide_start(mesh);
    subdivideLinear(iterations);
    return subdivide_end();
}

ofMesh ofxButterfly::subdividePascal(ofMesh &mesh, int iterations)
{
    subdivide_start(mesh);
    subdividePascal(iterations);
    return subdivide_end();
}

ofMesh ofxButterfly::subdivideBoundary(ofMesh &mesh, float pixel_prescision, int iterations)
{
    subdivide_start(mesh);
    subdivideBoundary(pixel_prescision, iterations);
    return subdivide_end();
}


// -- Batch subdivision pipeline.

// Prepares the given mesh for subdivision.
void ofxButterfly::subdivide_start(ofMesh &mesh)
{
    map_vertice_index.clear();
    map_index_vertice.clear();
    current_WE = toWingedEdge(mesh, map_vertice_index, map_index_vertice);
}

inline void ofxButterfly::subdivideButterfly(int iterations)
{
    subdivide(iterations, BUTTERFLY);
}

inline void ofxButterfly::subdivideLinear(int iterations)
{
    subdivide(iterations, LINEAR);
}

inline void ofxButterfly::subdividePascal(int iterations)
{
    subdivide(iterations, PASCAL);
}

inline void ofxButterfly::subdivideBoundary(float pixel_prescision, int iterations)
{
    subdivide(iterations, BOUNDARY, pixel_prescision);
}

inline ofMesh ofxButterfly::subdivide_end()
{
    // Extract the subdivided mesh.
    ofMesh output = fromWingedEdge(current_WE, map_vertice_index, map_index_vertice);
    return output;
}

// Fast repetitive subdivision routines.
void ofxButterfly::topology_start(ofMesh &mesh)
{
    map_vertice_index.clear();
    map_index_vertice.clear();
    
    // -- Initialize the transformation mapping.
    transformation.clear();
    int len = mesh.getNumVertices();
    for(int i = 0; i < len; i++)
    {
        vector<int> val;
        val.push_back(i);
        transformation[i] = val;
    }
    
    current_WE = toWingedEdge(mesh, map_vertice_index, map_index_vertice);
}

void ofxButterfly::topology_subdivide_boundary()
{
    std::map<gfx::Vertex, std::vector<gfx::Vertex> > info;
    
    // -- Subdivide every boundary vertice.
    current_WE = current_WE.BoundaryTrianglularSubdivide(info);
    
    // Update the transformation.
    int next_index = transformation.size();
    for(auto iter = current_WE.vertexList.begin(); iter != current_WE.vertexList.end(); ++iter)
    {
        gfx::Vertex v = iter -> first;
        
        // If the vertice is old, just continue.
        if(map_vertice_index.find(v) != map_vertice_index.end())
        {
            continue;
        }
        
        // The vertice is new.
        map_vertice_index[v] = next_index;
        map_index_vertice[next_index] = v;
        
        // -- Convert the vertex_derivation to an indice derivation.
        // note: that all vertices/indexes in the derivation must be old, becuase of the subdivision algorithm.
        std::vector<gfx::Vertex> vertex_derivation = info.find(v)->second;
        std::vector<int> indice_derivation;
        
        for(auto iter = vertex_derivation.begin(); iter != vertex_derivation.end(); ++iter)
        {
            indice_derivation.push_back(map_vertice_index[*iter]);
        }
        
        transformation[next_index] = indice_derivation;
        
        next_index++;
    }
   
}

ofMesh ofxButterfly::topology_end()
{
    return subdivide_end();
}


void ofxButterfly::fixMesh(ofMesh &mesh, ofMesh &subdivided_mesh)
{
    vector<ofVec3f> original_vertices = mesh.getVertices();
    ofVec3f * sub_vertices = subdivided_mesh.getVerticesPointer();
    
    int original_vert_num = original_vertices.size();
    
    // Move all of the original vertices to the divided mesh.
    for(int i = 0; i < original_vert_num; i++)
    {
        sub_vertices[i] = original_vertices[i];
    }

    deriveVertices(original_vert_num, sub_vertices, subdivided_mesh.getNumVertices());
    
    
    // --  handle texture coordinates.
    // handle texture coordinates.
    ofVec2f * sub_textureCoords = subdivided_mesh.getTexCoordsPointer();
    int original_texture_num = mesh.getNumTexCoords();
    for(int i = 0; i < original_texture_num; i++)
    {
        sub_textureCoords[i] = mesh.getTexCoord(i);
    }
    
    // Derive the rest of the texture coordinates.
    deriveTextureVertices(original_vert_num, sub_textureCoords, subdivided_mesh.getNumVertices());
}

// -- Private work functions.

// Only called from fixMesh.
inline void ofxButterfly::deriveVertices(int first_derived_indice, ofVec3f * vertices, int max_indice)
{
//    int max_indice = vertices.size();
    
    // Temporary vectors.
    ofVec3f a1, a2, b1, b2, c1, c2, c3, c4, d1, d2;
    
    for(int i = first_derived_indice; i < max_indice; i++)
    {
        vector<int> inputs = transformation[i];
        
        int size = inputs.size();
        
        switch(size)
        {
             // Same vertex.
            case 1: vertices[i] = vertices[inputs[0]];
                continue;
                
            // Linear Interpolation.
            case 2: vertices[i] = (vertices[inputs[0]] + vertices[inputs[1]])/2;
                continue;
                
            // Boundary interpolation.
            case 4:
                a1 = vertices[inputs[0]];
                a2 = vertices[inputs[1]];
                b1 = vertices[inputs[2]];
                b2 = vertices[inputs[3]];
                vertices[i] = (9*a1 + 9*a2 - b1 - b2)/16.0;
                continue;
                
            // Internal 6 regular butterfly subdivision.
            case 8:
                a1 = vertices[inputs[0]];
                a2 = vertices[inputs[1]];
                b1 = vertices[inputs[2]];
                b2 = vertices[inputs[3]];

                c1 = vertices[inputs[4]];
                c2 = vertices[inputs[5]];
                c3 = vertices[inputs[6]];
                c4 = vertices[inputs[7]];

                /*
                d1 = vertices[inputs[8]];
                d2 = vertices[inputs[9]];
                */
                 
                // FIXME : I do not know if this is correct.
                vertices[i] = (8*(a1 + a2) + 2*(b1 + b2) -(c1 + c2 + c3 + c4))/16.0;// + d1 + d2;
                continue;
                
            default : throw new RuntimeError("Error in the topology Derivation data structures.");
        }
    }
}

inline void ofxButterfly::deriveTextureVertices(int first_derived_indice, ofVec2f * vertices, int max_indice)
{
    //    int max_indice = vertices.size();
    
    // Temporary vectors.
    ofVec3f a1, a2, b1, b2, c1, c2, c3, c4, d1, d2;
    
    for(int i = first_derived_indice; i < max_indice; i++)
    {
        vector<int> inputs = transformation[i];
        
        int size = inputs.size();
        
        switch(size)
        {
                // Same vertex.
            case 1: vertices[i] = vertices[inputs[0]];
                continue;
                
                // Linear Interpolation.
            case 2: vertices[i] = (vertices[inputs[0]] + vertices[inputs[1]])/2;
                continue;
                
                // Boundary interpolation.
            case 4:
                a1 = vertices[inputs[0]];
                a2 = vertices[inputs[1]];
                b1 = vertices[inputs[2]];
                b2 = vertices[inputs[3]];
                vertices[i] = (9*a1 + 9*a2 - b1 - b2)/16.0;
                continue;
                
                // Internal 6 regular butterfly subdivision.
            case 8:
                a1 = vertices[inputs[0]];
                a2 = vertices[inputs[1]];
                b1 = vertices[inputs[2]];
                b2 = vertices[inputs[3]];
                
                c1 = vertices[inputs[4]];
                c2 = vertices[inputs[5]];
                c3 = vertices[inputs[6]];
                c4 = vertices[inputs[7]];
                
                /*
                 d1 = vertices[inputs[8]];
                 d2 = vertices[inputs[9]];
                 */
                
                // FIXME : I do not know if this is correct.
                vertices[i] = (8*(a1 + a2) + 2*(b1 + b2) -(c1 + c2 + c3 + c4))/16.0;// + d1 + d2;
                continue;
                
            default : throw new RuntimeError("Error in the topology Derivation data structures.");
        }
    }
}


// FIXME : Disperse this code up to the other functions.

// Performs one iteration of the subdivision.
void ofxButterfly::subdivide(int iterations, subdivision_type type, float pixel_prescision)
{
    // Subdivide. (Do I need to free memory???)

    for(int i = 0; i < iterations; i++)
    {
        switch(type)
        {
            case BUTTERFLY:
                current_WE = current_WE.ButterflySubdivide();
                break;
            case LINEAR:
                current_WE = current_WE.LinearSubdivide();
                break;
            case BOUNDARY:
                current_WE = current_WE.BoundaryTrianglularSubdivide(pixel_prescision);
                break;
            case PASCAL:
                current_WE = current_WE.SillyPascalSubdivide();
                break;
            default:
                throw new RuntimeError("Malformed type. We do not know how to subdivide the mesh in the given way.");
        }
    }
}