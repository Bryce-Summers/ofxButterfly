#include "ofApp.h"

#include "ofxButterfly.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    mesh.load("handmarksNew.ply");
    ofxButterfly butterfly;
    subdivided = butterfly.subdivide(mesh, 1);
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
    ofSetColor(ofColor::white);
    mesh.drawWireframe();
    
    // Draw the new subdivided edges in yellow.
    ofSetColor(ofColor::yellow);
    subdivided.drawWireframe();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}
