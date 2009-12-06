/*
 *  Chain.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/5/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CHAIN_
#define _CHAIN_

#include "ofxVec2f.h"
#include "ofMain.h"

class Chain {
public:
	
	vector < ofxVec2f > segments;
	ofxVec2f target;
	ofxVec2f baseLoc;
	
	ofxVec2f* tarPointer;
	
	float segmentLength;
	
	void build( int $numSegments, float $totalLength);
	void drag(ofxVec2f& $segment, float $x, float $y);
	
	void setTargetLoc(float $x, float $y);
	void setBaseLoc(float $x, float $y);
	
	ofxVec2f getFirstSegmentLoc();
	
	void update();
	void draw();
};

#endif