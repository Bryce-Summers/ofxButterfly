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
 */

#ifndef OFXBUTTERFLY_H_
#define OFXBUTTERFLY_H_

#include "ofMesh.h"
#include "vertex.hpp"
#include "edge.hpp"
#include "cubic_spline.h"

class ofxButterfly
{
    
    
public:
	// Constructor.
	ofxButterfly();

	// Destructor.
	virtual ~ofxButterfly();

	// Subdivision procedures, requires trianglular meshes.
    ofMesh subdivideButterfly(ofMesh mesh, int iterations = 1);
    ofMesh subdivideLinear   (ofMesh mesh, int iterations = 1);
    ofMesh subdividePascal   (ofMesh mesh, int iterations = 1);
    ofMesh subdivideBoundary (ofMesh mesh, int iterations = 1);
    
private:
    
    enum subdivision_type {BUTTERFLY, LINEAR, BOUNDARY, PASCAL};
    ofMesh subdivide(ofMesh mesh, int iteration, subdivision_type type);
    ofMesh subdivide(ofMesh mesh, subdivision_type type);

};

#endif /* OFXBUTTERFLY_H_ */
