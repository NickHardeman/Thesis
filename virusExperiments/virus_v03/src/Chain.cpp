/*
 *  Chain.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/5/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Chain.h"


void Chain::build( int $numSegments, float $totalLength) {
	segmentLength = (float)$totalLength / (float)$numSegments;
	for (int i = 0; i < $numSegments; i++) {
		ofxVec2f seg;
		seg.x = ofRandom(300, 500);
		seg.y = ofRandom(300, 500);
		segments.push_back( seg );
	}
	cout << "segment built: " << segments.size() << endl;
}

void Chain::setTargetLoc(float $x, float $y) {
	target.set($x, $y);
}

void Chain::setBaseLoc(float $x, float $y) {
	baseLoc.set($x, $y);
}



ofxVec2f Chain::getFirstSegmentLoc() {
	return segments[0];
}

void Chain::update() {
	//cout << "target1: " << target1->x << target1->y << endl;
	//if (target1 != NULL) {
		//drag(segments[0], target1->x, target1->y);
	drag(segments[0], target.x, target.y);
		//cout << "target1 " << target1->x << target1->y << endl;
	//cout << "Chain::update x, y " << $x << ", " << $y << endl;
		
		for (int i = 1; i < segments.size(); i++) {
			//segments[i].x = target1->x;
			//segments[i].y = target1->y;
			drag(segments[i], segments[i-1].x, segments[i-1].y);
		}
	//}
	drag(segments[segments.size()-1], baseLoc.x, baseLoc.y);
	for (int i = segments.size() - 2; i >= 0; i--) {
		//segments[i].x = target1->x;
		//segments[i].y = target1->y;
		drag(segments[i], segments[i+1].x, segments[i+1].y);
	}
	
}

void Chain::drag(ofxVec2f& $segment, float $x, float $y) {
	//$segment.x = $x;
	//$segment.y = $y;
	ofxVec2f diff = ofxVec2f ($x, $y) - $segment;
	diff.normalize();
	diff *= segmentLength;
	//diff.x += $x;
	//diff.y += $y;
	$segment.x = $x - diff.x;
	$segment.y = $y - diff.y;
}

void Chain::draw() {
	//if (target1 != NULL || target2 != NULL) {
		ofFill();
		ofSetColor(0, 0, 0);
		for (int i = 0; i < segments.size(); i++) {
			if (i < segments.size() - 2) {
				ofCircle(segments[i].x, segments[i].y, 1);
			} else {
				ofCircle(segments[i].x, segments[i].y, 5);
			}
		}
	//}
}