/*
 *  Tick.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _TICK_ORGANISM_
#define _TICK_ORGANISM_

#include "TickLeg.h"
#include "Organism.h"

class Tick : public Organism {
public:
	TickLeg rLeg;
	TickLeg lLeg;
	
	bool isRightLeg;
	
	Tick();
	void update();
	void updateLegs();
	void draw();
	void setTarget(float $x, float $y);
	
protected:
	
private:
	float LEG_FORCE;
	int MAX_LEG_TIMER;
	
	int _legTimer;
	
	void _setDefaults();
	ofxVec3f target;
};

#endif