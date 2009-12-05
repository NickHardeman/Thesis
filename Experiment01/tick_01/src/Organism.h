/*
 *  Organism.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ORGANISM_BASE_
#define _ORGANISM_BASE_

#include "ofxVectorGraphics.h"
#include "ofxVectorMath.h"
#define OF_ADDON_USING_OFXVECTORMATH

class Organism {
public:
	
	Organism();
	
	float MAX_SPEED;
	float MAX_STEER;
	
	float BODY_HEIGHT;
	float BODY_WIDTH;
	
	ofxVec3f loc;
	ofxVec3f vel;
	ofxVec3f acc;
	ofxVec3f prevLoc;
	ofxVec3f direction;
	
	float rot;
	
	
	void update();
	void addForce(ofxVec3f $loc, float $minDist, float $scale);
	void addForce(float $x, float $y, float $z, float $minDist, float $scale);
	void steer(ofxVec3f target, bool slowdown);
	void steer(ofxVec3f $target, bool $slowdown, float $scale);
	void seek(float $x, float $y, float $z = 0.f);
	void addDamping(float $damp);
	
	void addClockwiseForce(Organism & org, float $minDist, float $scale );
	void addClockwiseForce(ofxVec3f $loc, float $minDist, float $scale );
	void addClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale );
	
	void addCounterClockwiseForce(Organism & org, float $minDist, float $scale );
	void addCounterClockwiseForce(ofxVec3f $loc, float $minDist, float $scale );
	void addCounterClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale );
	
	void draw();
	
protected:
	void _checkEdges();
	void _setDefaults();
	
};
#endif