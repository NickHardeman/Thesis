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
#include "VirusBarb.h"
#include "VirusBodyPoint.h"

class VirusBase : public Organism {
public:
	// loc is used as Center of Mass, points and motion are relative to that, i.e. rotates around loc //
	
	// The oneOver vars are used for multiplication instead of division, since it is faster //
	// Moment of Intertia is how hard the object is to move //
	float mass, oneOverMass, momentOfInertia, oneOverMomentOfInertia;
	// Torque is the force added to angVel, from a point around Center of Mass //
	float torque;
	// angVel is the rate of rotation in radians //
	float angVel;
	// The rotation of the object, in Radians, similar to location or position //
	float orientation;
	
	float ANGULAR_DAMPING;
	
	vector < VirusBarb > barbs;
	vector < VirusBodyPoint > bodyPts;
	
	float WALL_PADDING;
	
	float mouseX, mouseY;
	
	// used to fire virus like a gun //
	int barbIndex;
	
	bool bFireBarb;
	
	// Body attributes and calculations //
	void calculateMOIRectangle(float $w, float $h);
	void calculateMOICircle(float $r);
	void calculateMOIRigidBody();
	
	void setMass( float $m );
	
	
	void setup(float $x, float $y);
	void update();
	void draw();
	
	void fire();
	void reload();
	
	void setMouseLoc( float $x, float $y);
	
	void addBasePt(float $x, float $y, float $tarX, float $tarY);
	void addBaseForce(float $x, float $y, float $minDist, float $scale);
	void addForceBodyPoint(Particle2D& bp, ofxVec2f $dir, float $scale);
	void addForceBodyPoint(VirusBodyPoint& bp, ofxVec2f $dir, float $minDist, float $scale);
	
	void bounceOffEdges(Particle2D &pt, bool bDampedOnCollision);
	void elasticEdges( Particle2D &pt, float $elastic, float $buffer, float $friction );
	
	// shape configuration constructors //
	void constructCircleBase(float $numPoints, float $r);
	void constructCrossBase(float $w, float $h);
	
};

#endif
