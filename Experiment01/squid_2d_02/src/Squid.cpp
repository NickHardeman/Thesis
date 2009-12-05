/*
 *  Squid.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Squid.h"

Squid::Squid() {
	_setDefaults();
	for (int i = 0; i < TENTACLE_LENGTH; i++) {
		tentacle.push_back( ofxVec3f(loc.x, loc.y, loc.z) );
	}
}


// PUBLIC METHODS //////////////////////////////////////////////////////////////////////
void Squid::update() {
	_checkBreathVolume();
	_breath += _getBreathSpeed();
	
	_pctBreath = _breath / (MAX_INHALE_BREATH - MIN_INHALE_BREATH);
	_pctBreath = _pctBreath < 0.f ? 0.f : _pctBreath;
	_pctBreath = _pctBreath > .99f ? .99f : _pctBreath;
	
	acc.limit( MAX_SPEED );
	
	ofxVec3f tarVel = vel + acc;
	tarVel *= ((1 - _pctBreath) + .01) * (MAX_SPEED);
	tarVel.limit(MAX_SPEED);
	float xeno = .4;
	//vel *= ((1 - _pctBreath) + .01) * (MAX_SPEED);
	vel = xeno * tarVel + (1 - xeno) * vel;
	
	vel.limit( MAX_SPEED );
	
	loc += vel;
	acc *= .99;
	
	_updateTentacles();
	
	/*
	 prevPos = pos;
	 pos.x = catchUpSpeed * catchX + (1-catchUpSpeed) * pos.x; 
	 pos.y = catchUpSpeed * catchY + (1-catchUpSpeed) * pos.y; 
	 angle = atan2(pos.y - prevPos.y, pos.x - prevPos.x);
	 */
	
	ofxVec3f dir = loc - prevLoc;
	dir.normalize();
	if (dir.length() > 0.f) {
		rot = atan2(dir.y, dir.x);
		// for the rotation matrix, don't forget to add 90 since it is oriented upwards //
		rot = 90 + (rot * RAD_TO_DEG);
	}
	
	//cout << "rotation = " << rot << ", dir.x = " << dir.x << ", dir.y = " << dir.y << ", pctBreah = " << _pctBreath << endl;
	
	prevLoc = loc;
	//acc.set(0, 0, 0);
	_checkEdges();
}

void Squid::addDamping(float $damp) {
	acc = (acc - vel) * $damp;
}

void Squid::addBreathForce() {
	ofxVec3f dir(vel.x, vel.y, 0);
	dir.normalize();
	
	dir *= .1;
	
	acc += dir;
}

void Squid::seek(float $x, float $y) {
	steer(ofxVec3f($x, $y), false);
}

void Squid::steer(ofxVec3f $target, bool $slowdown) {
    ofxVec3f steer = loc - prevLoc;
    ofxVec3f desired = $target - loc;
    float d = desired.length();
	float vPct = vel.length() / MAX_SPEED;
	steer.normalize();
	//float vPct = (steer.length());
	vPct = vPct < 0 ? 0 : vPct;
	vPct = vPct > .9 ? .9 : vPct;
    // If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0.f && vPct > 0.f) {
		desired.normalize();
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if (($slowdown) && (d < 100.0f)) {desired *= (MAX_SPEED * (d/100.0f));} // This damping is somewhat arbitrary
		else {desired *= (MAX_SPEED); }//desired *= (1 - _pctBreath);}
		//cout << "steer :: vel.length = " << vel.length() << ", MAX_SPEED = " << MAX_SPEED << ", vPct = " << vPct << endl;
		steer = (desired - (loc - prevLoc));
		steer.limit(MAX_STEER);
		steer *= ((vPct) + .1);
		
    } else {
		steer.set(0.f, 0.f, 0.f);
    }
    acc += steer;
}

void Squid::addForce(float $x, float $y, float $z, float $minDist, float $scale) {
	ofxVec3f dir($x, $y, $z);
	dir = dir - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}

void Squid::addForce(ofxVec3f $loc, float $minDist, float $scale) {
	ofxVec3f dir = $loc - loc;
	float d = dir.length();
	if ( d < $minDist) {
		dir.normalize();
		dir *= $scale * ( 1 - (d / $minDist) );
		acc += dir;
	}
}





///////////////////////////////////

void Squid::draw() {
	
	glPushMatrix();
	glTranslatef(loc.x, loc.y, loc.z);
	glRotatef(rot, 0, 0, 1);
	
	output.setColor(125, 125, 125);
	ofNoFill();
	ofSetLineWidth(3);
	
	output.beginShape();
	
	float cenX		= 0;
	float cenY		= 0;
	
	float pct = _breath / (MAX_INHALE_BREATH - MIN_INHALE_BREATH);
	float rad2 = (MAX_INHALE_BREATH - MIN_INHALE_BREATH) * pct;
	
	float bw = (BODY_WIDTH * .5);
	float bh = (BODY_HEIGHT * .25) + ( (BODY_HEIGHT * .25) * (1 - _pctBreath));
	
	float topBw = bw * .7;
	float botBw = bw * 1.5;
	
	float botCtPtSpread = botBw * .25;
	float bodyCtPtSpread = bh * .25;
	
	ofxVec3f ap1(cenX + topBw, cenY - bh, 0);
	
	ofxVec3f cp1a(cenX + rad2, cenY - bodyCtPtSpread, 0 );
	ofxVec3f cp1b(cenX + rad2, cenY + bodyCtPtSpread, 0 );
	
	ofxVec3f ap2(cenX + botBw, cenY + bh, 0);
	
	ofxVec3f cp2a(cenX + botCtPtSpread, (cenY + bh) + (bh * .25), 0);
	ofxVec3f cp2b(cenX - botCtPtSpread, (cenY + bh) + (bh * .25), 0);
	
	ofxVec3f ap3(cenX - botBw, cenY + bh, 0);
	
	ofxVec3f cp3a(cenX - rad2, cenY + bodyCtPtSpread, 0);
	ofxVec3f cp3b(cenX - rad2, cenY - bodyCtPtSpread, 0);
	
	ofxVec3f ap4(cenX - topBw, cenY - bh, 0);
	
	ofxVec3f cp4(cenX, (cenY - bh) - (bh * .1), 0);
	
	output.polyVertex(ap1.x, ap1.y);
	
	output.bezierVertex(cp1a.x, cp1a.y, cp1b.x, cp1b.y, ap2.x, ap2.y);
	
	output.bezierVertex(cp2a.x, cp2a.y, cp2b.x, cp2b.y, ap3.x, ap3.y);
	
	output.bezierVertex(cp3a.x, cp3a.y, cp3b.x, cp3b.y, ap4.x, ap4.y);
	
	output.bezierVertex(cp4.x, cp4.y, cp4.x, cp4.y, ap1.x, ap1.y);
	
	output.endShape(true);
	
	
	ofFill();
	ofSetColor(200, 200, 200);
	ofCircle(cp1a.x, cp1a.y, 2);
	ofCircle(cp1b.x, cp1b.y, 2);
	ofCircle(cp2a.x, cp2a.y, 2);
	ofCircle(cp2b.x, cp2b.y, 2);
	ofCircle(cp3a.x, cp3a.y, 2);
	ofCircle(cp3b.x, cp3b.y, 2);
	ofSetColor(0, 0, 0);
	//ofCircle(cp4.x, cp4.y, 2);
	
	ofSetColor(150, 150, 150);
	ofCircle(ap1.x, ap1.y, 1);
	ofCircle(ap2.x, ap2.y, 1);
	ofCircle(ap3.x, ap3.y, 1);
	ofCircle(ap4.x, ap4.y, 1);
	
	glPopMatrix();
	
	_renderTentacles();
}


////////////////////////////////////////////////////////////////////



// Private Methods //////////////////////////////////////////////////////////////////////
void Squid::_setDefaults() {
	INHALE_SPEED = .6;
	EXHALE_SPEED = 5.5;
	
	BODY_HEIGHT = 100;
	BODY_WIDTH = 30;
	
	MAX_INHALE_BREATH = BODY_HEIGHT * .75;
	MIN_INHALE_BREATH = (BODY_WIDTH * .5) - 5;
	
	// Number of Tentacle points //
	TENTACLE_LENGTH = 6;
	TENTACLE_SPACING = .1f;
	TENTACLE_SPRING = .2;
	
	_status = INHALING;
	
	_breath = (float) BODY_WIDTH;
	
	MAX_SPEED = 15.f;
	MAX_STEER = .9f;
	
	loc.set(200.f, 200.f, 0.f);
	vel.set(1.f, 1.f, 0.f);
	acc.set(0.f, 0.f, 0.f);
	
	prevLoc = loc;
}

float Squid::_getBreathSpeed() {
	if (_status == INHALING) {
		return INHALE_SPEED;
	} else if (_status == EXHALING) {
		return -1 * EXHALE_SPEED;
	}
}

void Squid::_checkBreathVolume() {
	if (_breath > MAX_INHALE_BREATH) {
		_status = EXHALING;
	} else if (_breath < MIN_INHALE_BREATH) {
		_status = INHALING;
	}
}

void Squid::_checkEdges() {
	float buffer = BODY_HEIGHT * 4;
	if (loc.x > ofGetWidth() + buffer ) loc.x = -buffer;
	if (loc.x < -buffer) loc.x = ofGetWidth() + buffer;
	
	if (loc.y > ofGetHeight() + buffer) loc.y = -buffer;
	if (loc.y < -buffer) loc.y = ofGetHeight() + buffer;
}

void Squid::_renderTentacles() {
	float pct;
	for (int i = 0; i < TENTACLE_LENGTH; i++) {
		
		pct = ( (float) i / (float) TENTACLE_LENGTH);
		int c = (pct * 125.f) + 100;
		
		ofSetColor(c, c, c);
		if (i < TENTACLE_LENGTH - 1) 
			ofLine(tentacle[i].x, tentacle[i].y, tentacle[i + 1].x, tentacle[i + 1].y);
		
		ofFill();
		ofSetColor(255, 255, 255);
		ofCircle(tentacle[i].x, tentacle[i].y, 5);
		
		
		ofNoFill();
		ofSetColor(c, c, c);
		ofCircle(tentacle[i].x, tentacle[i].y, 5);
	}
}


void Squid::_updateTentacles() {
	
	ofxVec3f dir(1, 0, 0);
	dir.rotate(rot + 90, ofxVec3f(0, 0, 1));
	dir.normalize();
	float bh = (BODY_HEIGHT * .25) + ( (BODY_HEIGHT * .25) * (1 - _pctBreath)) * .9;
	dir *= bh;
	dir += loc;
	
	ofSetColor(0, 0, 50);
	//ofCircle(dir.x, dir.y, 5);
	
	_dragSegment(tentacle[0], dir, TENTACLE_SPACING);
	
	for (int i = 1; i < TENTACLE_LENGTH; i++) {
		_dragSegment(tentacle[i], tentacle[i - 1], TENTACLE_SPACING);
	}
	
}

void Squid::_dragSegment(ofxVec3f & $segment, ofxVec3f & $tarSeg, float $length) {
	
	ofxVec3f dir = $tarSeg - $segment;
	float angle = atan2(dir.y, dir.x);
	
	/////////////////
	float w = ($segment.x + cos(angle) * $length) - $segment.x;
	float h = ($segment.y + sin(angle) * $length) - $segment.y;
	
	float xLoc = $tarSeg.x - w;
	float yLoc = $tarSeg.y - h;
	
	$segment.x += (xLoc - $segment.x) * TENTACLE_SPRING;
	$segment.y += (yLoc - $segment.y) * TENTACLE_SPRING;
	
	//glPushMatrix();
	//glTranslatef($tarSeg.x, $tarSeg.y, $tarSeg.z);
	
	//ofxVec3f locLoc($segment.x - $tarSeg.x, $segment.y - $tarSeg.y);
	
	//ofSetColor(220, 220, 220);
	//ofCircle(locLoc.x, locLoc.y, 3);
	
	//glRotated(angle * RAD_TO_DEG, 0, 0, 1);
	//glPopMatrix();
	
	//////
	
	
}

////////////////////////////////////////////////////////////////////





