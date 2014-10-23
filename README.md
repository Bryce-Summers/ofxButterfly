ofxButterfly
============

Butterfly Subdivision wrapper library for Open Frameworks.

Authors: Bryce Summers, Kyle McDonald

Purpose: This wrapper library, wraps the butterfly subdivision implementation found here: https://github.com/jawr/cpp-butterfly-subdivision, for use with subdividing Open Frameworks OfMesh meshes.


This Wrapper library was written under the auspices of the Studio for Creative Inquiry at Carnegie Mellon University:
http://studioforcreativeinquiry.org/

The Original article about Butterfly Subdivision:
http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf

Article about Butterfly Subdivision:
http://www.gamasutra.com/view/feature/131584/implementing_subdivision_surface_.php?print=1


Cababilities
===

<B>Example Input</B>
![alt text](https://github.com/Bryce-Summers/ofxButterfly/blob/master/Images/_Input%20Triangle.png "Input Triangle")


<B>Butterfly Subdivision</B>



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
    /* 4 boundary subdivisions that will stop subdividing when the new 	 * points are closer than 1.3 pixels apart. */
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

The butterfly subdivision does not properly handle all special interior cases.