/*
 *  VirusBase.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "VirusBase.h"

bool bRemoveBarb(VirusBarb $b) {
	return $b.status == STATUS_REMOVE;
	//return !t.bFoundThisFrame;
}

void VirusBase::setup(float $x, float $y) {
	loc.set($x, $y);
	BODY_HEIGHT = 30;
	
	WALL_PADDING = 75.f;
	
	setMass( 5.f );
	calculateMOIRectangle(20.f, 40.f);
	
	torque = 0.f;
	angVel = .0f;
	orientation = 0.f;
	
	ANGULAR_DAMPING = 25.f;
	barbIndex = 0;
	bFireBarb = false;
	MAX_SPEED = 3.f;
}

void VirusBase::calculateMOIRectangle(float $w, float $h) {
	momentOfInertia = (mass / 12) * ( ($w * $w) + ($h * $h) );
	oneOverMomentOfInertia = 1 / momentOfInertia;
}

void VirusBase::calculateMOICircle(float $r) {
	momentOfInertia = ( mass * ($r * $r) ) * .5f;
	oneOverMomentOfInertia = 1 / momentOfInertia;
}

void VirusBase::calculateMOIRigidBody() {
	momentOfInertia = 0.f;
	float ptMass = mass / barbs.size();
	
	for (int i = 0; i < barbs.size(); i++) {
		momentOfInertia += barbs[i].loc.dot(barbs[i].loc) * ptMass;
	}
	oneOverMomentOfInertia = 1 / momentOfInertia;
}

void VirusBase::setMass( float $m ) {
	mass = $m;
	oneOverMass = 1 / mass;
}

void VirusBase::constructCircleBase(float $numPoints, float $r) {
	setMass( $r * .07 );
	float angle = 360 / $numPoints;
	
	 for (int i = 0; i < $numPoints; i++) {
		 ofxVec2f len($r, 0);
		 len.rotate(angle * (float)i);
		 
		 addBasePt(len.x, len.y, len.x, len.y);
	 }
	
	calculateMOICircle( $r );
	 
}

void VirusBase::constructCrossBase(float $w, float $h) {
	float w = $w * .5;
	float h = $h * .5;
	
	float w_1 = w * .333333f;
	float w_2 = w * .666666f;
	
	float h_1 = h * .333333f;
	float h_2 = h * .666666f;
	
	addBasePt(0, -20, 0, -h);
	addBasePt(20, -20, w_1, -h);
	addBasePt(20, -30, w_1, -h_2);
	
	addBasePt(20, -20, w_1, -h_1);
	addBasePt(20, -20, w_2, -h_1);
	addBasePt(30, -20, w, -h_1);
	
	addBasePt(30, 0, w, 0);
	addBasePt(30, 10, w, h_1);
	addBasePt(20, 10, w_2, h_1);
	addBasePt(10, 10, w_1, h_1);
	
	addBasePt(10, 20, w_1, h_2);
	addBasePt(10, 30, w_1, h);
	addBasePt(0, 30, 0, h);
	addBasePt(-10, 30, -w_1, h);
	
	addBasePt(-10, 20, -w_1, h_2);
	addBasePt(-10, 10, -w_1, h_1);
	addBasePt(-20, 10, -w_2, h_1);
	addBasePt(-30, 10, -w, h_1);
	
	addBasePt(-30, 0, -w, 0);
	addBasePt(-30, -10, -w, -h_1);
	addBasePt(-20, -10, -w_2, -h_1);
	addBasePt(-10, -10, -w_1, -h_1);
	
	addBasePt(-10, -20, -w_1, -h_2);
	addBasePt(-10, -30, -w_1, -h);
	
	calculateMOIRigidBody();
}

void VirusBase::addBasePt(float $x, float $y, float $tarX, float $tarY) {
	VirusBarb barb;
	barb.loc.x = $x;
	barb.loc.y = $y;
	//barb.DAMPING = .09f;
	barb.MAX_SPEED = 50.f;
	barb.setBodyPtIndex( bodyPts.size() );
	barb.init();
	
	VirusBodyPoint bodyPt;
	bodyPt.loc.set( $x, $y);
	bodyPt.originPt.set( $tarX, $tarY );
	
	ofxVec2f normalUp(0, 1);
	ofxVec2f locNormal = bodyPt.originPt.normalized();
	
	float bOrientation = normalUp.angleRad( locNormal ) + ( PI * .5f);
	
	bodyPt.orientation = bOrientation;
	
	bodyPts.push_back( bodyPt );
	barbs.push_back( barb );
	
	//barbs[barbs.size()-1].init( &bodyPts[bodyPts.size()-1] );
}

void VirusBase::setRandomLocations(float $minMult, float $maxMult) {
	for (int i = 0; i < bodyPts.size(); i++) {
		ofxVec2f startLoc = bodyPts[i].originPt;
		startLoc *= ofRandom($minMult, $maxMult);
		bodyPts[i].loc.set( startLoc );
	}
}

void VirusBase::setRandomVelocities( ofxVec2f $vel, float $offsetX, float $offsetY, float $minMult, float $maxMult) {
	ofxVec2f newVel = $vel;
	
	for (int i = 0; i < bodyPts.size(); i++) {
		newVel = $vel;
		newVel.x += ofRandom(-$offsetX, $offsetX);
		newVel.y += ofRandom(-$offsetY, $offsetY);
		
		newVel *= ofRandom($minMult, $maxMult);
		bodyPts[i].vel = newVel;
	}
}

// x, y and z are global coords //
void VirusBase::addBaseForce(float $x, float $y, float $minDist, float $scale) {
	ofxVec2f dir($x, $y);
	dir = dir - loc;
	for (int i = 0; i < bodyPts.size(); i++) {
		//barbs[i].addForce(dir, $minDist, $scale);
		addForceBodyPoint(bodyPts[i], dir, $minDist, $scale);
	}
}

void VirusBase::addForceBodyPoint(Particle2D& bp, ofxVec2f $dir, float $scale) {
	acc += $dir;
	bp.acc += $dir * $scale;
	torque += bp.loc.getPerpendicular().dot( ($dir) );
}

void VirusBase::addForceBodyPoint(VirusBodyPoint& bp, ofxVec2f $dir, float $minDist, float $scale) {
	ofxVec2f diff = $dir - bp.loc;
	if (diff.length() < $minDist) {
		diff.normalize();
		diff *= $scale * ( 1 - (diff.length() / $minDist) );
		acc += diff;
		bp.acc += diff * 1.5;
		torque += bp.loc.getPerpendicular().dot( (diff) );
	}
}

void VirusBase::setMouseLoc( float $x, float $y) {
	mouseX = $x; mouseY = $y;
}

// used to fire virus like a gun //
void VirusBase::fire() {
	//barbs[barbIndex].setDirection( bodyPts[barbIndex].loc );
	addForceBodyPoint(bodyPts[barbIndex], -bodyPts[barbIndex].loc.normalize() * barbs[barbIndex].FIRE_VEL * 10, .07);
	barbs[barbIndex].fire( bodyPts[barbIndex].loc );
	barbIndex++;
	if (barbIndex >= barbs.size()) {
		barbIndex = 0;
	}
}

void VirusBase::reload() {
	for (int i = 0; i < bodyPts.size(); i++) {
		barbs[i].status = STATUS_ALIVE;
	}
}

void VirusBase::update() {
	
	// remove_if sorts to the end via a boolean value, 
	// http://en.wikipedia.org/wiki/Erase-remove_idiom
	//barbs.erase( remove_if(barbs.begin(), barbs.end(), bRemoveBarb), barbs.end() );
	
	for (int i = 0; i < barbs.size(); i++) {
		//barbs[i].spring(bodyPts[i].x, bodyPts[i].y, 0.f, .02f, 500.f);
		if (barbs[i].status == STATUS_SEEKING) {
			//barbs[i].updateTarget(mouseX, mouseY);
			//barbs[i].seek(mouseX, mouseY);
			barbs[i].updateBaseLoc(bodyPts[barbs[i].bodyPtIndex].loc.x + loc.x, bodyPts[barbs[i].bodyPtIndex].loc.y + loc.y);
			barbs[i].steer(ofxVec2f(mouseX, mouseY), false, 1.9);
		}
		if (barbs[i].status != STATUS_REMOVE) {
			barbs[i].update();
		}
	}
	
	for (int i = 0; i < bodyPts.size(); i++) {
		bodyPts[i].fixedSpring(0.f, 0.f, 0.3f, bodyPts[i].originPt.length(), acc);
	}
	
	for (int i = 0; i < bodyPts.size(); i++) {
		//elasticEdges( bodyPts[i], 1.75, -WALL_PADDING, .08f);
		bounceOffEdges( bodyPts[i], .7f, WALL_PADDING, .08f );
		bodyPts[i].update( );
	}
	
	addDamping(.0009);
	
	bounceOffEdges( .04f, WALL_PADDING );
	acc *= oneOverMass;
	
	vel += acc;
	vel.limit( MAX_SPEED );
	loc += vel;
	
	torque += -angVel * ANGULAR_DAMPING; 
	angVel += ( torque * oneOverMomentOfInertia );
	orientation += angVel;
	
	for (int i = 0; i < bodyPts.size(); i++) {
		bodyPts[i].loc.set(bodyPts[i].loc.length(), 0);
		bodyPts[i].loc.rotateRad( orientation + bodyPts[i].orientation );
		// these barbs are drawn in world coords, because once fired it is easier to keep track //
		if (barbs[i].status == STATUS_ALIVE) {
			barbs[i].loc.set(bodyPts[barbs[i].bodyPtIndex].loc.x + loc.x, bodyPts[barbs[i].bodyPtIndex].loc.y + loc.y);
			barbs[i].setDirection( bodyPts[barbs[i].bodyPtIndex].loc );
		}
	}
	
	acc.set(0, 0);
	torque = 0.f;
}

void VirusBase::draw() {
	
	glPushMatrix();
	glTranslatef(loc.x, loc.y, 0);
	ofSetColor(150, 150, 150);
	ofCircle(0, 0, 5);
	ofSetColor(0, 0, 0);
	
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < bodyPts.size(); i++) {
		ofVertex(bodyPts[i].loc.x, bodyPts[i].loc.y);
		ofCircle(bodyPts[i].loc.x, bodyPts[i].loc.y, 1);
		ofSetColor(0, 0, 0);
		ofCircle(bodyPts[i].loc.x, bodyPts[i].loc.y, 2);
	}
	ofEndShape(true);
	
	glPopMatrix();
	
	
	for (int i = 0; i < barbs.size(); i++) {
		//ofSetColor(0, 0, 0);
		//ofCircle(barbs[i].loc.x, barbs[i].loc.y, 2);
		//if (barbs[i].status == STATUS_ALIVE) {
		//	ofxVec2f diff;
		//	if (i == 0) {
		//		diff = barbs[barbs.size() - 1].loc - barbs[i + 1].loc;
		//	} else if (i == barbs.size() - 1) {
		//		diff = barbs[i - 1].loc - barbs[0].loc;
		//	} else {
		///		diff = barbs[i-1].loc - barbs[i + 1].loc;
		//	}
			
			//barbs[i].setDirection( diff.getPerpendicular() );
		//}
		barbs[i].draw();
	}
	
	 
}

void VirusBase::bounceOffEdges( float $damping, float $wallPadding ) {
	float minx = $wallPadding;
	float miny = $wallPadding;
	float maxx = ofGetWidth() - $wallPadding;
	float maxy = ofGetHeight() - $wallPadding;
	
	bool bDidICollide;
	
	if (loc.x > maxx){
		//loc.x = maxx; // move to the edge, (important!)
		//vel.x *= -1;
		vel.x -= $damping;
		//vel.x *= $damping;
		bDidICollide = true;
	} else if (loc.x < minx){
		//loc.x = minx; // move to the edge, (important!)
		vel.x += $damping;
		//vel.x *= $damping;
		bDidICollide = true;
	}
	
	if (loc.y > maxy){
		//loc.y = maxy; // move to the edge, (important!)
		vel.y -= $damping;
		//vel.y *= $damping;
		bDidICollide = true;
	} else if (loc.y < miny){
		//loc.y = miny; // move to the edge, (important!)
		vel.y += $damping;
		//vel.y *= $damping;
		bDidICollide = true;
	}
	
	if (bDidICollide == true) {
		//vel *= $damping;
	}
}


//------------------------------------------------------------
// friction is the amount of traction that the particle induces, i.e. higher traction will imitate a sticky surface //
void VirusBase::bounceOffEdges(Particle2D &pt, float $damping, float $wallPadding, float $friction) {
	ofxVec2f frc(0, 0);
	ofxVec2f totVel = pt.vel + vel;
	// since it is added when it is past wall (elastic) adds way too much rotation,
	// may work with stiff walls ? //
	//ofxVec2f tanVec = pt.loc.getPerpendicular();
	//tanVec.normalize();
	//tanVec *= angVel * 2.5f;
	//totVel += tanVec;
	
	// what are the walls
	float minx = $wallPadding;
	float miny = $wallPadding;
	float maxx = ofGetWidth() - $wallPadding;
	float maxy = ofGetHeight() - $wallPadding;
	
	bool bDidICollide;
	
	if (loc.x + pt.loc.x > maxx){
		pt.loc.x = maxx - loc.x; // move to the edge, (important!)
		pt.vel.x *= -1;
		frc.y = -totVel.y * $friction;
		bDidICollide = true;
	} else if (loc.x + pt.loc.x < minx){
		pt.loc.x = minx - loc.x; // move to the edge, (important!)
		pt.vel.x *= -1;
		frc.y = -totVel.y * $friction;
		bDidICollide = true;
	}
	
	if (loc.y + pt.loc.y > maxy){
		pt.loc.y = maxy - loc.y; // move to the edge, (important!)
		pt.vel.y *= -1;
		frc.x = -totVel.x * $friction;
		bDidICollide = true;
	} else if (loc.y + pt.loc.y < miny){
		pt.loc.y = miny - loc.y; // move to the edge, (important!)
		pt.vel.y *= -1;
		frc.x = -totVel.x * $friction;
		bDidICollide = true;
	}
	
	if (bDidICollide == true) {
		pt.vel *= $damping;
	}
	addForceBodyPoint(pt, frc, 1.4f);
	
}

void VirusBase::elasticEdges( Particle2D &pt, float $elastic, float $buffer, float $friction ) {
	ofxVec2f frc(0, 0);
	ofxVec2f totVel = pt.vel + vel;
	// since it is added when it is past wall (elastic) adds way too much rotation,
	// may work with stiff walls ? //
	//ofxVec2f tanVec = pt.loc.getPerpendicular();
	//tanVec.normalize();
	//tanVec *= angVel;
	//totVel += tanVec;
	
	if (loc.x + pt.loc.x > ofGetWidth() + $buffer ) {
		frc.x = -$elastic;
		frc.y = -totVel.y * ($elastic * $friction );
	}
	if (loc.x + pt.loc.x < -$buffer) {
		frc.x += $elastic;
		frc.y = -totVel.y * ($elastic * $friction );
	}
	
	if (loc.y + pt.loc.y > ofGetHeight() + $buffer) {
		frc.x = -totVel.x * ($elastic * $friction);
		frc.y += -$elastic;
	}
	if (loc.y + pt.loc.y < -$buffer) {
		frc.x = -totVel.x * ($elastic * $friction);
		frc.y += $elastic;
	}
	//pt.addForce( frc );
	addForceBodyPoint(pt, frc, 1.4f);
}



