/*
 *  Particle.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"

Particle::Particle() {
	loc.set(0.f, 0.f, 0.f);
	
	_setDefaults();
}

Particle::Particle(float $x, float $y, float $z) {
	loc.set($x, $y, $z);
}

void Particle::addDamping(float $damp) {
	acc = (acc - vel) * $damp;
}

void Particle::seek(float $x, float $y, float $z) {
	steer(ofxVec3f($x, $y, $z), false);
}

void Particle::steer(ofxVec3f $target, bool $slowdown) {
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
void Particle::steer(ofxVec3f $target, bool $slowdown, float $scale) {
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

void Particle::addForce(ofxVec3f $loc) {
	acc += $loc;
}

void Particle::addForce(float $x, float $y) {
	acc.x += $x;
	acc.y += $y;
}

void Particle::addForce(float $x, float $y, float $z, float $minDist, float $scale) {
	ofxVec3f dir($x, $y, $z);
	dir = dir - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}

void Particle::addForce(ofxVec3f $loc, float $minDist, float $scale) {
	ofxVec3f dir = $loc - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}



void Particle::addClockwiseForce(ofxVec3f $loc, float $minDist, float $scale ) {
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


void Particle::addClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale ) {
	addClockwiseForce(ofxVec3f($x, $y, $z), $minDist, $scale );
}


void Particle::addCounterClockwiseForce(ofxVec3f $loc, float $minDist, float $scale ) {
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

void Particle::addCounterClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale ) {
	addCounterClockwiseForce( ofxVec3f($x, $y, $z), $minDist, $scale );
}

void Particle::spring(float $x, float $y, float $z, float $spring, float $minDist) {
	ofxVec3f diff = ofxVec3f($x, $y, $z) - loc;
	if (diff.length() < $minDist) {
		diff *= $spring;
		addForce( diff );
	}
}

void Particle::fixedSpring(Particle &p, float $spring, float $len) {
	ofxVec3f diff = p.loc - loc;
	float springForce = $spring * ($len - diff.length());
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	p.addForce( diff );
	
}

void Particle::fixedSpring(ofxVec3f &p, float $spring, float $len, bool $add) {
	ofxVec3f diff = p - loc;
	float springForce = $spring * ($len - diff.length());
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	if ($add) p += diff; 
}

void Particle::fixedSpring(ofxVec3f &p, float $spring, float $len, ofxVec3f &addVec) {
	ofxVec3f diff = p - loc;
	float springForce = $spring * ($len - diff.length());
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	addVec += diff; 
}

void Particle::fixedSpring(float $x, float $y, float $z, float $spring, float $len, ofxVec3f &addVec) {
	ofxVec3f p($x, $y, $z);
	ofxVec3f diff = p - loc;
	float springForce = ($spring * ($len - diff.length()));
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	addVec += diff; 
}



// Bounds checking ---------------------------------------

//------------------------------------------------------------
void Particle::bounceOffEdges(bool bDampedOnCollision) {
	
	// sometimes it makes sense to damped, when we hit
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (loc.x > maxx){
		loc.x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (loc.x < minx){
		loc.x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (loc.y > maxy){
		loc.y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (loc.y < miny){
		loc.y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
	
}

void Particle::checkExtendedEdges(float $buffer) {
	float buffer = $buffer;
	if (loc.x > ofGetWidth() + buffer ) loc.x = -buffer;
	if (loc.x < -buffer) loc.x = ofGetWidth() + buffer;
	
	if (loc.y > ofGetHeight() + buffer) loc.y = -buffer;
	if (loc.y < -buffer) loc.y = ofGetHeight() + buffer;
}


void Particle::elasticEdges( float $elastic, float $buffer ) {
	if (loc.x > ofGetWidth() + $buffer ) vel.x += -$elastic;
	if (loc.x < -$buffer)				vel.x += $elastic;
	
	if (loc.y > ofGetHeight() + $buffer) vel.y += -$elastic;
	if (loc.y < -$buffer)				vel.y += $elastic;
}








void Particle::_setDefaults() {
	
	MAX_SPEED = 15.f;
	MAX_STEER = .9f;
	
	DAMPING = 0.1f;
	
	vel.set(0.f, 0.f, 0.f);
	acc.set(0.f, 0.f, 0.f);
	
	prevLoc = loc;
}