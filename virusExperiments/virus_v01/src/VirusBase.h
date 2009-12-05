/*
 *  VirusBase.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 11/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIRUS_BASE_
#define _VIRUS_BASE_

#include "Organism.h"

class ParticleWTar : public Particle {
public:
	ofxVec3f tar;
	
	void setTarget(float $x, float $y, float $z) { tar.set($x, $y, $z); };
};

class VirusBase : public Organism {
public:
	void setup();
	void update();
	void draw();
	
	vector < ParticleWTar > basePts;
	
	void addBasePt(float $x, float $y, float $tarX, float $tarY);
	void addBaseForce(float $x, float $y, float $z, float $minDist, float $scale);
	
	void bounceOffEdges(ParticleWTar &pt, bool bDampedOnCollision);
	void elasticEdges( ParticleWTar &pt, float $elastic, float $buffer );
	
};

#endif
