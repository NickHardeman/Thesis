/*
 *  MotionMeter.cpp
 *  openFrameworks
 *
 *  Created by Bruce Drummond on 12/5/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotionMeter.h"
#include "ofMain.h"

MotionMeter::MotionMeter(){
	w = 10;
	xpos = 10;
	ypos = 100;
	accumulatedMotion = 0;
}

void MotionMeter::update(){
	contract();
	//ofFill();
	//ofSetColor(255, 0, 0);
	ofRect(xpos, ypos, w, -accumulatedMotion);
}

void MotionMeter::draw(){
	ofFill();
	ofSetColor(255, 0, 0);
	ofRect(xpos, ypos, w, -accumulatedMotion);
	ofNoFill();
	ofRect(xpos, ypos, w, -ypos);
	ofLine(0, 50, 20, 50);
}

void MotionMeter::contract(){
	if (accumulatedMotion > 0) {
		accumulatedMotion -= 0.1;
	}
}

void MotionMeter::reset(){
	accumulatedMotion = 0;
}