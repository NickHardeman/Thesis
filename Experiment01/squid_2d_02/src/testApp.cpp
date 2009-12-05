#include "testApp.h"
#include "stdio.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofEnableSmoothing();
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(60);
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	
	minNoise = 0.499;
	maxNoise = 0.501;
	
	vx = vy = 0;
	
	counter = .1;
	
	xLoc = ofRandom(0, ofGetWidth());
	yLoc = ofRandom(0, ofGetHeight());
	
	perPointer = new Perlin(3, .01, .3, 15);
}

//--------------------------------------------------------------
void testApp::update() {
	
	/*
	 //Robert Hodgin //
	 // http://www.flight404.com/blog/?p=113 //
	 */
	
	float xyRads = getRads(xLoc, 0, 10.f, 20.f);
	float yRads = getRads(xLoc, yLoc, 10.f, 20.f);
	
	float xDegs = xyRads * RAD_TO_DEG;
	
	//cout << "these are the xDegs " << xDegs << endl;
	
	vx += cos(xyRads) * .25;
	vy += -sin(yRads) * .25;
	
	if (vx > 3) vx = 3;
	if (vx < -3) vx = -3;
	if (vy > 3) vy = 3;
	if (vy < -3) vy = -3;
	
	xLoc += vx;
	yLoc += vy;
	
	if (xLoc < 0) xLoc = ofGetWidth();
	if (xLoc > ofGetWidth()) xLoc = 0;
	if (yLoc < 0) yLoc = ofGetHeight();
	if (yLoc > ofGetHeight()) yLoc = 0;
	
	//squid.addDamping( .01 );
	//squid.seek((float) mouseX, (float) mouseY);
	
	squid.seek(xLoc, yLoc);
	squid.addForce(squidB.loc, 400, -2.3);
	squid.update();
	
	//squidB.addDamping( .01 );
	squidB.seek(xLoc, yLoc);
	squidB.addForce(squid.loc, 400, -2.3);
	squidB.update();
	
	counter += 1;
	if (counter > 70) {
		delete perPointer;
		//cout << "changing perlin" << endl;
		perPointer = new Perlin(3, .01, .3, (int)ofRandom(10, 250));
		counter = .1;
	}
}

float testApp::getRads(float $val1, float $val2, float $mult, float $div){
	float rads = perPointer->Get($val1/$div, $val2/$div);
	
	rads = ofMap(rads, -1, 1, 0, 1);
	
	if (rads < minNoise) minNoise = rads;
	if (rads > maxNoise) maxNoise = rads;
	
	rads -= minNoise;
	rads *= 1.0/(maxNoise - minNoise);
	
	return rads * $mult;
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(225, 225, 225);
	ofFill();
	ofCircle(xLoc, yLoc, 5);
	
	squid.draw();
	squidB.draw();
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
