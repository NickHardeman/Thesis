/*
 *  VirusBodyPoint.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "VirusBodyPoint.h"

VirusBodyPoint::VirusBodyPoint() {
	orientation = 0.f;
}

void VirusBodyPoint::update() {
	addDamping( DAMPING );
	
	vel += acc;
	vel.limit(MAX_SPEED);
	loc += vel;
	
	acc.set(0, 0);
}

void VirusBodyPoint::draw() {
	
}