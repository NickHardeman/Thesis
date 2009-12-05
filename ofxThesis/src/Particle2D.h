/*
 *  Particle.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PARTICLE_2D_
#define _PARTICLE_2D_

#include "ofMain.h"
#include "ofxVec2f.h"

class Particle2D {
public:
	ofxVec2f loc;
	ofxVec2f vel;
	ofxVec2f acc;
	ofxVec2f prevLoc;
	
	float MAX_SPEED;
	float MAX_STEER;
	float DAMPING;
	float radius;
	
	Particle2D();
	Particle2D(float $x, float $y);
	
	
	void addForce(ofxVec2f $loc);
	void addForce(float $x, float $y);
	void addForce(ofxVec2f $loc, float $minDist, float $scale);
	void addForce(float $x, float $y, float $minDist, float $scale);
	void steer(ofxVec2f target, bool slowdown);
	void steer(ofxVec2f $target, bool $slowdown, float $scale);
	void seek(float $x, float $y);
	void addDamping(float $damp);
	
	void addClockwiseForce(ofxVec2f $loc, float $minDist, float $scale );
	void addClockwiseForce(float $x, float $y, float $minDist, float $scale );
	
	void addCounterClockwiseForce(ofxVec2f $loc, float $minDist, float $scale );
	void addCounterClockwiseForce(float $x, float $y, float $minDist, float $scale );
	
	void spring(float $x, float $y, float $spring, float $minDist);
	
	void fixedSpring(ofxVec2f &p, float $spring, float $len, bool $add = false);
	void fixedSpring(ofxVec2f &p, float $spring, float $len, ofxVec2f &addVec);
	void fixedSpring(Particle2D &p, float $spring, float $len);
	void fixedSpring(float $x, float $y, float $spring, float $len, ofxVec2f &addVec);
	
	void bounceOffEdges(bool bDampedOnCollision = false);
	void checkExtendedEdges(float $buffer = 0.f);
	void elasticEdges( float $elastic, float $buffer );
	
private:
	void _setDefaults();
};

#endif