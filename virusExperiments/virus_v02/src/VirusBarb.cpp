/*
 *  VirusBarb.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "VirusBarb.h"

// NOTICE: THESE ARE PLACED IN WORK COORDS IN THE VIRUS BASE CLASS !! //
// EASIER FOR CHECKING WITH OTHER ITEMS IN WORLD SPACE //

VirusBarb::VirusBarb() {
	status = STATUS_ALIVE;
	FIRE_VEL = 10.f;
	MAX_SPEED = FIRE_VEL;
	MAX_STEER = FIRE_VEL;
	
	DAMPING = .01;
	bodyPtIndex = 0;
}

void VirusBarb::setDirection(float $x, float $y) {
	dirVec.set($x, $y);
	dirVec.normalize();
}

void VirusBarb::setDirection( ofxVec2f $dir ) {
	dirVec = $dir;
	dirVec.normalize();
}

void VirusBarb::fire(ofxVec2f $vel) {
	status = STATUS_SEEKING;
	vel = $vel;
	vel.normalize();
	vel *= FIRE_VEL;
}

void VirusBarb::setBodyPtIndex( int $i ) {
	bodyPtIndex = $i;
}

void VirusBarb::updateTarget(float $x, float $y) {
	target.set($x, $y);
}

void VirusBarb::update() {
	
	addDamping( DAMPING );
	vel += acc;
	vel.limit( MAX_SPEED );
	loc += vel;
	
	// this lil guy is looking around for someone to kill //
	if (status == STATUS_SEEKING) {
		dirVec = vel.normalized();
		checkRemove();
	}
	
	acc.set(0, 0);	
}

void VirusBarb::checkRemove() {
	if (loc.x > ofGetWidth() - 25) {
		status = STATUS_REMOVE;
	}
	if (loc.x < 25) {
		status = STATUS_REMOVE;
	}
	if (loc.y < 25) {
		status = STATUS_REMOVE;
	}
	if (loc.y > ofGetHeight() - 25) {
		status = STATUS_REMOVE;
	}
	
}

void VirusBarb::draw() {
	
	dirVec *= 7;
	ofSetColor(255, 0, 0);
	ofLine(loc.x, loc.y, loc.x + dirVec.x, loc.y + dirVec.y);
	
	dirVec.normalize();
	dirVec *= -9;
	
	ofxVec2f endLoc = dirVec.getPerpendicular();
	endLoc *= 3;
	
	dirVec += loc;
	
	ofNoFill();
	ofSetColor(100, 100, 100);
	ofTriangle(loc.x, loc.y, 
			   
			   dirVec.x + endLoc.x, dirVec.y + endLoc.y, dirVec.x - endLoc.x, dirVec.y - endLoc.y);
	
	ofFill();
	ofSetColor(50, 50, 50);
	ofCircle(dirVec.x, dirVec.y, 1);
}