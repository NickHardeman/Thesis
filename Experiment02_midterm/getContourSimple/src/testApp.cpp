#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(255, 255, 255);
	
	targetPoint.x = 200;
	targetPoint.y = 100;
	
	contourPoint.x = 0;
	contourPoint.y = 0;
	
	xscale = (float)ofGetWidth() / (float)VIDEO_WIDTH;
	yscale = (float)ofGetHeight() / (float)VIDEO_HEIGHT;
	
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.setVerbose(true);
		vidGrabber.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
	#endif
	
    colorImg.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
	grayImage.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
	grayBg.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
	grayDiff.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
	
	grayDiffSmall.allocate(VIDEO_WIDTH * .25, VIDEO_HEIGHT * .25);
	
	VF.setupField(grayDiffSmall.width, grayDiffSmall.height, ofGetWidth(), ofGetHeight());
	
	for (int i = 0; i < 200; i++) {
		Boid b;
		boids.push_back( b );
	}
	
	
	bLearnBakground = true;
	threshold = 30;
	bDrawDiagnostic = false;
}

//--------------------------------------------------------------
void testApp::update(){
	bool bNewFrame = false;
	
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.grabFrame();
		bNewFrame = vidGrabber.isFrameNew();
	#else
		bNewFrame = false;
	#endif
	
	#ifdef _USE_LIVE_VIDEO
	if (bNewFrame){
			
		colorImg.setFromPixels(vidGrabber.getPixels(), VIDEO_WIDTH, VIDEO_HEIGHT);
			
		grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			//grayBg.scaleIntoMe(grayImage);
			bLearnBakground = false;
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		
		//grayDiff.absDiff(grayImage, videoCvPrevFrame);
		
		grayDiff.threshold(threshold);
		grayDiff.blur(2);
		grayDiffSmall.scaleIntoMe(grayDiff);
		grayDiffSmall.erode_3x3();
		//grayDiffSmall.erode_3x3();
		grayDiffSmall.blur(2);
		
		//scaledGrayBg.scaleIntoMe(grayDiff);
		contourFinder.findContours(grayDiff, 20 * 20, (VIDEO_WIDTH * VIDEO_WIDTH), 10, false);
		VF.setFromPixels(grayDiffSmall.getPixels(), false, .01f);
		
		for (int b = 0; b < boids.size(); b++) {
			float dx, dy, dist;
			float minDist = ofGetWidth() * 2;
			ofPoint tarPt;
			tarPt.set(-10, -1);
			
			for (int i = 0; i < contourFinder.nBlobs; i++){
				for (int j = 0; j < contourFinder.blobs[i].nPts; j++) {
					dx = (boids[b].loc.x / xscale) - contourFinder.blobs[i].pts[j].x;
					dy = (boids[b].loc.y / yscale) - contourFinder.blobs[i].pts[j].y;
					dist = sqrt( (dx * dx) + (dy * dy) );
					if (dist < minDist) {
						tarPt.x = contourFinder.blobs[i].pts[j].x;
						tarPt.y = contourFinder.blobs[i].pts[j].y;
						minDist = dist;
					}
				}
			}
			
			boids[b].setMouseLoc(mouseX, mouseY);
			
			boids[b].setTargetPoint(tarPt.x * xscale, tarPt.y * yscale);
			
			ofxVec2f frc;
			frc = VF.getForceFromPos(boids[b].loc.x, boids[b].loc.y);
			boids[b].addForce(frc * 150);
			
			boids[b].update();
			
		}
	}
	#else
	for (int b = 0; b < boids.size(); b++) {
		boids[b].setTargetPoint(mouseX, mouseY);
		boids[b].update();
	}
	#endif
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 255, 255);
	#ifdef _USE_LIVE_VIDEO
		colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());
	#endif
	
	ofSetColor(0, 0, 255);
	ofNoFill();
	ofSetLineWidth(.5);
	/*
	for (int i = 0; i < contourFinder.nBlobs; i++){
        //contourFinder.blobs[i].draw(0, 0);
		ofBeginShape();
		for (int j = 0; j < contourFinder.blobs[i].nPts; j++) {
			//cout << contourFinder.blobs[i].pts[j].x << ", " << contourFinder.blobs[i].pts[j].y << endl;
			ofVertex(contourFinder.blobs[i].pts[j].x * xscale, contourFinder.blobs[i].pts[j].y * yscale);
			//ofCircle(contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y, 2);
            
		}
		ofEndShape(true);
   }
	 */
	ofFill();
	ofSetColor(255, 255, 255);
	//grayDiffSmall.draw(0, 0, ofGetWidth(), ofGetHeight());
	#ifdef _USE_LIVE_VIDEO
	ofSetColor(150, 100, 255);
	VF.draw();
	#endif
	
	for (int b = 0; b < boids.size(); b++) {
		ofSetColor(255, 0, 0);
		ofCircle(boids[b].targetPoint.x, boids[b].targetPoint.y, 1);
		boids[b].draw();
	}
	
	
	ofSetColor(0, 0, 255);
	//ofCircle(targetPoint.x * xscale, targetPoint.y * yscale, 3);
	
	ofSetColor(0, 255, 0);
	//ofCircle(contourPoint.x * xscale, contourPoint.y * yscale, 3);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 's') {
		#ifdef _USE_LIVE_VIDEO
		vidGrabber.videoSettings();
		#endif
	} else if (key == ' ') {
		bLearnBakground = true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

