/*
 * ofxButterfly.h
 *
 *  Created on: Sep 4, 2014
 *      Author: Bryce Summers
 *
 *      Purpose : This class allows the user to subdivide ofMeshes via adaptive Butterfly Subdivision.
 *
 *                oxfButterfly::subdivide() gurantees that the original indices of the ofMesh will be preserved.
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

	// Subdivision procedures.
    ofMesh subdivide(ofMesh mesh, int iterations = 1);
    ofMesh subdivide(ofMesh mesh);

};

#endif /* OFXBUTTERFLY_H_ */
