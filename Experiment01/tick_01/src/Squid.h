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
#include "Organism.h"
#define OF_ADDON_USING_OFXVECTORMATH

class Squid : public Organism {
public:
	static const int INHALING		= 1;
	static const int EXHALING		= 2;
	
	// CONSTRUCTORS //
	Squid();
	
	void update();
	
	void addBreathForce();
	
	void draw();
	
protected:
	float INHALE_SPEED;
	float EXHALE_SPEED;
	
	float MAX_INHALE_BREATH;
	float MIN_INHALE_BREATH;
	
	int TENTACLE_LENGTH;
	float TENTACLE_SPACING;
	float TENTACLE_SPRING;
	
	int _status;
	ofxVectorGraphics output;
	
	float _breath;
	float _pctBreath;
	
	vector <ofxVec3f> tentacle;
	
	float _getBreathSpeed();
	void _checkBreathVolume();
	void _updateTentacles();
	void _dragSegment(ofxVec3f & $segment, ofxVec3f & $tarSeg, float $length);
	void _setDefaults();
	
	void _renderTentacles();
	
};

#endif
