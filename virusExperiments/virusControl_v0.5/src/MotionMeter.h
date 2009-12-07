/*
 *  MotionMeter.h
 *  openFrameworks
 *
 *  Created by Bruce Drummond on 12/5/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MOTION_METER
#define _MOTION_METER

class MotionMeter {
	public:
	
		float w;
		float xpos;
		float ypos;
		float accumulatedMotion;
		
		MotionMeter();
		
		void update();
		void draw();
		void contract();
		void reset();
	
};

#endif