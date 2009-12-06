/*
 *  VirusBodyPoint.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIRUS_BODY_POINT_
#define _VIRUS_BODY_POINT_

#include "Particle2D.h"

class VirusBodyPoint : public Particle2D {
public:
	float orientation;
	ofxVec2f originPt;
	
	VirusBodyPoint();
	
	void update();
	void draw();
};

#endif