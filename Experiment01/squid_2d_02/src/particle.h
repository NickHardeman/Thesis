#ifndef _PARTICLE_
#define _PARTICLE_

#include "ofMain.h"
#define OF_ADDON_USING_OFXVECTORMATH
#include "ofxVectorMath.h"

class particle {
public:
	vector <ofxVec3f> locs;
	ofxVec3f loc;
	ofxVec3f vel;
	ofxVec3f acc;
	
	int _tailLength;
	
	float rad;
	float _b;
	float _r;
	
	particle();
	particle(float $x, float $y, float $z);
	particle(float $x, float $y, float $z, float radius);
	particle(ofxVec3f location, ofxVec3f velocity, float radius);
	
	void addForce(ofxVec3f frc);
	void addForce(float x, float y);
	ofxVec3f gravitate(ofxVec3f location, float G);
	ofxVec3f spring(ofxVec3f location, float scale);
	ofxVec3f repel(ofxVec3f location, float minDist, float scale);
	ofxVec3f addClockwiseForce(ofxVec3f location, float minDist, float scale);
	
	void setTailLength(int $tail);
	void renderTail();
	void setLocs(float $x, float $y, float $z);
	void setPositions();
	
	void screenWrap();
	void getEdges();
	void update();
	void draw();
	
	
};

#endif