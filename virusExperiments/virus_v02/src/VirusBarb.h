/*
 *  VirusBarb.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIRUS_BARB_
#define _VIRUS_BARB_

#include "Particle2D.h"

#define STATUS_ALIVE		1
#define STATUS_FIRE			2
#define STATUS_SEEKING		3
#define STATUS_DIE			4
#define STATUS_DYING		5
#define STATUS_REMOVE		6

class VirusBarb : public Particle2D {
public:
	ofxVec2f target;
	// used for rendering image //
	ofxVec2f dirVec;
	int status;
	
	// used to keep track of parent location index, so that when it is removed, 
	// we still know where it originated
	int bodyPtIndex;
	
	float FIRE_VEL;
	
	VirusBarb();
	
	void setBodyPtIndex( int $i );
	
	void setDirection(float $x, float $y);
	void setDirection( ofxVec2f $dir );
	
	void fire(ofxVec2f $vel);
	
	void update();
	void updateTarget(float $x, float $y);
	void checkRemove();
	
	void draw();
	
};

#endif