/*
 *  TickLeg.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TickLeg.h"

TickLeg::TickLeg() {
	_setDefaults();
}

// IN DEGREES //
void TickLeg::setLegAngle(float $angle) {
	LEG_ANGLE = $angle;
	
	float angle = LEG_ANGLE * DEG_TO_RAD;
	float xLoc = cos(angle) * MIN_LEG_LENGTH;
	float yLoc = sin(angle) * MIN_LEG_LENGTH;
	
	axis.set(xLoc, yLoc, 0);
	loc.set(axis.x, axis.y, 0);
	
	tarLoc = axis.normalized();
	tarLoc *= MIN_LEG_LENGTH;
}

void TickLeg::setLegLengths(float $minLength, float $maxLength) {
	MIN_LEG_LENGTH = $minLength;
	MAX_LEG_LENGTH = $maxLength;
}

void TickLeg::update() {
	loc = xeno * (tarLoc) + (1 - xeno) * loc;
}

void TickLeg::setTarLoc(bool $max, float $xeno, bool $extending) {
	tarLoc = axis.normalized();
	if ($max)
		tarLoc *= MAX_LEG_LENGTH;
	else
		tarLoc *= MIN_LEG_LENGTH;
	xeno = $xeno;
	legExtending = $extending;
}

ofxVec3f TickLeg::getLegAcc(float $scale) {
	ofxVec3f dir;
	if (legExtending) {
		dir = tarLoc - (loc);
		float d = dir.length();
		
		float pct = d / MAX_LEG_LENGTH;
		pct = pct < 0 ? 0 : pct;
		pct = pct > 1 ? 1 : pct;
		dir.normalize();
		dir *= (1 - pct) * $scale;
		
		//cout << "dir.length = " << dir.length() << endl;
		
	} else {
		dir.set(0.f, 0.f, 0.f);
	}
	//cout << "dir.length = " << dir.length() << endl;
	return dir;
}

bool TickLeg::checkExtended() {
	if (legExtending) {
		ofxVec3f dist = tarLoc - loc;
		if (dist.length() < .2) {
			return true;
		}
	}
	return false;
}

void TickLeg::_setDefaults() {
	MAX_LEG_LENGTH = 60.f;
	MIN_LEG_LENGTH = 10.f;
	LEG_ANGLE = 150.f;
	
	setLegAngle(LEG_ANGLE);
	
	loc.set(axis.x, axis.y, 0);
	legExtending = false;
	tarLoc = axis.normalized();
	tarLoc *= MIN_LEG_LENGTH;
	
	xeno = .2;
}
