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

#include "Particle2D.h"

class Organism : public Particle2D {
public:
	
	Organism();
	
	float BODY_HEIGHT;
	float BODY_WIDTH;
	
	ofxVec2f direction;
	
	float rot;
	
	void update();
	
	void addClockwiseForce(Organism & org, float $minDist, float $scale );
	
	void addCounterClockwiseForce(Organism & org, float $minDist, float $scale );
	
	void draw();
	
protected:
	void _setDefaults();
	
};
#endif