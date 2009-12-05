/*
 *  TickLeg.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 9/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TICK_LEG_
#define _TICK_LEG_

#include "ofxVectorMath.h"
#define OF_ADDON_USING_OFXVECTORMATH

class TickLeg {
public:
	float MAX_LEG_LENGTH;
	float MIN_LEG_LENGTH;
	
	float LEG_ANGLE;
	
	bool legExtending;
	ofxVec3f axis;
	ofxVec3f loc;
	ofxVec3f tarLoc;
	ofxVec3f vel;
	
	float xeno;
	
	TickLeg();
	
	void setLegAngle(float $angle);
	void setLegLengths(float $minLength, float $maxLength);
	void update();
	void setTarLoc(bool $max, float $xeno, bool $extending);
	bool checkExtended();
	ofxVec3f getLegAcc(float $scale);
	
protected:
	
private:
	void _setDefaults();
	
};
#endif