#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	ofBackground(255, 255, 255);
	
	pos.set(300, 300);
	vel.set(0, 0);
	frc.set(0, 0);
	av = 0.f;
	torque = 0.f;
	
	recW = 40;
	recH = 20;
	
	for (int i = 0; i < 4; i++) {
		ofxVec2f p;
		vertices.push_back( p );
	}
	
	//vertices[0].set(10, 0);
	vertices[0].set(-recW * .5, -recH * .5);
	vertices[1].set(recW * .5, -recH * .5);
	vertices[2].set(-recW * .5, recH * .5);
	vertices[3].set(recW * .5, recH * .5);
	
	orientation = 0.f;
	
	float density = 0.006f;
	
	mass = density * recW * recH;
	OneOverMass = 1.f / mass;
	
	I = 0.f;
	
	//cout << vertices[0].dot( vertices[0] ) << endl;
	
	float ptMass = mass / vertices.size();
	
	for (int i = 0; i < vertices.size(); i++) {
		I += vertices[i].dot(vertices[i]) * ptMass;
	}
	
	I = (mass / 12) * ( (recW * recW) + (recH * recH) );
	
	OneOverMomentOfInertia = 1.f / I;
	
	bAddGravity		= false;
	bAddMouseFrc	= true;
	bWorldSpring	= true;
	
	minMouseDist	= 20.f;
	damping			= .3f;
	
	//ofSetFrameRate(7);
}

//--------------------------------------------------------------
void testApp::update() {
	
	if (bAddGravity) {
		frc += ofxVec2f(0, 1.f);
	}
	
	if (bAddMouseFrc) {
		for(int i = 0; i < vertices.size(); i++) {
			addMouseForce(vertices[i]);
		}
		
	}
	if (bWorldSpring) {
		
	}
	
	frc += -vel * damping;
	
	frc *= OneOverMass;
	vel += frc;
	pos += vel;
	
	torque += -av * 25.f; 
	//av*= 0.95f;
	//av += ( torque / (I*0.02) );
	av += ( torque * OneOverMomentOfInertia );
	
	//av += torque;
	orientation += av;
	
	//cout << orientation << endl;
	
	vertices[0].set(-recW * .5, -recH * .5);
	vertices[1].set(recW * .5, -recH * .5);
	vertices[3].set(-recW * .5, recH * .5);
	vertices[2].set(recW * .5, recH * .5);
	
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].rotateRad( orientation );
		//vertices[i].rotate( ofRadToDeg(orientation) );
	}
	
	frc.set(0.f, 0.f);
	torque = 0.f;
	
}

void testApp::addMouseForce(ofxVec2f& pt) {
	ofxVec2f worldLoc = pt + pos;
	ofxVec2f mouseLoc(mouseX, mouseY);
	ofxVec2f diff = mouseLoc - worldLoc;
	if (diff.length() < minMouseDist) {
		float pct = 1 - (diff.length() / minMouseDist );
		diff *= pct;
		frc -= diff;
		torque -= pt.getPerpendicular().dot( (diff) );
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(200, 200, 200);
	ofCircle(mouseX, mouseY, minMouseDist);
	
	ofSetColor(150, 150, 150);
	ofCircle(pos.x, pos.y, 3);
	
	for (int i = 0; i < vertices.size(); i++) {
		int index = i + 1;
		if (i == vertices.size() - 1) index = 0;
		ofLine(pos.x + vertices[i].x, pos.y + vertices[i].y, pos.x + vertices[index].x, pos.y + vertices[index].y);
	}
	
	ofSetColor(255, 0, 0);
	for (int i = 0; i < vertices.size(); i++) {
		ofCircle(pos.x + vertices[i].x, pos.y + vertices[i].y, 3);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	switch (key) {
		case 'g':
			bAddGravity = !bAddGravity;
			break;
		case 'm':
			bAddMouseFrc = !bAddMouseFrc;
			break;
		case 'w':
			bWorldSpring = !bWorldSpring;
			break;
		default:
			break;
	}

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

