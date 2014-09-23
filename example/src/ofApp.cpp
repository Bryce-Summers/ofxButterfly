#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    mesh.load("triangle.ply");
    
    subdivided = mesh;
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
    ofDrawBitmapString("Press R to reset the mesh!",    x, 200);
    ofDrawBitmapString("'B' = Butterfly subdivide",     x, 250);
    ofDrawBitmapString("'L' = linear subdivide",        x, 300);
    ofDrawBitmapString("'P' = pascal subdivide",        x, 350);
    ofDrawBitmapString("'E' = Boundary Subdivide",      x, 400);
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
    if(key == 'e')
    {
        subdivided = butterfly.subdivideBoundary(subdivided, 1.4, 1);
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
