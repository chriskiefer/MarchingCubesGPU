#include "testApp.h"
#include <sstream>

//--------------------------------------------------------------
void testApp::setup(){

	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
    
    mc.setup();

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    mc.prepareToDraw();
    cam.begin();
    mc.draw();
    cam.end();
    ofSetColor(255,255,255);
    stringstream fps;
    fps << "'d': change mode, mousex => isolevel, drag mouse => move camera\n";
    fps << "fps: " << ofGetFrameRate();
    ofDrawBitmapString(fps.str(), ofPoint(10,15));
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch(key) {
        case 'w':
            mc.toggleWireframe();
            break;
        case 'd':
            mc.nextData();
            break;
    }

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    mc.isolevel = ofGetMouseX() / (float)ofGetWidth();

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}