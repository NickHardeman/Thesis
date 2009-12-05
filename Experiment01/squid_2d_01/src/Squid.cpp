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
}


// PUBLIC METHODS //////////////////////////////////////////////////////////////////////
void Squid::update() {
	_checkBreathVolume();
	_breath += _getBreathSpeed();
	
	_pctBreath = _breath / (MAX_INHALE_BREATH - MIN_INHALE_BREATH);
	_pctBreath = _pctBreath < 0.f ? 0.f : _pctBreath;
	_pctBreath = _pctBreath > .99f ? .99f : _pctBreath;
	
	//vel += acc;
	//vel *= ((1 - _pctBreath) + .001) * (MAX_SPEED * 1.4);
	//vel.limit(MAX_SPEED);
	
	//acc *= ((1 - _pctBreath) + .01) * (MAX_SPEED);
	
	//cout << "acc = " << acc.length() << endl;
	
	acc.limit( MAX_SPEED );
	
	ofxVec3f tarVel = vel + acc;
	tarVel *= ((1 - _pctBreath) + .01) * (MAX_SPEED);
	tarVel.limit(MAX_SPEED);
	float xeno = .4;
	//vel *= ((1 - _pctBreath) + .01) * (MAX_SPEED);
	vel = xeno * tarVel + (1 - xeno) * vel;
	
	vel.limit( MAX_SPEED );
	
	loc += vel;
	
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
		//if (desired.length() < .01) {
		//	steer.set(0.f, 0.f, 0.f);
		//} else {
			// Steering = Desired minus Velocity
			//cout << "steer :: vel.length = " << vel.length() << ", MAX_SPEED = " << MAX_SPEED << ", vPct = " << vPct << endl;
			steer = (desired - (loc - prevLoc));
			steer.limit(MAX_STEER);
		steer *= ((vPct) + .1);
		//}
    } else {
		steer.set(0.f, 0.f, 0.f);
    }
    acc += steer;
}


void Squid::draw() {
	
	glPushMatrix();
	glTranslatef(loc.x, loc.y, loc.z);
	glRotatef(rot, 0, 0, 1);
	output.setColor(0xb8b8b8);
	
	output.beginShape();
	
	float cenX		= 0;
	float cenY		= 0;
	
	float pct = _breath / (MAX_INHALE_BREATH - MIN_INHALE_BREATH);
	float rad2 = (MAX_INHALE_BREATH - MIN_INHALE_BREATH) * pct;
	
	float bw = BODY_WIDTH * .5;
	float bh = BODY_HEIGHT * .5;
	
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
	
	
	
	ofSetColor(255, 255, 255);
	ofCircle(cp1a.x, cp1a.y, 2);
	ofCircle(cp1b.x, cp1b.y, 2);
	ofCircle(cp2a.x, cp2a.y, 2);
	ofCircle(cp2b.x, cp2b.y, 2);
	ofCircle(cp3a.x, cp3a.y, 2);
	ofCircle(cp3b.x, cp3b.y, 2);
	ofSetColor(0, 255, 0);
	ofCircle(cp4.x, cp4.y, 2);
	
	ofSetColor(255, 0, 0);
	ofCircle(ap1.x, ap1.y, 1);
	ofCircle(ap2.x, ap2.y, 1);
	ofCircle(ap3.x, ap3.y, 1);
	ofCircle(ap4.x, ap4.y, 1);
	
	glPopMatrix();
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
	if (loc.x > ofGetWidth()) loc.x = 0;
	if (loc.x < 0) loc.x = ofGetWidth();
	
	if (loc.y > ofGetHeight()) loc.y = 0;
	if (loc.y < 0) loc.y = ofGetHeight();
}


////////////////////////////////////////////////////////////////////





