/*
 * ofxButterfly.h
 *
 *  Created on: Sep 4, 2014
 *      Author: Bryce Summers
 *
 *      Purpose : This class allows the user to subdivide ofMeshes via adaptive Butterfly Subdivision.
 *
 *                oxfButterfly::subdivide() gurantees that the original indices of the ofMesh will be preserved.
 *      Note: All of these subdivision procudure assume that the ofmesh has triangular faces.
 *
 *      September 26th, 2014 : The efficiently topologically identical repetition functions implementation has begun.
 *
 * To view the original unoptimized hopefully correct version without optimizations, please see: 
 *
 *     https://github.com/Bryce-Summers/ofxButterfly/releases/tag/v1.0
 *
 *
 */

#ifndef OFXBUTTERFLY_H_
#define OFXBUTTERFLY_H_

#include "ofMesh.h"
#include "vertex.hpp"
#include "edge.hpp"
#include "mesh.hpp"

class ofxButterfly
{
    
    
public:
	// Constructor.
	ofxButterfly();

	// Destructor.
	virtual ~ofxButterfly();
    
    
    // Single subdivisions.
    ofMesh subdivideButterfly(ofMesh &mesh, int iterations = 1);
    ofMesh subdivideLinear(ofMesh &mesh, int iterations = 1);
    ofMesh subdividePascal(ofMesh &mesh, int iterations = 1);
    ofMesh subdivideBoundary(ofMesh &mesh, float pixel_prescision, int iterations = 1);
    
    
    
    // -- Batch subdivision pipeline.
    
    // Prepares the given mesh for subdivision.
    void subdivide_start(ofMesh &mesh);
    
    // Subdivision procedures, requires trianglular meshes.
    void subdivideButterfly(int iterations = 1);
    void subdivideLinear   (int iterations = 1);
    void subdividePascal   (int iterations = 1);
    void subdivideBoundary (float pixel_prescision, int iterations = 1);
    
    // Ends the current subdivision.
    ofMesh subdivide_end();
    
    // -- Fast algorithms for repeatedly computing subdivisions of meshes with the same topology.
    
    // Gives ofxButterfly a mesh with a particular topology.
    // REQUIRES : mesh should be made of triangles.
    void topology_start(ofMesh &mesh);
    
    /* Topology subdivision routines.
     * REQUIRES : topology_start should have been called.
     * ENSURES :
     *  Uses the windged Edge structure of the current topology mesh.
     *  Subdivides the Winged Edge structure using a non adaptive boundary subdivision sheme.
     *  Computes and stores a mapping (new vertice v --> <a, b, c, d>),
     *  where a, b, c, d are the original boundary vertices that are used to compute v.  v = (9*a + 9*b - c - d)/16
     *
     * mesh will not be mutated.
     * the returned mesh is a boundary subdivided version of mesh.
     */
    void topology_subdivide_boundary();
    /*
    void topology_subdivide_pascal();
    void topology_subdivide_linear();
    void topology_subdivide_butterfly();
    */
    
    // Returns the mesh that is the result of all of the topology_subdivide_ calls.
    ofMesh topology_end();
    
    /* fixMesh
     * REQUIRES : mesh should have the same topology as the mesh sent to the previous call of topology_start.
     *            the subdivided_mesh should have been returned from the previous call to topology_end.
     *            the subdivided_mesh should also not have been mutated outside of ofxButterfly.
     * ENSURES  : the mesh is not mutated.
     *            subdivided mesh will be mutated in the following way:
     *            the vertices present in subdivided_mesh that have been changed in mesh will be updated to the new mesh locations.
     *            We will use the mapping constructed in the caching function() to compute the new locations for the subdivision
     *            vertices and fix their locations.
     */
    void fixMesh(ofMesh &mesh, ofMesh &subdivided_mesh);
    
private:
    
    // Subdivision routines.
    enum subdivision_type {BUTTERFLY, LINEAR, BOUNDARY, PASCAL};
    void subdivide(int iterations, subdivision_type type, float pixel_prescision = -1);
    
    inline void deriveVertices(int first_derived_indice, ofVec3f * vertices, int max_indice);
    inline void deriveTextureVertices(int first_derived_indice, ofVec2f * vertices, int max_indice);
    
    // -- Subdivision data structures.
    
    // A map from vertices to mesh indices.
    std::map<gfx::Vertex, int> map_vertice_index;
    
    // A map from mesh indices to vertices.
    std::map<int, gfx::Vertex> map_index_vertice;
    
    // The current windged edge structure.
    gfx::WingedEdge current_WE;
    
    // An mapping that maps indices in a subdivided mesh to the indices in a defored mesh needed to derive its position.
    std::map<int, std::vector<int> > transformation;
    
    
};

#endif /* OFXBUTTERFLY_H_ */
