#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	
	slider.x = 400;
	slider.y = 200;
	slider.setValues(100, 200);
	slider.setDimensions(150.f, 10.f);
	
	slider.setID(0);
	
	slider.addEventListeners();
	
	ofAddListener(slider.SLIDER_EVENT, this, &testApp::onSliderEvent);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

void testApp::onSliderEvent(OF_SLIDER_EVENT & $e) {
	$e.toString();
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

