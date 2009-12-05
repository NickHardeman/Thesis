#include "particle.h"
#include "ofMain.h"
#define OF_ADDON_USING_OFXVECTORMATH
#include "ofxVectorMath.h"

// Constructors ///////////////////////

particle::particle() {
	loc.set(0, 0, 0);
	vel.set(0, 0, 0);
	acc.set(0, 0, 0);
	rad = 2;
	getEdges();
	setTailLength(5);
}

particle::particle(float $x, float $y, float $z) {
	setLocs($x, $y, $z);
	loc.set($x, $y, $z);
	vel.set(0, 0, 0);
	acc.set(0, 0, 0);
	rad = 2;
	getEdges();
	setTailLength(5);
}

particle::particle(float $x, float $y, float $z, float radius) {
	setLocs($x, $y, $z);
	loc.set($x, $y, $z);
	vel.set(0, 0, 0);
	acc.set(0, 0, 0);
	rad = radius;
	getEdges();
	setTailLength(5);
}

particle::particle(ofxVec3f location, ofxVec3f velocity, float radius) {
	setLocs(location.x, location.y, location.z);
	loc = location;
	vel = velocity;
	acc.set(0, 0, 0);
	rad = radius;
	getEdges();
	setTailLength(5);
}

void particle::setLocs(float $x, float $y, float $z) {
	for (int i = 0; i < _tailLength; i++) {
		locs.push_back(ofxVec3f($x, $y, $z) );
	}
}
//////////////////////////////////////////////////

void particle::getEdges() {
	_r = (float)ofGetWidth();
	_b = (float)ofGetHeight();
}

void particle::update() {
	screenWrap();
	vel += acc;
	//vel.limit(4);
	
	setPositions();
	loc += vel;
	
	
	acc.set(0, 0, 0);
}

void particle::setPositions() {
	for( int i = _tailLength - 1; i > 0; i-- ){
		locs[i].set( locs[i - 1] );
    }
	locs[0] = loc;
}

// Courtesy Robert Hodgin / flight404.com //
void particle::renderTail() {
	float xp, yp, zp;
	float xOff, yOff, zOff;
	
	float radius = 20.f;
	
	glBegin(GL_QUAD_STRIP);
	
	for (int i = _tailLength - 1; i > 0; i--) {
		float per     = 1.0 - (float)i/(float)(_tailLength - 1);
		xp            = locs[i].x;
		yp            = locs[i].y;
		zp            = locs[i].z;
		
		if ( i < _tailLength - 2 ) {
			
			
			
			ofxVec3f v1 = locs[i] - locs[i + 1];
			v1.normalize();
			
			ofxVec3f ya = ofxVec3f(0, 1, 0);
			
			ofxVec3f v2 = ya.cross( v1 );
			ofxVec3f v3 = v1.cross( v2 ).normalize();
			
			xOff        = v3.x * radius * per;
			yOff        = v3.y * radius * per;
			zOff        = v3.z * radius * per;
			
			glColor4f( per, per*.25, 1.0 - per, per * .5);
			
			glVertex3f( xp - xOff, yp - yOff, zp - zOff );
			glVertex3f( xp + xOff, yp + yOff, zp + zOff );
			
			
			
			/*
			ofxVec3f perp0 = locs[i] - locs[i + 1];
			//perp0.normalize();
			
			ofxVec3f perp1 = perp0.crossed( ofxVec3f( 0, 1, 0 ) );
			perp1.normalize();
			ofxVec3f perp2 = perp0.crossed( perp1 );
			perp2.normalize();
			perp1 = perp0.crossed( perp2 );
			perp1.normalize();
			
			xOff        = perp1.x * radius * per;
			yOff        = perp1.y * radius * per;
			zOff        = perp1.z * radius * per;
			
			glColor4f( per, per*.25, 1.0 - per, per * .5);
			glVertex3f( xp - xOff, yp - yOff, zp - zOff );
			glVertex3f( xp + xOff, yp + yOff, zp + zOff );
			*/
		}
	}
	glEnd();
}

void particle::draw() {
	ofFill();
	
	renderTail();
	ofNoFill();
	ofSetColor(0, 0, 0);
	//ofCircle(loc.x, loc.y, rad);
	glBegin(GL_POLYGON);
		glVertex3f( loc.x - 5, loc.y - 5, loc.z );
		glVertex3f( loc.x + 5, loc.y - 5, loc.z );
		glVertex3f( loc.x - 5, loc.y + 5, loc.z );
		glVertex3f( loc.x + 5, loc.y + 5, loc.z );
	glEnd();
	
}

void particle::addForce(ofxVec3f frc) {
	acc += frc;
}

void particle::addForce(float x, float y) {

}

ofxVec3f particle::gravitate(ofxVec3f location, float G) {
	ofxVec3f dir = location - loc;
	float d = dir.length();
	if (d < 10) d = 10;
	if (d > 75) d = 75;
	dir.normalize();
	float force = (G * 9) / (d * d);
	dir *= force;
	return dir;
}

ofxVec3f particle::spring(ofxVec3f location, float scale) {
	ofxVec3f dir = location - loc;
	float d = dir.length();
	dir.x = (location.x - loc.x) * scale;
	dir.y = (location.y - loc.y) * scale;
	return dir;
}


ofxVec3f particle::repel(ofxVec3f location, float minDist, float scale) {
	ofxVec3f dir = location - loc;
	float d = dir.length();
	float pct = 1 - (d / minDist);
	dir.normalize();
	dir.x = dir.x * pct * scale;
	dir.y = dir.y * pct * scale;
	dir *= -1;
	return dir;
}

ofxVec3f particle::addClockwiseForce(ofxVec3f location, float minDist, float scale) {
	ofxVec3f dir = loc - location;
	float d = dir.length();
	float pct = 1 - (d / minDist);
	
	dir.normalize();
	dir.x = -1 * dir.y * pct * scale;
	dir.y =  dir.x * pct * scale;
	
	return dir;
}


void particle::screenWrap() {
	if (loc.x > _r + rad) {
		loc.x = -rad;
	} else if (loc.x < -rad) {
		loc.x = _r + rad;
	}
	
	if (loc.y > _b + rad) {
		loc.y = -rad;
	} else if (loc.y < -rad) {
		loc.y = _b + rad;
	}
	
	setLocs(loc.x, loc.y, loc.z);
		
}



// SETTERS ///////////////

void particle::setTailLength(int $tail) {
	_tailLength = $tail;
}




