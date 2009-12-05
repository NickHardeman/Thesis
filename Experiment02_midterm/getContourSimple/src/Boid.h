/*
 *  Boid.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 10/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _BOID_
#define _BOID_

#include "ofMain.h"
#include "ofxVec2f.h"

class Boid {
public:
	ofxVec2f acc, vel, loc, prevLoc;
	ofxVec2f targetPoint;
	ofxVec2f mouseLoc;
	
	float friction;
	float elastic;
	float spring;
	
	float MAX_VELOCITY;
	
	Boid();
	
	void update();
	void draw();
	void setTargetPoint(float $x, float $y);
	void setMouseLoc(float $x, float $y);
	void elasticBounds();
	void addForce(ofxVec2f $frc);
};

#endif