#include "testApp.h"
#include "stdio.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofEnableSmoothing();
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(60);
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	
}

//--------------------------------------------------------------
void testApp::update() {
	//tick.seek(mouseX, mouseY, 0);
	//tick.steer(ofxVec3f $target, bool $slowdown, float $scale);
	tick.setTarget(mouseX, mouseY);
	tick.update();
}


//--------------------------------------------------------------
void testApp::draw() {
	tick.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    switch (key){
       
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
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

void testApp::windowResized(int w, int h){

}
