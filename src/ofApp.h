#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

	public:

		// OpenFramework Video Variables
			ofVideoGrabber cam;             
			ofxCvColorImage colorImg;        

		// OpenCV Variables
			ofxCvGrayscaleImage grayImg;      
			ofxCvGrayscaleImage blurredImg;
			ofxCvGrayscaleImage cannyEdge;
			ofxCvGrayscaleImage houghTransform;
			ofxCvColorImage stackedEdgesOnGray;


		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
