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

void Organism::addDamping(float $damp) {
	acc = (acc - vel) * $damp;
}

void Organism::seek(float $x, float $y, float $z) {
	steer(ofxVec3f($x, $y, $z), false);
}

void Organism::steer(ofxVec3f $target, bool $slowdown) {
    ofxVec3f steer;
    ofxVec3f desired = $target - loc;
    float d = desired.length();
	
    // If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0.f) {
		desired.normalize();
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if (($slowdown) && (d < 100.0f)) {desired *= (MAX_SPEED * (d/100.0f));} // This damping is somewhat arbitrary
		else {desired *= (MAX_SPEED); }
		
		steer = (desired - vel);
		steer.limit(MAX_STEER);
		
    } else {
		steer.set(0.f, 0.f, 0.f);
    }
    acc += steer;
}

// Daniel Shiffman //
void Organism::steer(ofxVec3f $target, bool $slowdown, float $scale) {
    ofxVec3f steer;
    ofxVec3f desired = $target - loc;
    float d = desired.length();
	
    // If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0.f) {
		desired.normalize();
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if (($slowdown) && (d < 100.0f)) {desired *= (MAX_SPEED * (d/100.0f));} // This damping is somewhat arbitrary
		else {desired *= (MAX_SPEED); }
		
		steer = (desired - vel);
		steer.limit(MAX_STEER);
		steer *= $scale;
    } else {
		steer.set(0.f, 0.f, 0.f);
    }
    acc += steer;
}



void Organism::addForce(float $x, float $y, float $z, float $minDist, float $scale) {
	ofxVec3f dir($x, $y, $z);
	dir = dir - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}

void Organism::addForce(ofxVec3f $loc, float $minDist, float $scale) {
	ofxVec3f dir = $loc - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}



void Organism::addClockwiseForce(Organism & org, float $minDist, float $scale ) {
	ofxVec3f dir = loc - org.loc;
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

void Organism::addClockwiseForce(ofxVec3f $loc, float $minDist, float $scale ) {
	ofxVec3f dir = loc - $loc;
	float d = dir.length();
	if (d < $minDist && d > 0) {
		float pct = 1 - (d / $minDist);
		dir.normalize();
		dir *= (MAX_SPEED);
		dir = (dir - vel);
		dir *= ($scale * pct);
		
		acc.x = acc.x - dir.y;
		acc.y = acc.y + dir.x;
	}
}


void Organism::addClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale ) {
	addClockwiseForce(ofxVec3f($x, $y, $z), $minDist, $scale );
}


void Organism::addCounterClockwiseForce(Organism & org, float $minDist, float $scale ) {
	ofxVec3f dir = loc - org.loc;
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

void Organism::addCounterClockwiseForce(ofxVec3f $loc, float $minDist, float $scale ) {
	ofxVec3f dir = loc - $loc;
	float d = dir.length();
	if (d < $minDist && d > 0) {
		float pct = 1 - (d / $minDist);
		dir.normalize();
		dir *= (MAX_SPEED);
		dir = (dir - vel);
		dir *= ($scale * pct);
		
		acc.x = acc.x + dir.y;
		acc.y = acc.y - dir.x;
		
	}
}

void Organism::addCounterClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale ) {
	addCounterClockwiseForce( ofxVec3f($x, $y, $z), $minDist, $scale );
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
	
	loc.set(200.f, 200.f, 0.f);
	vel.set(0.f, 0.f, 0.f);
	acc.set(0.f, 0.f, 0.f);
	
	prevLoc = loc;
}


void Organism::_checkEdges() {
	float buffer = BODY_HEIGHT * 4;
	if (loc.x > ofGetWidth() + buffer ) loc.x = -buffer;
	if (loc.x < -buffer) loc.x = ofGetWidth() + buffer;
	
	if (loc.y > ofGetHeight() + buffer) loc.y = -buffer;
	if (loc.y < -buffer) loc.y = ofGetHeight() + buffer;
}


////////////////////////////////////////////////////////////////////