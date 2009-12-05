#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	ofBackground(255, 255, 255);
	
	pos.set(100, 400);
	vel.set(8, 18);
	frc.set(0, 0);
	av = 0.f;
	torque = 0.f;
	
	recW = 40.f;
	recH = 20.f;
	
	bIsHitting = bIsEmbedded = false;
	radius = 30.f;
	
	wallStartPos.set(0, 500);
	wallNormal.set(0, -1);
	
	orientation = 0.f;
	
	float density = 0.006f;
	
	mass = density * recW * recH;
	OneOverMass = 1.f / mass;
	
	I = 0.f;
	
	//cout << vertices[0].dot( vertices[0] ) << endl;
	
	float ptMass = mass / vertices.size();
	
	for (int i = 0; i < vertices.size(); i++) {
		//I += vertices[i].dot(vertices[i]) * ptMass;
	}
	
	I = (mass / 12) * ( (recW * recW) + (recH * recH) );
	
	OneOverMomentOfInertia = 1.f / I;
	
	float ang = 0.f; 
	for (int i = 0; i < 4; i++) {
		ofxVec2f p;
		vertices.push_back( p );
		angles.push_back( ang );
		ang += (.5 * PI);
	}
	
	bAddGravity		= false;
	bAddMouseFrc	= true;
	bWorldSpring	= true;
	
	minMouseDist	= 20.f;
	damping			= .15f;
	
	mouseLoc.set(0, 0);
	mouseVel.set(3, 4);
}

//--------------------------------------------------------------
void testApp::update() {
	bIsHitting = bIsEmbedded = false;
	
	ofxVec2f mousePos(mouseX, mouseY);
	mouseVel = mousePos - mouseLoc;
	mouseLoc = mousePos;
	
	ofxVec2f diff = wallStartPos - pos;
	float d = diff.dot(wallNormal);
	d = d < 0 ? d * -1 : d;
	if (( d ) < radius) {
		bIsEmbedded = true;
	}
	
	frc += ofxVec2f(0, 1);
	
	frc += -vel * damping;
	
	frc *= OneOverMass;
	vel += frc;
	pos += vel;
	
	torque += -av * 25.f; 
	av += ( torque * OneOverMomentOfInertia );
	
	orientation += av;
	
	
	
	if (pos.y + radius >= wallStartPos.y) {
		pos.y = wallStartPos.y - radius;
		float e = .15;
		ofxVec2f moment(radius, 0);
		ofxVec2f velTan = moment.getPerpendicular();
		velTan *= av;
		
		ofxVec2f angularVel = vel + velTan;
		
		cout << "velTan: " << velTan.x << ", " << velTan.y << " length: " << velTan.length() << endl;
		
		float dp1 = moment.cross( wallNormal );
		
		//cout << "this is the dp1: " << dp1 << endl;
		
		float J = -1 * (1 + e) * ofxVec2f( angularVel ).dot( wallNormal ) / ( 1 / mass ) + ( ( dp1 * dp1 ) / I );
		vel = vel + ((J * wallNormal) / mass);
		//cout << "av before: " << av << endl;
		//av = av + ( J * dp1 ) / I;
		//float addedAngVel = (moment.getPerpendicular().dot(J * wallNormal)) / I;
		//float addedAngVel = J * dp1 / I;
		//cout << "dot: " << moment.getPerpendicular().dot(wallNormal) << ", J cross " << moment.getPerpendicular().cross(J * wallNormal) << endl;
		//cout << "J " << J << "  crossed then * J " << J * moment.getPerpendicular().cross(wallNormal) << endl;
		//J = J < 0 ? J * -1 : J;
		//float addedAngVel = moment.getPerpendicular().dot(J * wallNormal) / I;
		
		//ofxVec2f frictionFrc;
		//frictionFrc = wallNormal.getPerpendicular() * vel.normalized() * -1;
		//vel += frictionFrc;
		
		float cp = angularVel.cross(wallNormal);
		cout << "velTan.length: " << velTan.length() << endl;
		velTan *= cp;
		av -= 1;
		cout << "cp: " << cp << " velTan.length: " << velTan.length() << endl;
		
		//cout << "wall cross vel: " << wallNormal.cross(vel) << endl;
		
		//av = av + addedAngVel;
		//cout << "av after: " << av << "  addedAngVel: " << addedAngVel << endl;
	}
	
	
	
	
	
	
	
	for (int i = 0; i < 4; i++) {
		vertices[i].set(radius - 5, 0);
		vertices[i].rotateRad( orientation + angles[i] );
	}
	
	frc.set(0.f, 0.f);
	torque = 0.f;
	
}

void testApp::addMouseForce( ) {
	//ofxVec2f worldLoc = pt + pos;
	ofxVec2f worldLoc = pos;
	ofxVec2f diff = mouseLoc - worldLoc;
	if (diff.length() < minMouseDist + radius) {
		float pct = 1 - (diff.length() / minMouseDist );
		ofxVec2f mv = mouseVel;
		mv *= pct;
		frc -= mv;
		ofxVec2f pt = diff;
		pt.normalize();
		pt *= radius;
		torque -= pt.getPerpendicular().dot( (mv) );
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(0, 0, 0);
	ofLine(wallStartPos.x, wallStartPos.y, wallStartPos.x + ofGetHeight(), wallStartPos.y);
	
	ofSetColor(175, 175, 175);
	ofCircle(mouseX, mouseY, minMouseDist);
	
	if (bIsEmbedded)
		ofSetColor(255, 0, 0);
	else if (bIsHitting) 
		ofSetColor(255, 0, 0);
	else 
		ofSetColor(150, 150, 150);
	ofNoFill();
	ofCircle(pos.x, pos.y, radius);
	ofFill();
	ofSetColor(0, 0, 0);
	for (int i = 0; i < 4; i++) {
		ofCircle(vertices[i].x + pos.x, vertices[i].y + pos.y, 2);
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	

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

