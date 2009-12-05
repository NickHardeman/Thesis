#include "testApp.h"
#include "stdio.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofEnableSmoothing();
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(60);
	
	
	
}

//--------------------------------------------------------------
void testApp::update() {
	squid.update();
	
    
}

//--------------------------------------------------------------
void testApp::draw() {
	squid.seekMouse((float) mouseX, (float) mouseY);
	squid.draw();
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
