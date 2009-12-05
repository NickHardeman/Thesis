/*
 *  Squid.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SQUID_CREATURE
#define _SQUID_CREATURE

#include "ofxVectorGraphics.h"
#include "ofxVectorMath.h"
#define OF_ADDON_USING_OFXVECTORMATH

#include "ofxOsc.h"

//#define HOST "149.31.144.59"
#define HOST "149.31.142.106"
#define PORT 10000

class Squid {
public:
	static const int INHALING		= 1;
	static const int EXHALING		= 2;
	float MAX_SPEED;
	float MAX_STEER;
	
	ofxVec3f loc;
	ofxVec3f vel;
	ofxVec3f acc;
	
	// CONSTRUCTORS //
	Squid();
	
	void update();
	void steer(ofxVec3f target, bool slowdown);
	void addBreathForce();
	void seekMouse(float $x, float $y);
	
	void draw();
	
private:
	float INHALE_SPEED;
	float EXHALE_SPEED;
	
	float BODY_HEIGHT;
	float BODY_WIDTH;
	float MAX_INHALE_BREATH;
	float MIN_INHALE_BREATH;
	
	int _status;
	ofxVectorGraphics output;
	
	float _breath;
	float _pctBreath;
	
	void _setDefaults();
	float _getBreathSpeed();
	void _checkBreathVolume();
	
	void _checkEdges();
	
	ofxOscSender sender;
	
};

#endif
