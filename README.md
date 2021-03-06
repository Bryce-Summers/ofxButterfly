ofxButterfly
============

Subdivision library for Open Frameworks.

###Authors:
Bryce Summers, Kyle McDonald

###Purpose:
This wrapper library, wraps the butterfly subdivision implementation found here: https://github.com/jawr/cpp-butterfly-subdivision, for use with subdividing Open Frameworks OfMesh meshes.
There were also extensive modifications and improvments to the original library.

###Interesting Reading
The Original article about Butterfly Subdivision:
http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf

Article about Butterfly Subdivision:
http://www.gamasutra.com/view/feature/131584/implementing_subdivision_surface_.php?print=1

### SFCI
This Wrapper library was written under the auspices of the Studio for Creative Inquiry at Carnegie Mellon University:
http://studioforcreativeinquiry.org/



Cabability Images
===

Example Input
---

![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/_Input%20Triangle.png "Input Triangle")

Butterfly Subdivision
---

<B>1 Butterfly Subdivision</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/1%20butterfly.png "1 Butterfly Subdivision")

<B>2 Butterfly Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/2%20butterfly.png "2 Butterfly Subdivision")

<B>3 Butterfly Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/3%20butterfly.png "3 Butterfly Subdivision")

<B>4 Butterfly Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/4%20butterfly.png "4 Butterfly Subdivision")

Boundary Subdivision
---

<B>1 Boundary Subdivision</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/1%20boundary.png "1 Boundary Subdivision")

<B>2 Boundary Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/2%20boundary.png "2 Boundary Subdivision")

<B>3 Boundary Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/3%20boundary.png "3 Boundary Subdivision")

<B>4 Boundary Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/4%20boundary.png "4 Boundary Subdivision")

Linear Subdivision
---

<B>1 Linear Subdivision</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/1%20linear.png "1 Linear Subdivision")

<B>2 Linear Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/2%20linear.png "2 Linear Subdivision")

<B>3 Linear Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/3%20linear.png "3 Linear Subdivision")

<B>4 Linear Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/4%20linear.png "4 Linear Subdivision")

Pascal Subdivision
---

<B>1 Pascal Subdivision</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/1%20pascal.png "1 Silly Pascal Subdivision")

<B>2 Pascal Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/2%20pascal.png "2 Silly Pascal Subdivision")

<B>3 Pascal Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/3%20pascal.png "3 Silly Pascal Subdivision")

<B>4 Pascal Subdivisions</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/4%20pascal.png "4 Silly Pascal Subdivision")

Usage
=====

<B>Normal Subdivision</B>

    ofMesh mesh, subdivided;
    ofxButterfly butterfly;// No setup() required.
    mesh.load("triangle.ply");
    
    
    int iterations = [Number of subdivisions desired];
    subdivided = butterfly.subdivideButterfly(mesh, iterations);
    //subdivided = butterfly.subdivideLinear(linear, iterations);
    //subdivided = butterfly.subdivideBoundary(subdivided, -1, 1);
    //subdivided = butterfly.subdividePascal(subdivided);
    

<B>Batch Subdivision</B>


    ofMesh mesh, subdivided;
    ofxButterfly butterfly;// No setup() required.
    mesh.load("triangle.ply");

    // Start the batch subdivision.
    butterfly.subdivision_start(mesh);
    
    // Perform the desired operations.
    butterfly.subdivideButterfly();// 1 subdivision.
    butterfly.subdivideLinear();
    butterfly.subdividePascal();
    butterfly.subdivideBoundary(-1);
    
    butterfly.subdivideButterfly(5);// 5 subdivisions.
    butterfly.subdivideLinear(-5);  // 0 subdivisions.
    butterfly.subdividePascal(3);   // 3 subdivisions.
    /* 4 boundary subdivisions that will stop subdividing when the new
     * points are closer than 1.3 pixels apart. */
    butterfly.subdivideBoundary(4, 1.3);
    
    // Finish the batch subdivision.
    subdivided = butterfly.subdivide_end();
    
    /*
     * Please note that it may be safe to call the sudivide_end()
     * function multiple times and extract subdivision meshes along the
     * way as long as subdivide_start() is only called at the start.
     */

<B>Fast Subdivision Recomputations:</B>


    ofMesh mesh, subdivided;
    ofxButterfly butterfly;

    // ---  Subdividing a first mesh.
    mesh.load("triangle.ply");
    
    // Initiate turbo charged precomputation.
    butterfly.topology_start(mesh);
    
    // Precompute a sequence of subdivisions.
    butterfly.topology_subdivide_butterfly();
    butterfly.topology_subdivide_linear();
    butterfly.topology_subdivide_pascal();
    butterfly.topology_subdivide_boundary();
    butterfly.topology_subdivide_butterfly();
    butterfly.topology_subdivide_boundary();
    butterfly.topology_subdivide_boundary();
    
    // Get a mesh that can be efficiently recomputed in linear time.
    subdivided = butterfly.topology_end();
    
    
    
    /* --- Fix the mesh 
     *	whenever we change the positions of the original mesh vertices.
     *
     * This will destructivly mutate the subdivided mesh into the 
     * correct subdivision for any updated mesh that is isomorphic to 
     * 'mesh' and has vertices at the same indices. An easy way to tae 
     * the orginal 'mesh' and mutate it only by changing the position 
     * vectors for each of its vertices, therebye keeping the triangular 
     * faces and indices of the vertices the same.
     */
     
     butterfly.fixMesh(updatedmesh, subdivided);
     
     
    /*
     * Please note that it may be safe to call the topology_end()
     * function multiple times and extract subdivision meshes along the
     * way as long as topology_start() is only called at the start.
     *
     * The fixMesh(um, sub) function will work for any updatedmesh 'um' 
     * that is isomorphic to the orginal mesh 'mesh' and has vertices at 
     * the same indices as in 'mesh'. sub must be one of the meshes 
     * returned from a call to butterfly.topology_end().
     */

	/* WARNING: never call topology_start() on any new mesh using the 
	 * same butterfly object. If you need multiple meshes to be 
	 * turbosubdivided, then use multiple butterfly objects.
	 */




Special Thanks To:
=================
Kyle McDonald for helping out with Open Frameworks issues.

Lauren McCarthy for providing expertise and encouragment.

Golan Levin for leading the project that motivated the creation of this Wrapper Library.

Note : Watch out with the boundary subdivision scheme. If used excessivly, some numerical errors will produce undesired results.

Warning:
--------
The butterfly subdivision does not properly handle all special interior cases. Let the author know if you would be interested in the special cases being handled to allow for extra fairness properties in the algorithm.

Interesting Mathematical Note:
------------

To subdive a curve defined by a list of points, simply split every edge in two with the point defined by:

<p align="center">
-A/16 + 9B/16 + 9C/16 - D/16
</p>

where A, B, C, and D are the points in order on the entire curve, where the subdivision edge is BC.
