#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	CoM.set(300, 300, 0);
	pt1.set(0, -100, 0);
	pt2.set(0, 20, 0);
	//pt3.set(0, 100, 0);
	//p14.set(-100, 0, 0);
	
	angle = 0;
	radius = 20;
	
	bIsCloseEnough = false;
	
}

//--------------------------------------------------------------
void testApp::update() {
	bIsCloseEnough = false;
	mouseLoc.set(mouseX, mouseY);
	torque.set(0, 0);
	
	ofxVec3f loc = CoM + pt1;
	ofxVec3f diff = loc - mouseLoc;
	
	ofxVec3f normLoc = pt1;
	normLoc.normalize();
	ofxVec3f normFrc = diff;
	normFrc.normalize();
	
	//ofxVec2f rad(pt1.length(), 0);
	
	if ( diff.length() < radius ) {
		bIsCloseEnough = true;
		float angleBtw = normLoc.angleRad( normFrc );
		torque = pt1.length() * diff * sin( angleBtw );
		
		//ofxVec2f torque = pt1.cross( diff );
		//torque = normLoc.crossed( normFrc );
		cout << "angle: " << angleBtw << endl;
		cout << torque.x << ", " << torque.y << ", "<< torque.z << endl;
		cout << "diff "<<diff.x << ", " << diff.y << endl;
	}
	
	torque2.set(0, 0);
	loc = CoM + pt2;
	diff = loc - mouseLoc;
	normLoc = pt2;
	normLoc.normalize();
	normFrc = diff;
	normFrc.normalize();
	
	if ( diff.length() < radius ) {
		bIsCloseEnough = true;
		float angleBtw = normLoc.angleRad( normFrc );
		torque2 = pt2.length() * diff * sin( angleBtw );
		
		//ofxVec2f torque = pt1.cross( diff );
		//torque = normLoc.crossed( normFrc );
		//cout << "angle: " << angleBtw << endl;
		//cout << torque.x << ", " << torque.y << ", "<< torque.z << endl;
		//cout << "diff "<<diff.x << ", " << diff.y << endl;
	}
	
	torque.normalize();
	torque *= 20;
	
	torque2.normalize();
	torque2 *= 20;
	
	prevMouseLoc = mouseLoc;
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(255, 255, 255);
	if (bIsCloseEnough) ofCircle(mouseX, mouseY, radius);
	ofSetColor(0, 0, 0);
	ofLine(pt1.x + CoM.x, pt1.y + CoM.y, pt2.x + CoM.x, pt2.y + CoM.y);
	ofSetColor(255, 0, 0);
	ofCircle(CoM.x, CoM.y, 5);
	ofSetColor(0, 0, 0);
	ofCircle(pt1.x + CoM.x, pt1.y + CoM.y, 3);
	ofCircle(pt2.x + CoM.x, pt2.y + CoM.y, 3);
	ofSetColor(0, 255, 0);
	
	//torque.scale(20);
	ofLine(pt1.x + CoM.x, pt1.y + CoM.y, pt1.x + CoM.x + torque.x, pt1.y + CoM.y + torque.y);
	
	//torque2.scale(20);
	ofLine(pt2.x + CoM.x, pt2.y + CoM.y, pt2.x + CoM.x + torque2.x, pt2.y + CoM.y + torque2.y);
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

