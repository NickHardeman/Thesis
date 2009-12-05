/*
 *  Boid.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 10/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Boid.h"

Boid::Boid() {
	loc.set(ofRandom(0, ofGetWidth()), ofRandom(0, 100));
	vel.set(0, 0);
	acc.set(0, 0);
	prevLoc = loc;
	targetPoint.set(-1, -1);
	
	friction = ofRandom(0.3f, 0.7f);
	elastic = ofRandom(0.3f, 0.7f);
	spring = ofRandom(.003, .02);
	
	MAX_VELOCITY = 40.f;
	
}

void Boid::setTargetPoint(float $x, float $y) {
	targetPoint.set($x, $y);
}

void Boid::setMouseLoc(float $x, float $y) {
	mouseLoc.set($x, $y);
}

void Boid::update() {
	prevLoc = loc;
	friction = ofRandom(0.3f, 0.7f);
	//elastic = ofRandom(0.3f, 0.7f);
	spring = ofRandom(.03, .2);
	
	// switch these to pursue mouse, naming is bad cuz I am lazy //
	//ofxVec2f mouseDiff = mouseLoc - loc;
	if (targetPoint.x > -1) {
		ofxVec2f mouseDiff = targetPoint - loc;
		float dist = mouseDiff.length();
		
		if (dist < 300) {
			float pct = 1 - ( (dist / 300.f) * .75);
			mouseDiff *= pct;
			acc += ( mouseDiff ) * spring * friction;
		}
	}
	
	vel += acc;
	vel = vel - ((vel - acc) * .05f);
	
	vel.limit( MAX_VELOCITY );
	
	loc += vel;
	elasticBounds();
	
	acc.set(0, 0);
}

void Boid::addForce(ofxVec2f $frc) {
	acc += $frc;
}

void Boid::draw() {
	ofFill();
	ofSetColor(0, 0, 0);
	
	ofxVec2f locDiff = loc - prevLoc;
	float dist = locDiff.length();
	float MAX_STROKE_WIDTH = 6;
	float MIN_STROKE_WIDTH = .5;
	
	float strokeWidth;
	if (dist > 0)
		strokeWidth = (1 - (dist / MAX_VELOCITY)) * (MAX_STROKE_WIDTH - MIN_STROKE_WIDTH) + MIN_STROKE_WIDTH;
	else
		strokeWidth = MAX_STROKE_WIDTH;
	
	ofSetLineWidth(strokeWidth);
	if (dist > 0) {
		if (dist < strokeWidth) {
			locDiff.normalize();
			locDiff *= strokeWidth;
			ofLine(loc.x, loc.y, loc.x + locDiff.x, loc.y + locDiff.y);
		} else {
			ofLine(loc.x, loc.y, prevLoc.x, prevLoc.y);
		}
	} else {
		ofLine(loc.x, loc.y, loc.x, loc.y + MAX_STROKE_WIDTH);
	}
	//cout << dist << " - " << strokeWidth << endl;
}

void Boid::elasticBounds() {
	if(loc.x > ofGetWidth() - 50) {
		//loc.x = ofGetWidth();
		vel.x += -elastic;
	}
	if (loc.x < 50) {
		//loc.x = 0;
		vel.x += elastic;
	}
	if (loc.y < 50) {
		//loc.y = 0;
		vel.y += elastic;
	}
	if (loc.y > ofGetHeight() - 50) {
		//loc.y = ofGetHeight();
		vel.y += -elastic;
	}
}



