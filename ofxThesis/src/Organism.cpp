/*
 *  Organism.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Organism.h"

Organism::Organism() {
	_setDefaults();
}


// PUBLIC METHODS //////////////////////////////////////////////////////////////////////
void Organism::update() {
	
}



void Organism::addClockwiseForce(Organism & org, float $minDist, float $scale ) {
	ofxVec2f dir = loc - org.loc;
	float d = dir.length();
	if (d < $minDist && d > 0) {
		float pct = 1 - (d / $minDist);
		dir.normalize();
		dir *= (MAX_SPEED);
		dir = (dir - vel);
		dir *= ($scale * pct);
		
		acc.x = acc.x - dir.y;
		acc.y = acc.y + dir.x;
		
		org.acc.x = org.acc.x + dir.y;
		org.acc.y = org.acc.y - dir.x;
		
	}
}


void Organism::addCounterClockwiseForce(Organism & org, float $minDist, float $scale ) {
	ofxVec2f dir = loc - org.loc;
	float d = dir.length();
	if (d < $minDist && d > 0) {
		float pct = 1 - (d / $minDist);
		dir.normalize();
		dir *= (MAX_SPEED);
		dir = (dir - vel);
		dir *= ($scale * pct);
		
		acc.x = acc.x + dir.y;
		acc.y = acc.y - dir.x;
		
		org.acc.x = org.acc.x - dir.y;
		org.acc.y = org.acc.y + dir.x;
		
	}
}







///////////////////////////////////

void Organism::draw() {
	
}


////////////////////////////////////////////////////////////////////



// Protected Methods //////////////////////////////////////////////////////////////////////
void Organism::_setDefaults() {
	
	BODY_HEIGHT = 100.f;
	BODY_WIDTH = 30.f;
	
	MAX_SPEED = 15.f;
	MAX_STEER = .9f;
	
	loc.set(200.f, 200.f);
	vel.set(0.f, 0.f);
	acc.set(0.f, 0.f);
	
	prevLoc = loc;
}



////////////////////////////////////////////////////////////////////