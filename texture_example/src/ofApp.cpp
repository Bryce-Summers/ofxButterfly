#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    mesh.load("triangle.ply");
    
    butterfly.topology_start(mesh);
    
    for(int i = 0; i < 5; i++)
    butterfly.topology_subdivide_boundary();
    subdivided = butterfly.topology_end();
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(ofColor::black);

    // Draw the original edges white.
    // ofSetColor(ofColor::white);
    // mesh.drawWireframe();
    
    // Draw the new subdivided edges in yellow.
    ofSetColor(ofColor::white);
    subdivided.drawWireframe();
    
    int x = 700;
    ofDrawBitmapString("Press 'r' to reset the mesh!",  x, 200);
    ofDrawBitmapString("'b' = Butterfly subdivide",     x, 250);
    ofDrawBitmapString("'l' = linear subdivide",        x, 300);
    ofDrawBitmapString("'p' = pascal subdivide",        x, 350);
    ofDrawBitmapString("'e' = Boundary Subdivide",      x, 400);
    ofDrawBitmapString("'f' = recompute a boundary subdivided mesh.",  x, 450);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // Reset the mseh to the original subdivision.
    if(key == 'r')
    {
        subdivided = mesh;
        return;
    }
    
    // Butterfly subdivide the internal and boundary vertices.
    if(key == 'b')
    {
        subdivided = butterfly.subdivideButterfly(subdivided);
        return;
    }
    
    // Linearly subdividethe mesh.
    if(key == 'l')
    {
        subdivided = butterfly.subdivideLinear(subdivided);
        return;
    }
    
    // Subdivide the boundaries of the mesh.
    if(key == 'f')
    {
        
        //mesh.getVertices()[0] = ofVec3f(0, 0, 0);
        
        int start, end;
        start = ofGetElapsedTimeMicros();
        butterfly.fixMesh(mesh, subdivided);
        end = ofGetElapsedTimeMicros();
        
        cout << "Fast Edge Fixing Time : " << end - start << std::endl;
        
        
        //subdivided = butterfly.subdivideBoundary(subdivided, 2, 1);
        return;
    }
    
    if(key == 'e')
    {
        subdivided = butterfly.subdivideBoundary(subdivided, -1, 1);
        return;
    }
    
    // Subdivide the mesh using a whimpsical pascal's triangle scheme.
    if(key == 'p')
    {
        subdivided = butterfly.subdividePascal(subdivided);
        return;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

// Enable the user to drag ply files into the window.
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    if(dragInfo.files.size() > 0)
    {
        mesh.load(dragInfo.files.at(0));
        subdivided = mesh;
    }
}
