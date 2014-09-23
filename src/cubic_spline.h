//
//  cubic_spline.h
//  ofPuppetButterfly
//
//  Created by Tube on 9/16/14.
//
//

#ifndef __ofPuppetButterfly__cubic_spline__
#define __ofPuppetButterfly__cubic_spline__

#include "ofMain.h"
#include "error.hpp"

class cubic_spline
{
    
private:
    
    ofVec3f A, B, C, D;
    
public:
	// Constructor.
	cubic_spline(ofVec3f p1, ofVec3f t1, ofVec3f p2, ofVec3f t2);
    
	// Destructor.
	virtual ~cubic_spline();
    
    // Interpolation procedures.
    ofVec3f eval(float f);
    
};



#endif /* defined(__ofPuppetButterfly__cubic_spline__) */
