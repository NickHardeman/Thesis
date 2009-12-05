#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVec2f.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	void		addMouseForce();
	
	ofxVec2f	pos, vel, frc;
	float		orientation, av, torque;
	
	float		mass, I, OneOverMass, OneOverMomentOfInertia;
	
	float		recW, recH;
	
	vector < ofxVec2f > vertices;
	vector < float >	angles;
	
	bool		bAddGravity, bAddMouseFrc, bWorldSpring;
	float		minMouseDist;
	float		damping;
	
	bool bIsHitting, bIsEmbedded;
	float radius;
	
	ofxVec2f wallStartPos, wallNormal, mouseLoc, mouseVel;

};

#endif
