#include "testApp.h"

bool pointInPolygon(float x, float y, vector<ofPoint>pts);
bool pointInPolygon(float x, float y, vector<ofPoint>pts){
	
	// code via Randolph Franklin...
	int i, j, c = 0;
	int nPts = pts.size();
	
	for (i = 0, j = nPts-1; i < nPts; j = i++) {
		if ((((pts[i].y <= y) && (y < pts[j].y)) ||
			 ((pts[j].y <= y) && (y < pts[i].y))) &&
			(x < (pts[j].x - pts[i].x) * (y - pts[i].y) / (pts[j].y - pts[i].y) + pts[i].x))
			c = !c;
	}
	return (bool) c;
}

//--------------------------------------------------------------
void testApp::setup(){
	
	//meter stuff
	MOTION_THRESHOLD = 50.0f;
	
	//ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	//virus stuff
	WALL_PADDING = 75.f;
	
	virus.setup(ofGetWidth() * .5, ofGetHeight() * .5);
	virus.WALL_PADDING = WALL_PADDING;
	virus.constructCircleBase(20.f, 50.f);
	
	//setup the particles
	for (int i = 0; i < 50; i++) {
		particle myParticle;
		//myParticle.setInitialCondition(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), 0, 0);
		myParticle.setInitialCondition(ofRandom(0, ofGetWidth()), ofGetHeight(), 0, 0);
		particles.push_back(myParticle);
	}

	// setup video grabber
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.setVerbose(false);
		vidGrabber.initGrabber(320,240);
	#else
		video.loadMovie("test_vid.mov");
		video.play();
		//video control
		videoIsPlaying = true;
	#endif
	
	// get the width and height, and allocate color and grayscale images:
	#ifdef _USE_LIVE_VIDEO
		width = vidGrabber.width;
		height = vidGrabber.height;9  
	#else
		width = video.width; 
		height = video.height;
	#endif
	
	videoColorCvImage.allocate(width, height);
	backgroundVideo.allocate(width, height);
	videoGrayscaleCvImage.allocate(width, height);
	videoBgImage.allocate(width, height);
	videoDiffImage.allocate(width, height);
	
	//motion history
	videoPrevFrameImage.allocate(width, height);
	videoMotionDiff.allocate(width, height);
	
	// set background color to be white: 
	ofBackground(255, 255, 255);
	
	
	panel.setup("cv settings", 720, 0, 300, 748);
	panel.addPanel("control", 1, false);
	
	panel.setWhichPanel("control");
	panel.setWhichColumn(0);
	panel.addToggle("learn background ", "B_LEARN_BG", true);
	panel.addSlider("threshold ", "THRESHOLD", 127, 0, 255, true);
	
	panel.addSlider("nDilations ", "N_DILATIONS", 2, 0, 20, true);
	panel.addSlider("nErosions ", "N_EROSION", 0, 0, 20, true);
	
	panel.addSlider("minBlobSize ", "MIN_BLOB_SIZE", 50, 0, width*height*0.2f, true);
	panel.addSlider("maxBlobSize ", "MAX_BLOB_SIZE", 20000, 0, width*height*0.5f, true);
	panel.addSlider("nBlobs to find ", "N_BLOBS_TO_FIND", 10, 0, 15, true);
	
	panel.addSlider("minMotionBlobSize ", "MIN_MOTION_BLOB_SIZE", 50, 0, width*height*0.2f, true);
	panel.addSlider("maxMotionBlobSize ", "MAX_MOTION_BLOB_SIZE", 20000, 0, width*height*0.5f, true);
	panel.addSlider("nMotionBlobs to find ", "N_MOTION_BLOBS_TO_FIND", 10, 0, 15, true);
	
	panel.addToggle("Draw contour", "B_DRAW_CONTOUR", true);
	panel.addToggle("Draw centroid", "B_DRAW_CENTROID", true);
	
	panel.loadSettings("cvSettings.xml");
	
	//ofSetCircleResolution(10);
	setupBins(ofGetWidth(), ofGetHeight(), 19, 14);
	
	bDrawDiagnostic = true;
	bDrawPanel = true;
	bDrawContour = true;
	bDrawCentroid = true;
	bDrawVirus = false;
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	//virus stuff
	//virus.addBaseForce(mouseX, mouseY, 100, -0.9f);
	//virus.setMouseLoc(mouseX, mouseY);
	virus.update();
	
	backgroundVideo = videoColorCvImage;
		
	panel.update();
	
	
	bool bLearnBg			= panel.getValueB("B_LEARN_BG");
	int threshold			= panel.getValueI("THRESHOLD");
	int nDilations			= panel.getValueI("N_DILATIONS");
	int nErosions			= panel.getValueI("N_EROSION");
	int maxOperations		= MAX(nDilations, nErosions);
	
	int minBlobSize			= panel.getValueI("MIN_BLOB_SIZE");
	int maxBlobSize			= panel.getValueI("MAX_BLOB_SIZE");
	int bBlobsTofind		= panel.getValueI("N_BLOBS_TO_FIND");
	
	int minMotionBlobSize	= panel.getValueI("MIN_MOTION_BLOB_SIZE");
	int maxMotionBlobSize	= panel.getValueI("MAX_MOTION_BLOB_SIZE");
	int bMotionBlobsTofind	= panel.getValueI("N_MOTION_BLOBS_TO_FIND");
	
	bDrawContour		= panel.getValueB("B_DRAW_CONTOUR");
	bDrawCentroid		= panel.getValueB("B_DRAW_CENTROID");
	
	bool bNewFrame = false;
	
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.grabFrame();
		bNewFrame = vidGrabber.isFrameNew();
	#else
		video.idleMovie();
		bNewFrame = video.isFrameNew();
	#endif
	
	//if (video.isFrameNew()){
	if (bNewFrame){
				
		#ifdef _USE_LIVE_VIDEO
			videoColorCvImage.setFromPixels(vidGrabber.getPixels(), width,height);
			videoColorCvImage.mirror(false, true);
		#else
			videoColorCvImage.setFromPixels(video.getPixels(), width,height);
		#endif
		
		//videoColorCvImage.setFromPixels(video.getPixels(), width, height);
		videoGrayscaleCvImage = videoColorCvImage;
		
		if (bLearnBg ){ 
			videoBgImage = videoGrayscaleCvImage;
			panel.setValueB("B_LEARN_BG", false);
		}
		
		if (ofGetElapsedTimef() < 2.0){
			videoBgImage = videoGrayscaleCvImage;
		}
		
		videoDiffImage.absDiff(videoGrayscaleCvImage, videoBgImage);
		videoDiffImage.threshold(threshold);
		
		/*for (int i = 0; i < maxOperations; i++){
			
			if (i < nErosions)	videoDiffImage.erode();
			if (i < nDilations)	videoDiffImage.dilate();
			
		}*/
		
		//motion history stuff
		videoMotionDiff.absDiff(videoGrayscaleCvImage, videoPrevFrameImage);
		
		//calculate the motion percentage
		motionPct = (float) videoMotionDiff.countNonZeroInRegion(0, 0, videoMotionDiff.width, videoMotionDiff.height)/(width * height);
		
		//find the contours
		motionContourFinder.findContours(videoMotionDiff, minMotionBlobSize, maxMotionBlobSize, bMotionBlobsTofind, false, true);
		
		//get the centroid
		centerX = centerY = 0.f;
		for (int i = 0; i < motionContourFinder.nBlobs; i++){
			centerX += motionContourFinder.blobs[i].centroid.x;
			centerY += motionContourFinder.blobs[i].centroid.y;
		}
		
		centerX /= motionContourFinder.nBlobs;
		centerY /= motionContourFinder.nBlobs;
		
		//----------------------------------------------------- virus hit
		if (bDrawVirus) {
			virus.setMouseLoc(mouseX, mouseY);
			for (int j = 0; j < contourParticles.size(); j++) {
				virus.addBaseForce(contourParticles[j].pos.x, contourParticles[j].pos.y, 40, -14.f);
			}
		}
		
		//if motion is greater than 0.2 accumulate it
		if (motionPct > 0.2f) {
			if (meter.accumulatedMotion > MOTION_THRESHOLD) {
				if (!bDrawVirus) {
					meter.reset();
					cout << "RELEASE VIRUS" << endl;
					float scaleX = (float) ofGetWidth() / (float) videoDiffImage.width;
					float scaleY = (float) ofGetHeight() / (float) videoDiffImage.height;
					int xpos = centerX;
					int ypos = centerY;
					virus.loc.x = xpos * scaleX + 200;
					virus.loc.y = ypos * scaleY;
					//virus.addBaseForce(xpos, ypos, 100, -0.9f);
					virus.setRandomLocations( .08, .4 );
					virus.setRandomVelocities( vel, 8.f, 4.f, .3, 1);
				}
				
				bDrawVirus = true;
			} else {
				meter.accumulatedMotion += motionPct * 10;
			}
		}
		
		videoPrevFrameImage = videoGrayscaleCvImage;
		
		contourFinder.findContours(	videoDiffImage, minBlobSize,maxBlobSize,bBlobsTofind,false, true);
		
		contourParticles.clear();
		
		if (contourFinder.nBlobs > 0) {
			float scaleX = (float) ofGetWidth() / (float) videoDiffImage.width;
			float scaleY = (float) ofGetHeight() / (float) videoDiffImage.height;
			for (int i = 0; i < contourFinder.blobs[0].nPts; i += 4) {
				particle p;
				p.setInitialCondition(contourFinder.blobs[0].pts[i].x*scaleX, contourFinder.blobs[0].pts[i].y*scaleY, 0, 0);
				p.radius = 5;
				contourParticles.push_back(p);
			}
		}
		
		
	}
	
	
	//----------------------------------------------------- particle stuff
	
	
	// do binning for both
	for (int i = 0; i < particles.size(); i++){
		computeBinPosition( particles[i].pos.x, particles[i].pos.y,  &(particles[i].bitFlagW), &(particles[i].bitFlagH));
	}
	
	for (int i = 0; i < contourParticles.size(); i++){
		computeBinPosition( contourParticles[i].pos.x, contourParticles[i].pos.y,  &(contourParticles[i].bitFlagW), &(contourParticles[i].bitFlagH));
	}
	
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	// particle v particle
	for (int i = 0; i < particles.size(); i++){
		unsigned int bitFlagH_pta = particles[i].bitFlagH;
		unsigned int bitFlagW_pta = particles[i].bitFlagW;
		for (int j = 0; j < i; j++){
			unsigned int bitFlagH_ptb = particles[j].bitFlagH;
			unsigned int bitFlagW_ptb = particles[j].bitFlagW;
			if ((bitFlagW_pta & bitFlagW_ptb) && (bitFlagH_pta & bitFlagH_ptb)){
				//particles[i].addRepulsionForce( particles[j],20,0.7);
				particles[i].addAttractionForce( particles[j],1000,0.03);
			}
        }
    }
	
	
	// do particle against hand: 
	for (int i = 0; i < particles.size(); i++){
		unsigned int bitFlagH_pta = particles[i].bitFlagH;
		unsigned int bitFlagW_pta = particles[i].bitFlagW;
		for (int j = 0; j < contourParticles.size(); j++){
			unsigned int bitFlagH_ptb = contourParticles[j].bitFlagH;
			unsigned int bitFlagW_ptb = contourParticles[j].bitFlagW;
			if ((bitFlagW_pta & bitFlagW_ptb) && (bitFlagH_pta & bitFlagH_ptb)){
				particles[i].addRepulsionForce(contourParticles[j].pos.x, contourParticles[j].pos.y, 30, 0.03f);
				particles[i].addAttractionForce(contourParticles[j].pos.x, contourParticles[j].pos.y, 50000, 0.5f);
			}
		}
	}
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].addAttractionForce( 500,500,1500,0.3);
		particles[i].addForce(0,0.04f);
		particles[i].addDampingForce();
		particles[i].bounceOffWalls();
		particles[i].update();
	}
	
	// alternative, use the image pixels !! 
	// got to be very careful about the edges :)
	
	unsigned char * pixels = videoDiffImage.getPixels();
	for (int i = 0; i < particles.size(); i++){
		int posScaledX = (int)(particles[i].pos.x *  (float)(videoDiffImage.width) / (float)ofGetWidth());
		int posScaledY = (int)(particles[i].pos.y *  (float)(videoDiffImage.height) / (float)ofGetHeight());
		if (posScaledX < 0) posScaledX = 0;
		if (posScaledX > videoDiffImage.width-1) posScaledX = videoDiffImage.width-1;
		if (posScaledY < 0) posScaledY = 0;
		if (posScaledY > videoDiffImage.height-1) posScaledY = videoDiffImage.height-1;
		unsigned char myPixels = pixels[posScaledY * videoDiffImage.width + posScaledX];
		if (myPixels > 0){
			//particles[i].setInitialCondition(ofRandom(0,ofGetWidth()), 0, 0,0);
			//particles[i].setInitialCondition(ofRandom(0,ofGetWidth()), ofGetHeight(), 0,0);
			if (i < motionContourFinder.nBlobs) {
				particlesX += motionContourFinder.blobs[i].centroid.x;
				particlesY += motionContourFinder.blobs[i].centroid.y;
				particles[i].setInitialCondition(particlesX, particlesY, 0,0);
			}
		}
	}
	
	meter.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	glPushMatrix();
	glScalef((float) ofGetWidth() / (float)videoMotionDiff.width, (float) ofGetHeight() / (float)videoMotionDiff.height, 1.0f);
	ofFill();
	ofSetColor(255, 255, 255);
	backgroundVideo.draw(0, 0);
	glPopMatrix();
		
	//virus stuff
	//ofFill();
	ofNoFill();
	ofSetColor(225, 225, 225);
	
	ofRect(0, 0, ofGetWidth(), WALL_PADDING);
	ofRect(0, 0, WALL_PADDING, ofGetHeight());
	ofRect(ofGetWidth() - WALL_PADDING, 0, WALL_PADDING, ofGetHeight());
	ofRect(0, ofGetHeight() - WALL_PADDING, ofGetWidth(), WALL_PADDING);
	
	ofSetColor(0, 0, 0);
	ofDrawBitmapString("Frame Rate: "+ofToString(ofGetFrameRate()), 20, 20);
	
	if (bDrawVirus) {
		virus.draw();
	}
	
	//ofNoFill();
	//ofCircle(mouseX, mouseY, 100);
	
	//particle stuff
	//if (!bDrawVirus) {
		ofFill();
		ofSetColor(0x00FF00);
		for (int i = 0; i < particles.size(); i++){
			particles[i].draw();
		}
	//}
	
	if (bDrawContour) {
		ofSetColor(0xff0000);
		for (int i = 0; i < contourParticles.size(); i++){
			contourParticles[i].draw();
		}
	
	
		if (contourFinder.nBlobs > 0){
			glPushMatrix();
			glScalef((float) ofGetWidth() / (float)videoDiffImage.width, (float) ofGetHeight() / (float)videoDiffImage.height, 1.0f);
			contourFinder.blobs[0].draw(0,0);
			glPopMatrix();
		}
	}
	
	if (bDrawDiagnostic == true){
	
		ofSetColor(255, 255, 255);
		videoGrayscaleCvImage.draw(20,20, 320,240);
		videoBgImage.draw(320+30, 20, 320, 240);
		videoDiffImage.draw(20,240+40, 320,240);
		//videoPrevFrameImage.draw(320+30, 240 + 40, 320, 240);
		videoMotionDiff.draw(320+30, 240+40, 320, 240);
	}
	
	//draw the panel
	if (bDrawPanel) {
		panel.draw();
	}
	
	//contourFinder.draw(20,240+40,640,480);
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofDrawBitmapString("blob id:" + ofToString(i), contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y);
	}
	
	meter.draw();
	
	//draw the circle
	if (bDrawCentroid) {
		if (motionContourFinder.nBlobs > 0){
			glPushMatrix();
			glScalef((float) ofGetWidth() / (float)videoMotionDiff.width, (float) ofGetHeight() / (float)videoMotionDiff.height, 1.0f);
			ofFill();
			ofSetColor(255, 0, 0);
			ofCircle(centerX, centerY, 10);
			glPopMatrix();
		}
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		//press space to pause and play the video
		case ' ':
			#ifdef _USE_LIVE_VIDEO
			#else
				if (videoIsPlaying) {
					//printf("stop the video");
					video.stop();
					videoIsPlaying = false;
				} else {
					//printf("play the video");
					video.play();
					videoIsPlaying = true;
				}
			#endif
			break;
		//draw diagnostic
		case 'd':
			bDrawDiagnostic = !bDrawDiagnostic;
			break;
		//draw panel
		case 'p':
			bDrawPanel = !bDrawPanel;
			break;
		//reset particles
		case 'v':
			for (int i = 0; i < particles.size(); i++){
				particles[i].setInitialCondition(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), 0,0);
			}
			break;
		//virus control
		case OF_KEY_UP:
			virus.addForce( 0, -220.f );
			break;
		case OF_KEY_RIGHT:
			virus.addForce(220.f, 0);
			break;
		case OF_KEY_DOWN:
			virus.addForce(0, 220.f);
			break;
		case OF_KEY_LEFT:
			virus.addForce(-220.f, 0);
			break;
		case 'f':
			virus.fire();
			break;
		case 'r':
			virus.reload();
			break;
		case 'z':
			virus.angVel -= .005;
			break;
		case 'x':
			virus.angVel += .005;
			break;
		default:
			break;
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
	panel.mouseDragged(x,y,button);
	
	/*particles.erase(particles.begin());
	particle pt;
	pt.setInitialCondition(x, y, 0, 0);
	particles.push_back(pt);
	 */
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}