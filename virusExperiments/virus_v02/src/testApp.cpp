#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	//ofSetVerticalSync(true);
	ofSetFrameRate(120);
	//ofEnableSmoothing();
	
	WALL_PADDING = 75.f;
	
	virus.setup(ofGetWidth() * .5, ofGetHeight() * .5);
	virus.WALL_PADDING = WALL_PADDING;
	//virus.constructCrossBase(100, 100);
	virus.constructCircleBase( 20.f, 50.f );
	//virus.addBasePt(20.f, -10, 20.f, 20);
	
	 
}

//--------------------------------------------------------------
void testApp::update() {
	//virus.addForce(mouseX, mouseY, 0, 200, -0.39);
	virus.addBaseForce(mouseX, mouseY, 100, -0.9f);
	virus.setMouseLoc(mouseX, mouseY);
	virus.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	ofFill();
	ofSetColor(225, 225, 225);
	
	ofRect(0, 0, ofGetWidth(), WALL_PADDING);
	ofRect(0, 0, WALL_PADDING, ofGetHeight());
	ofRect(ofGetWidth() - WALL_PADDING, 0, WALL_PADDING, ofGetHeight());
	ofRect(0, ofGetHeight() - WALL_PADDING, ofGetWidth(), WALL_PADDING);
	
	ofSetColor(0, 0, 0);
	ofDrawBitmapString("Frame Rate: "+ofToString(ofGetFrameRate()), 20, 20);
	
	virus.draw();
	
	ofNoFill();
	ofCircle(mouseX, mouseY, 100);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch (key) {
		case OF_KEY_UP:
			virus.addForce( 0, -220.f );
			break;
		case OF_KEY_RIGHT:
			virus.addForce(220.f, 0);
			break;
		case OF_KEY_DOWN:
			virus.addForce(0, 220.f);
			break;
		case OF_KEY_LEFT:
			virus.addForce(-220.f, 0);
			break;
		case ' ':
			virus.fire();
			break;
		case 'r':
			virus.reload();
			break;
		case 'z':
			virus.angVel -= .005;
			break;
		case 'x':
			virus.angVel += .005;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

