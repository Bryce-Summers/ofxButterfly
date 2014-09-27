#ifndef __GFX_MESH_HPP
#define __GFX_MESH_HPP

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <map>
#include <set>
#include <vector>
#include "vertex.hpp"
#include "edge.hpp"
#include "face.hpp"

namespace gfx
{

typedef std::map<Face, std::set<Edge> > FaceList;

struct EdgeList
{
  std::set<Vertex> vertices;
  std::set<Face> faces;
  std::set<Edge> edges;
};

typedef std::map<Edge, EdgeList> EdgeListMap;

typedef std::map<Vertex, std::set<Edge> > VertexList;

/* based on wikipedia article http://en.wikipedia.org/wiki/Polygon_mesh */
class WingedEdge
{
public:
    /* made these public for fromWingedEdge */
    FaceList faceList;
    EdgeListMap edgeListMap;
    VertexList vertexList;
    
    WingedEdge(){}
    
    Vertex AddVertex(GLfloat x, GLfloat y, GLfloat z);
    Edge AddEdge(const Vertex& v1, const Vertex& v2);
    Edge AddEdge(const Edge& e);
    Face AddFace(const Edge& e1, const Edge& e2, const Edge& e3);
    
    int NumVertices() const { return vertexList.size(); }
    int NumEdges() const { return edgeListMap.size(); }
    int NumFaces() const { return faceList.size(); }
    
    void Draw();
    
    // Linear interpolated subdivision. Triangles in/out.
    WingedEdge LinearSubdivide();
   
    // Butterfly subdivision with naive inner cases and boundary cases.
    // Triangles in/out.
    WingedEdge ButterflySubdivide();
    
    // Subdivides the boundaries smoothly. Does not subdivide interior triangles.
    // Triangles in/out.
    // if max_len > 0 this will only subdivide edges of length greater than min_len.
    WingedEdge BoundaryTrianglularSubdivide(float min_len = -1);
    
    // Subdivides exterior faces, deletes interior vertices.
    // This is not the most serious of subdivision schemes.
    WingedEdge SillyPascalSubdivide();
  
    
    
    
    /*
     * Special Derivation capable routinues.
     */
    WingedEdge BoundaryTrianglularSubdivide(std::map<Vertex, std::vector<Vertex> > &derivations);
    
    
private:

    // The internal subdivision algorithm that takes options and subdivides based on the user's wishes.
    WingedEdge Subdivide(bool linear, bool pascal);
    
    // Computes interpolated vertices.
    Vertex SubdivideEdge(const Face& f1, Edge& e, Vertex b1, bool linear);
    
    
    // -- Windged Edge transversal helper functions.
    
    Face GetAdjacentFace(const Face& face, const Edge& edge, bool &success);
    Vertex GetAdjacentVertex(const Face& face, const Edge& edge, bool &success);
    Vertex GetAdjacentFaceVertex(const Face& face, const Edge& edge, bool &success);
    
    // Some more helpful helper functions.
    int getNumAdjacentFaces(const Edge& edge);
    Vertex getOtherBoundaryVertice(Vertex &a, Edge &e);
    Vertex getOtherVertex(Edge &edge, Vertex &v);

    
    void performTriangulation(WingedEdge &mesh,
                                    Vertex &v1, Vertex &v2, Vertex &v3,
                                    Vertex &v4, Vertex &v5, Vertex &v6);
  
    // Returns the squared euclidean distance between the two vertices.
    float computeSqrOffset(Vertex v1,Vertex v2);

    
    /*
     * Special Derivation capable routines.
     */
    // Computes interpolated vertices.
    Vertex SubdivideBoundaryEdge(Edge& e, std::map<Vertex, std::vector<Vertex> > &derivations);
    
};

/* end */
}
#endif
