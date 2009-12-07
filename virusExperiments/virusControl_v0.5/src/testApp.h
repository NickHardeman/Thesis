#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxControlPanel.h"

#include "particle.h"
#include "binner.h"

#include "VirusBase.h"

#include "MotionMeter.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file


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
		
		// create a variable of the type ofImage
		ofxControlPanel			panel;
	
		int						width, height;
	
		#ifdef _USE_LIVE_VIDEO
			ofVideoGrabber 		vidGrabber;
		#else
			ofVideoPlayer 		video;
		#endif
		
		//ofVideoPlayer			video;
		
		ofxCvColorImage			videoColorCvImage;
		ofxCvColorImage			backgroundVideo;
		ofxCvGrayscaleImage		videoGrayscaleCvImage;
		ofxCvGrayscaleImage		videoBgImage;
		ofxCvGrayscaleImage		videoDiffImage;
	
		//motion history
		ofxCvGrayscaleImage		videoPrevFrameImage;
		ofxCvGrayscaleImage		videoMotionDiff;
	
		ofxCvContourFinder		contourFinder;
		ofxCvContourFinder		motionContourFinder;
	
		//video control
		Boolean					videoIsPlaying;
	
		//particle vectors
		vector <particle> particles;
		vector <particle> contourParticles;
	
		Boolean					bDrawDiagnostic;
		Boolean					bDrawPanel;
		Boolean					bDrawContour;
		Boolean					bDrawCentroid;
		Boolean					bDrawVirus;
		Boolean					bDrawParticles;
	
		float					WALL_PADDING;
		VirusBase				virus;
	
		float					motionPct;
		float					vel;
	
		MotionMeter				meter;
	
		//the motion threshold to release the virus
		float					MOTION_THRESHOLD;
	
		float					centerX;
		float					centerY;
	
		float					particlesX;
		float					particlesY;
		
};

#endif
