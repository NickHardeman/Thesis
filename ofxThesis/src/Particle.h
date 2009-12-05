/*
 *  Particle.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PARTICLE_
#define _PARTICLE_

#include "ofMain.h"
#include "ofxVectorMath.h"

class Particle {
public:
	ofxVec3f loc;
	ofxVec3f vel;
	ofxVec3f acc;
	ofxVec3f prevLoc;
	
	float MAX_SPEED;
	float MAX_STEER;
	float DAMPING;
	float radius;
	
	Particle();
	Particle(float $x, float $y, float $z);
	
	void addForce(ofxVec3f $loc);
	void addForce(float $x, float $y);
	void addForce(ofxVec3f $loc, float $minDist, float $scale);
	void addForce(float $x, float $y, float $z, float $minDist, float $scale);
	void steer(ofxVec3f target, bool slowdown);
	void steer(ofxVec3f $target, bool $slowdown, float $scale);
	void seek(float $x, float $y, float $z = 0.f);
	void addDamping(float $damp);
	
	void addClockwiseForce(ofxVec3f $loc, float $minDist, float $scale );
	void addClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale );
	
	void addCounterClockwiseForce(ofxVec3f $loc, float $minDist, float $scale );
	void addCounterClockwiseForce(float $x, float $y, float $z, float $minDist, float $scale );
	
	void spring(float $x, float $y, float $z, float $spring, float $minDist);
	
	void fixedSpring(ofxVec3f &p, float $spring, float $len, bool $add = false);
	void fixedSpring(ofxVec3f &p, float $spring, float $len, ofxVec3f &addVec);
	void fixedSpring(Particle &p, float $spring, float $len);
	void fixedSpring(float $x, float $y, float $z, float $spring, float $len, ofxVec3f &addVec);
	
	void bounceOffEdges(bool bDampedOnCollision = false);
	void checkExtendedEdges(float $buffer = 0.f);
	void elasticEdges( float $elastic, float $buffer );
	
private:
	void _setDefaults();
};

#endif