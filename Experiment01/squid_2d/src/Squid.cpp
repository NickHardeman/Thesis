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
	
	// open an outgoing connection to HOST:PORT
	sender.setup( HOST, PORT );
}


// PUBLIC METHODS //////////////////////////////////////////////////////////////////////
void Squid::update() {
	_checkBreathVolume();
	_breath += _getBreathSpeed();
	
	_pctBreath = _breath / (MAX_INHALE_BREATH - MIN_INHALE_BREATH);
	
	//vel.x = 15 * (1 - _pctBreath);
	
	//steer(ofxVec3f(800, 500, 0), true);
	
	//acc *= (10 * (1 - _pctBreath));
	//MAX_SPEED = (1 - _pctBreath) * 15;
	
	//addBreathForce();
	
	vel += acc;
	
	vel *= ((1 - _pctBreath) + .01) * 8;
	
	vel.limit(MAX_SPEED);
	
	loc.x += vel.x;
	loc.y += vel.y;
	
	acc.set(0, 0, 0);
	
	cout << loc.x << endl;
	
	_checkEdges();
	
	ofxOscMessage flow;
	flow.setAddress( "/FLOW" );
	flow.addFloatArg( _breath/100 );
	sender.sendMessage( flow );

	ofxOscMessage filter;
	filter.setAddress( "/FILTER" );
	filter.addFloatArg( loc.x/1024 );
	sender.sendMessage( filter );
	
	ofxOscMessage delay;
	delay.setAddress( "/DELAY" );
	delay.addFloatArg( loc.y/1024 );
	sender.sendMessage( delay );
}

void Squid::addBreathForce() {
	ofxVec3f dir(vel.x, vel.y, 0);
	dir.normalize();
	
	dir *= .1;
	
	acc += dir;
}

void Squid::seekMouse(float $x, float $y) {
	steer(ofxVec3f($x, $y), false);
}

void Squid::steer(ofxVec3f $target, bool $slowdown) {
    ofxVec3f steer;
    ofxVec3f desired = $target - loc;
    float d = desired.length();
    // If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0) {
		desired.normalize();
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if (($slowdown) && (d < 100.0f)) desired *= (MAX_SPEED * (d/100.0f)); // This damping is somewhat arbitrary
		else desired *= (MAX_SPEED);
		// Steering = Desired minus Velocity
		steer = desired - vel;
		steer.limit(MAX_STEER);
    } else {
		steer.set(0.f, 0.f, 0.f);
    }
    acc += steer;
	
}


void Squid::draw() {
	
	// for the rotation matrix, don't forget to add 90 since it is oriented upwards //
	
	ofxVec3f dir = vel;
	float rot = atan2(dir.y, dir.x);
	
	rot = 90 + (rot * RAD_TO_DEG);
	if (rot > 360) rot -= 360;
	
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
	
	MAX_SPEED = 10.f;
	MAX_STEER = .10f;
	
	loc.set(200.f, 200.f, 0.f);
	vel.set(1.f, 1.f, 0.f);
	acc.set(0.f, 0.f, 0.f);
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





