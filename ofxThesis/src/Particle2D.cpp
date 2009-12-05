/*
 *  Particle2D.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle2D.h"

Particle2D::Particle2D() {
	loc.set(0.f, 0.f);
	
	_setDefaults();
}

Particle2D::Particle2D(float $x, float $y) {
	loc.set($x, $y);
}

void Particle2D::addDamping(float $damp) {
	acc = (acc - vel) * $damp;
}

void Particle2D::seek(float $x, float $y) {
	steer(ofxVec2f($x, $y), false);
}

void Particle2D::steer(ofxVec2f $target, bool $slowdown) {
    ofxVec2f steer;
    ofxVec2f desired = $target - loc;
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
		steer.set(0.f, 0.f);
    }
    acc += steer;
}

// Daniel Shiffman //
void Particle2D::steer(ofxVec2f $target, bool $slowdown, float $scale) {
    ofxVec2f steer;
    ofxVec2f desired = $target - loc;
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
		steer.set(0.f, 0.f);
    }
    acc += steer;
}

void Particle2D::addForce(ofxVec2f $loc) {
	acc += $loc;
}

void Particle2D::addForce(float $x, float $y) {
	acc.x += $x;
	acc.y += $y;
}

void Particle2D::addForce(float $x, float $y, float $minDist, float $scale) {
	ofxVec2f dir($x, $y);
	dir = dir - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}

void Particle2D::addForce(ofxVec2f $loc, float $minDist, float $scale) {
	ofxVec2f dir = $loc - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}



void Particle2D::addClockwiseForce(ofxVec2f $loc, float $minDist, float $scale ) {
	ofxVec2f dir = loc - $loc;
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


void Particle2D::addClockwiseForce(float $x, float $y, float $minDist, float $scale ) {
	addClockwiseForce(ofxVec2f($x, $y), $minDist, $scale );
}


void Particle2D::addCounterClockwiseForce(ofxVec2f $loc, float $minDist, float $scale ) {
	ofxVec2f dir = loc - $loc;
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

void Particle2D::addCounterClockwiseForce(float $x, float $y, float $minDist, float $scale ) {
	addCounterClockwiseForce( ofxVec2f($x, $y), $minDist, $scale );
}

void Particle2D::spring(float $x, float $y, float $spring, float $minDist) {
	ofxVec2f diff = ofxVec2f($x, $y) - loc;
	if (diff.length() < $minDist) {
		diff *= $spring;
		addForce( diff );
	}
}

void Particle2D::fixedSpring(Particle2D &p, float $spring, float $len) {
	ofxVec2f diff = p.loc - loc;
	float springForce = $spring * ($len - diff.length());
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	p.addForce( diff );
	
}

void Particle2D::fixedSpring(ofxVec2f &p, float $spring, float $len, bool $add) {
	ofxVec2f diff = p - loc;
	float springForce = $spring * ($len - diff.length());
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	if ($add) p += diff; 
}

void Particle2D::fixedSpring(ofxVec2f &p, float $spring, float $len, ofxVec2f &addVec) {
	ofxVec2f diff = p - loc;
	float springForce = $spring * ($len - diff.length());
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	addVec += diff; 
}

void Particle2D::fixedSpring(float $x, float $y, float $spring, float $len, ofxVec2f &addVec) {
	ofxVec2f p($x, $y);
	ofxVec2f diff = p - loc;
	float springForce = ($spring * ($len - diff.length()));
	diff.normalize();
	diff *= springForce;
	
	addForce( -diff );
	addVec += diff; 
}



// Bounds checking ---------------------------------------

//------------------------------------------------------------
void Particle2D::bounceOffEdges(bool bDampedOnCollision) {
	
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

void Particle2D::checkExtendedEdges(float $buffer) {
	float buffer = $buffer;
	if (loc.x > ofGetWidth() + buffer ) loc.x = -buffer;
	if (loc.x < -buffer) loc.x = ofGetWidth() + buffer;
	
	if (loc.y > ofGetHeight() + buffer) loc.y = -buffer;
	if (loc.y < -buffer) loc.y = ofGetHeight() + buffer;
}


void Particle2D::elasticEdges( float $elastic, float $buffer ) {
	if (loc.x > ofGetWidth() + $buffer ) vel.x += -$elastic;
	if (loc.x < -$buffer)				vel.x += $elastic;
	
	if (loc.y > ofGetHeight() + $buffer) vel.y += -$elastic;
	if (loc.y < -$buffer)				vel.y += $elastic;
}





void Particle2D::_setDefaults() {
	
	MAX_SPEED = 15.f;
	MAX_STEER = .9f;
	
	DAMPING = 0.01f;
	
	vel.set(0.f, 0.f);
	acc.set(0.f, 0.f);
	
	prevLoc = loc;
}