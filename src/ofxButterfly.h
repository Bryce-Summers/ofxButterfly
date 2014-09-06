/*
 * ofxButterfly.h
 *
 *  Created on: Sep 4, 2014
 *      Author: Bryce Summers
 *
 *      Purpose : This class allows the user to subdivide ofMeshes.
 */

#ifndef OFXBUTTERFLY_H_
#define OFXBUTTERFLY_H_

#include "ofMesh.h"
#include "vertex.hpp"
#include "edge.hpp"

class ofxButterfly
{
public:
	// Constructor.
	ofxButterfly();

	// Destructor.
	virtual ~ofxButterfly();

	void setup(); // if necessary, perhaps there are parameters to set

	// Subdivision procedures.
    ofMesh subdivide(ofMesh mesh, int iterations = 1);

};

#endif /* OFXBUTTERFLY_H_ */
