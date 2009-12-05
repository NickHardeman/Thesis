/*
 *  Tick.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tick.h"

Tick::Tick() {
	_setDefaults();
}

// PUBLIC FUNCTIONS //
void Tick::update() {
	_checkEdges();
	
	addDamping(.03);
	
	updateLegs();
	
	acc += rLeg.getLegAcc(LEG_FORCE).rotate(rot, ofxVec3f(0, 0, 1) );
	acc += lLeg.getLegAcc(LEG_FORCE).rotate(rot, ofxVec3f(0, 0, 1) );
	
	steer(target, false, .02);
	
	//cout << "rot = " << rot << endl;
	//cout << "acc.x, acc.y = " << acc.x << ", " << acc.y << endl;
	
	vel += acc;
	loc += vel;
	
	direction = loc - prevLoc;
	direction.normalize();
	if (direction.length() > 0.f) {
		rot = atan2(direction.y, direction.x);
		rot = (rot * RAD_TO_DEG);
	}
	
	prevLoc = loc;
	
	acc.set(0, 0, 0);
}

void Tick::updateLegs() {
	_legTimer += 1;
	//cout << "legTimer" << _legTimer << endl;
	
	if(_legTimer >= MAX_LEG_TIMER) {
		//cout << "direction = "<< direction.x << ", " << ", target = " << target.x << ", " << target.y << endl;
		ofxVec3f dir = (loc) - target;
		dir.normalize();
		dir.rotate(-rot, ofxVec3f(0, 0, 1) );
		
		float r = atan2(dir.y, dir.x);
		r = r * RAD_TO_DEG;
		
		cout << "r = " << r << endl;
		
		isRightLeg = r <= 0 ? false : true;
		
		if (isRightLeg) {
			rLeg.setTarLoc(true, .3f, true);
			_legTimer = 0;
		} else {
			lLeg.setTarLoc(true, .3f, true);
			_legTimer = 0;
		}
		
	}
	
	if (rLeg.checkExtended()) {
		// cout << endl << endl << "we are setting the timer back to zero" << endl << endl;
		rLeg.setTarLoc(false, .15f, false);
	}
	
	if (lLeg.checkExtended()) {
		// cout << endl << endl << "we are setting the timer back to zero" << endl << endl;
		lLeg.setTarLoc(false, .15f, false);
	}
	
	rLeg.update();
	lLeg.update();
}

void Tick::setTarget(float $x, float $y) {
	target.set($x, $y, 0.f);
}



void Tick::draw() {
	glPushMatrix();
	
	ofSetColor(150, 150, 150);
	glTranslatef(loc.x, loc.y, loc.z);
	glRotatef(rot, 0, 0, 1);
	
	ofCircle(0, 0, 4);
	
	ofSetColor(255, 0, 0);
	ofCircle(rLeg.loc.x, rLeg.loc.y, 3);
	ofCircle(lLeg.loc.x, lLeg.loc.y, 3);
	
	ofSetColor(150, 150, 200);
	ofCircle(rLeg.tarLoc.x, rLeg.tarLoc.y, 1);
	ofCircle(lLeg.tarLoc.x, lLeg.tarLoc.y, 1);
	
	glPopMatrix();
}



// PRIVATE FUNCTIONS //

void Tick::_setDefaults() {
	Organism::_setDefaults();
	
	MAX_LEG_TIMER = 45;
	_legTimer = 0;
	
	LEG_FORCE = -.35;
	rLeg.setLegLengths( 10.f, 60.f);
	rLeg.setLegAngle(150.f);
	
	lLeg.setLegLengths( 10.f, 60.f);
	lLeg.setLegAngle(-150.f);
	
	target.set(0.f, 0.f, 0.f);
			
	isRightLeg = true;
		
	BODY_HEIGHT = 5;
	BODY_WIDTH = 5;
	
	MAX_SPEED = 8.f;
	MAX_STEER = .09f;
}