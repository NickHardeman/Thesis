/*
 *  VirusBase.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "VirusBase.h"

void VirusBase::setup() {
	loc.set(200, 200, 0);
	BODY_HEIGHT = 30;
}

void VirusBase::addBasePt(float $x, float $y, float $tarX, float $tarY) {
	ParticleWTar pt;
	pt.loc.x = $x;
	pt.loc.y = $y;
	pt.setTarget($tarX, $tarY, 0.f);
	basePts.push_back( pt );
}

// x, y and z are global coords //
void VirusBase::addBaseForce(float $x, float $y, float $z, float $minDist, float $scale) {
	ofxVec3f dir($x, $y, $z);
	dir = dir - loc;
	for (int i = 0; i < basePts.size(); i++) {
		basePts[i].addForce(dir, $minDist, $scale);
	}
}

void VirusBase::update() {
	
	for (int i = 0; i < basePts.size(); i++) {
		basePts[i].spring(basePts[i].tar.x, basePts[i].tar.y, 0.f, .02f, 500.f);
	}
	
	for (int i = 0; i < basePts.size(); i++) {
		basePts[i].fixedSpring(0.f, 0.f, 0.f, 0.008f, 30.f, acc);
	}
	
	for (int i = 0; i < basePts.size(); i++) {
		basePts[i].addDamping( .009 );
		basePts[i].vel += basePts[i].acc;
		basePts[i].loc += basePts[i].vel;
		//basePts[i].elasticEdges(1.5, -50);
		basePts[i].acc.set(0.f, 0.f, 0.f);
		//bounceOffEdges( basePts[i], true );
		elasticEdges( basePts[i], .003, -25 );
	}
	
	addDamping(.0009);
	
	//elasticEdges( .001, -20 );
	
	vel += acc;
	loc += vel;
	
	acc.set(0, 0, 0);
}

void VirusBase::draw() {
	glPushMatrix();
	glTranslatef(loc.x, loc.y, 0);
	ofSetColor(150, 150, 150);
	ofCircle(0, 0, 5);
	
	for (int i = 0; i < basePts.size(); i++) {
		ofSetColor(0, 0, 0);
		ofCircle(basePts[i].loc.x, basePts[i].loc.y, 2);
		ofxVec2f diff;
		if (i == 0) {
			diff = basePts[basePts.size() - 1].loc - basePts[i + 1].loc;
		} else if (i == basePts.size() - 1) {
			diff = basePts[i - 1].loc - basePts[0].loc;
		} else {
			diff = basePts[i-1].loc - basePts[i + 1].loc;
		}
		ofxVec2f perp = diff.getPerpendicular();
		perp *= 7;
		ofSetColor(255, 0, 0);
		ofLine(basePts[i].loc.x, basePts[i].loc.y, basePts[i].loc.x + perp.x, basePts[i].loc.y + perp.y);
		
		perp.normalize();
		perp *= -9;
		
		//diff.normalize();
		//diff *= 5.f;
		
		ofxVec2f endLoc = perp.getPerpendicular();
		endLoc *= 3;
		
		perp += basePts[i].loc;
		
		ofNoFill();
		ofSetColor(100, 100, 100);
		ofTriangle(basePts[i].loc.x, basePts[i].loc.y, 
				   
				   perp.x + endLoc.x, perp.y + endLoc.y, perp.x - endLoc.x, perp.y - endLoc.y);
		
		//ofLine(basePts[i].loc.x, basePts[i].loc.y, basePts[i].loc.x + perp.x, basePts[i].loc.y + perp.y);
		//ofLine(perp.x + endLoc.x, perp.y + endLoc.y, perp.x - endLoc.x, perp.y - endLoc.y);
		ofFill();
		ofSetColor(50, 50, 50);
		ofCircle(perp.x, perp.y, 1);
		//ofLine(0, 0, basePts[i].loc.x, basePts[i].loc.y);
	}
	glPopMatrix();
}


//------------------------------------------------------------
void VirusBase::bounceOffEdges(ParticleWTar &pt, bool bDampedOnCollision) {
	// sometimes it makes sense to damped, when we hit
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (loc.x + pt.loc.x > maxx){
		pt.loc.x = maxx - loc.x; // move to the edge, (important!)
		pt.vel.x *= -1;
		bDidICollide = true;
	} else if (loc.x + pt.loc.x < minx){
		pt.loc.x = minx - loc.x; // move to the edge, (important!)
		pt.vel.x *= -1;
		bDidICollide = true;
	}
	
	if (loc.y + pt.loc.y > maxy){
		pt.loc.y = maxy - loc.y; // move to the edge, (important!)
		pt.vel.y *= -1;
		bDidICollide = true;
	} else if (loc.y + pt.loc.y < miny){
		pt.loc.y = miny - loc.y; // move to the edge, (important!)
		pt.vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		pt.vel *= 0.3;
	}
	
}

void VirusBase::elasticEdges( ParticleWTar &pt, float $elastic, float $buffer ) {
	if (loc.x + pt.loc.x > ofGetWidth() + $buffer ) pt.vel.x += -$elastic;
	if (loc.x + pt.loc.x < -$buffer)				pt.vel.x += $elastic;
	
	if (loc.y + pt.loc.y > ofGetHeight() + $buffer) pt.vel.y += -$elastic;
	if (loc.y + pt.loc.y < -$buffer)				pt.vel.y += $elastic;
}



