#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	//ofSetVerticalSync(true);
	//ofSetFrameRate(60);
	
	virus.setup();
	
	float angle = 360 / 30;
	/*
	for (int i = 0; i < 30; i++) {
		//ofxVec2f len(ofRandom(10.f, 30.f), 0);
		ofxVec2f len(40.f, 0);
		len.rotate(angle * (float)i);
		//len *= ofRandom(.2f, 9.f);
		virus.addBasePt(len.x * ofRandom(.2, 10.f), len.y * ofRandom(.2, 10.f), len.x, len.y);
	}
	 */
	
	
	virus.addBasePt(0, -20, 0, -30);
	virus.addBasePt(20, -20, 10, -30);
	virus.addBasePt(20, -30, 10, -20);
	
	virus.addBasePt(20, -20, 10, -10);
	virus.addBasePt(20, -20, 20, -10);
	virus.addBasePt(30, -20, 30, -10);
	
	virus.addBasePt(30, 0, 30, 0);
	virus.addBasePt(30, 10, 30, 10);
	virus.addBasePt(20, 10, 20, 10);
	virus.addBasePt(10, 10, 10, 10);
	
	virus.addBasePt(10, 20, 10, 20);
	virus.addBasePt(10, 30, 10, 30);
	virus.addBasePt(0, 30, 0, 30);
	virus.addBasePt(-10, 30, -10, 30);
	
	virus.addBasePt(-10, 20, -10, 20);
	virus.addBasePt(-10, 10, -10, 10);
	virus.addBasePt(-20, 10, -20, 10);
	virus.addBasePt(-30, 10, -30, 10);
	
	virus.addBasePt(-30, 0, -30, 0);
	virus.addBasePt(-30, -10, -30, -10);
	virus.addBasePt(-20, -10, -20, -10);
	virus.addBasePt(-10, -10, -10, -10);
	
	virus.addBasePt(-10, -20, -10, -20);
	virus.addBasePt(-10, -30, -10, -30);
	 
}

//--------------------------------------------------------------
void testApp::update() {
	//virus.addForce(mouseX, mouseY, 0, 200, -0.39);
	virus.addBaseForce(mouseX, mouseY, 0, 200, -0.3f);
	virus.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	virus.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

