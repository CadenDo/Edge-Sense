#include "ofApp.h"
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(10); 
    // Setup and allocating memory to respective variables related to screen size
        cam.setup(1280, 720);
        colorImg.allocate(1280, 720);
        grayImg.allocate(1280, 720);
        blurredImg.allocate(1280, 720);
        cannyEdge.allocate(1280, 720);
        houghTransform.allocate(1280, 720);
        stackedEdgesOnGray.allocate(1280, 720);

}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update(); // Allows camera to store contiously store images, without grey image displayed                   

    if (cam.isFrameNew()) { 
        colorImg.setFromPixels(cam.getPixels()); // copies matrix of pixels to variable, 
        grayImg = colorImg;

      
        // Guassian Blur
            Mat grayMat;
            Mat blurMat;
            Size gaussKernal = Size(15, 15);

        // Blending
            Mat gray3ch;
            Mat canny3ch; 
            Mat blended;

        // Canny Edge
            Mat detectedEdges_x;
            Mat detectedEdges_y;
            Mat detectedEdges_output;

         // Hough Line Transformation
            vector<Vec2f> lines;
            Mat detectedLines_output;


        // Guassian Blur 
            grayMat = cvarrToMat(grayImg.getCvImage());  // converting ofxCvGrayscaleImage to dynamic OpenCV matrix
            GaussianBlur(grayMat, blurMat, gaussKernal, 5, 5);  // with 1:3 Canny Edge ratio, go to high = too dark, too low = very staticy
                // Could use grayImg.blurGaussian(Kernal Size), but doesn't give as much control over the blur
            blurredImg.setFromPixels(blurMat.data, blurMat.cols, blurMat.rows); // converts pixels (and info) to an array of values that can be drawn

        // Canny Edge

            // Determining dx and dy for Canny  
            Sobel(blurMat, detectedEdges_x, CV_16SC1, 1, 0, 5); // CV_16SC1 is a 16 bit signed matrix (referred to as channel), C3 = RGB channel (3 channel)
            Sobel(blurMat, detectedEdges_y, CV_16SC1, 0, 1, 5);

            // Identifying edges in image
            Canny(detectedEdges_x, detectedEdges_y, detectedEdges_output, 50, 150, false);

            // Creating image based on edges in image
            cannyEdge.setFromPixels(detectedEdges_output.data, detectedEdges_output.cols, detectedEdges_output.rows);


        // Hough Transform (focuses on lines drawn by lines from canny) 
            detectedLines_output = detectedEdges_output.clone();
            double theta = CV_PI / 180;
            HoughLines(detectedEdges_output, lines, 1, theta, 110);

            // Spherical to Rectangular Conversion 
            for (size_t i = 0; i < lines.size(); i++) {
                float rho = lines[i][0];
                float theta = lines[i][1];
                double a = cos(theta);
                double b = sin(theta);
                double x0 = a * rho;
                double y0 = b * rho;
                Point pt1, pt2;

                // Extend the lines for drawing
                pt1.x = cvRound(x0 + 1000 * (-b));
                pt1.y = cvRound(y0 + 1000 * (a));
                pt2.x = cvRound(x0 - 1000 * (-b));
                pt2.y = cvRound(y0 - 1000 * (a));
                Scalar color = Scalar(255, 255, 255);
                line(detectedLines_output, pt1, pt2, color, 1, LINE_AA);
            }

            houghTransform.setFromPixels(detectedLines_output.data, detectedLines_output.cols, detectedLines_output.rows);
           
            // Canny Edge x Grey Scale
                // Convert both grayscale and Canny to 3-channel for color blending
                    cvtColor(grayMat, gray3ch, COLOR_GRAY2BGR);
                    cvtColor(detectedEdges_output, canny3ch, COLOR_GRAY2BGR);
                    addWeighted(gray3ch, 0.8, canny3ch, 2, 0.0, blended);
                    stackedEdgesOnGray.setFromPixels(blended.data, blended.cols, blended.rows);


                /*
             Notes for report (contains math):
             Slider (another idea to add to make interesting): https://openframeworks.cc/learning/01_basics/how_to_create_slider/
             Mat class information - https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html
             Guassian Blur - https://docs.opencv.org/3.4/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1
                 Math behind Guassian Blur - https://www.pixelstech.net/article/1353768112-gaussian-blur-algorithm
             Mat - https://docs.opencv.org/4.x/d6/d6d/tutorial_mat_the_basic_image_container.html
             Canny, Sobel, Guassian Blur  - https://docs.opencv.org/3.4/d2/d2c/tutorial_sobel_derivatives.html
                 Some more stuff on canny edge https://docs.opencv.org/3.4/da/d5c/tutorial_canny_detector.html
             Hough Transformation - https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
            Blended - https://docs.opencv.org/3.4/d2/de8/group__core__array.html#gafafb2513349db3bcff51f54ee5592a19
            */

            // Debugging - Displaying output for detectedLines_output
                /*COMMENT THIS OR REMOVE SO PROGRAM CAN OUTPUT IMAGES 
                WITH THIS UNCOMMENTED/NOT REMOVED, PROGRAM WILL NOT OUTPUT IMAGES
                */

                   /*
                    cout << "Hough Image Output:" << endl;
                    for (int i = 0; i < detectedLines_output.rows; i++) {
                        for (int j = 0; j < detectedLines_output.cols; j++) {
                            cout << "[ " << detectedLines_output.at<double>(i, j) << " ] " << endl;
                        }
                    }
                    */
            
            /*
            Notes: All using 1:3 Canny Edge Threshold Ratio
            size 3 = very staticy
            size 11 = dark in some areas, but produces much less static than 3
            size 89 = very dark, hard to make out image most of the time
            size 501+ = laptop starts to lag
            With a small enough kernal size, blur becomes useless in canny edge detection (visually the same)
            Father away, more static that will start to appear as white objects

           Having difficulty trying to see the effect of sigmaX and sigmaY, need further testing
                Finer details like graphic shirts appear to be less staticy, the higher the sigma values are
                    Have not tested upper limit, downside is that things like face outline has edges taken out if it
           */
           
    }
}

void ofApp::draw(){
    // Row 1
        colorImg.draw(0, 0); // starts drawing top left            
        grayImg.draw(colorImg.getWidth(), 0);

    // Row 2
        blurredImg.draw(colorImg.getWidth() * 2, 0);
        cannyEdge.draw(0, colorImg.getHeight());
        houghTransform.draw(colorImg.getWidth(), colorImg.getHeight());
        stackedEdgesOnGray.draw(colorImg.getWidth() * 2, colorImg.getHeight());


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
