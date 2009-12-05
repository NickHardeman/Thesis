#include "testApp.h"
#include "stdio.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofEnableSmoothing();
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(60);
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	
	slider_width = 150.f;
	slider_height = 10.f;
	
	slider_legTimer.x = ofGetWidth() - slider_width - 20;
	slider_legTimer.y = 200;
	slider_legTimer.setValues(5.f, 150.f);
	slider_legTimer.setDimensions(slider_width, slider_height);
	slider_legTimer.setID(0);
	slider_legTimer.setValue( tick.MAX_LEG_TIMER );
	slider_legTimer.addEventListeners();
	ofAddListener(slider_legTimer.SLIDER_EVENT, this, &testApp::onSliderLegTimerEvent);
	
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

void testApp::onSliderLegTimerEvent(OF_SLIDER_EVENT & $e) {
	tick.MAX_LEG_TIMER = $e.value;
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
