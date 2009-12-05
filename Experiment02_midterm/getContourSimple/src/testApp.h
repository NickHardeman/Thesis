#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "Boid.h"
#include "vectorField.h"
#include "ofxVec2f.h"

//#define _USE_LIVE_VIDEO

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
	
	int 				threshold;
	bool				bLearnBakground;
	
	int static const VIDEO_WIDTH = 320;
	int static const VIDEO_HEIGHT = 240;
	
	bool bDrawDiagnostic;
	
	#ifdef _USE_LIVE_VIDEO
		ofVideoGrabber 		vidGrabber;
	#endif
	
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage		scaledGrayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvGrayscaleImage		grayDiffSmall;
	
	ofxCvContourFinder		contourFinder;
	
	ofPoint targetPoint;
	ofPoint contourPoint;
	
	float xscale, yscale;
	
	vectorField VF;
	vector <Boid> boids;
};

#endif
