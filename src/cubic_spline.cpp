/*
 * cubic_spline.cpp
 * ofPuppetButterfly
 * Created by Bryce Summers on 9/16/14.
 * Adapted to C++ from Bryce's personal code endeavors.
 *
 * These are not actually used in the project,
 * but rather are just cool mathematical objects that people should know about.
 */


#include "cubic_spline.h"

// Computes the Coefficients of a cubic polynomial.
// p1 and p2 are control points, t1 and t2 are tangant vectors that are desired at the two control points.
cubic_spline::cubic_spline(ofVec3f p1, ofVec3f t1, ofVec3f p2, ofVec3f t2)
{
	/*
	 * A =  2*p1 - 2*p2 + t1 + t2;
	 * B = -3*p1 + 3*p2 - t1 - 2*t2
	 * C = t2;
	 * D = p1;
	 */
		
	A = (2*p1)  - (2*p2) + t1 + t2;
	B = (-3*p1) + (3*p2) - t1 - (2*t2);
	C = t2;
	D = p1;
}
    
// REQUIRES : input should be between 0 and 1.0;
// ENSURES : Returns the point along the spline for the given time.
ofVec3f cubic_spline::eval(float input)
{
	if(input < 0 || input > 1.0)
	{
		throw RuntimeError("Error: Input not in function domain!");
	}
	
	double x_1 = input;
	double x_2 = x_1*input;
	double x_3 = x_2*input;
		
	return A*x_3 + B*x_2 + C*x_1 + D;
    
}