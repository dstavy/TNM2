// ofxFaceTrack2 example: SIMPLE EXAMPLE
//
// This example shows minimal usage of the ofxFaceTracker2 addon,
// doing simple landmark detection and pose estimation
//

#pragma once
//#define _SMILE
#include "ofMain.h"
#include "ofxFaceTracker2.h"
//#ifdef _SMILE
#include "ofxBiquadFilter.h"
//#endif
#include "ofxVideoRecorder.h"


typedef dlib::matrix<double,40,1> sample_type;
typedef dlib::radial_basis_kernel<sample_type> kernel_type;

typedef dlib::decision_function<kernel_type> dec_funct_type;
typedef dlib::normalized_function<dec_funct_type> funct_type;

typedef dlib::probabilistic_decision_function<kernel_type> probabilistic_funct_type;
typedef dlib::normalized_function<probabilistic_funct_type> pfunct_type;


class ofApp : public ofBaseApp{
public:
    void setup();
    void exit();
    void update();
    void draw();
    void keyReleased(int key);
    ofRectangle getBoundingBox(ofRectangle rec1, ofRectangle rec2);
    sample_type makeSample();
    void drawVideo(ofVideoPlayer player, int x, int y, int w, int h);
    
    ofxFaceTracker2 tracker;
    //ofVideoGrabber grabber;
    ofRectangle boundBoxes[8];
    int rotation;
//#ifdef _SMILE
    ofxBiquadFilter1f neutralValue;
    ofxBiquadFilter1f smallSmileValue;
    ofxBiquadFilter1f bigSmileValue;
    ofxBiquadFilter1f oValue;
    
    vector<pfunct_type> learned_functions;
//#endif
    
    ofxVideoRecorder    vidRecorder;
    ofSoundStream       soundStream;
    bool bRecording;
    int sampleRate;
    int channels;
    string fileName;
    string fileExt;
    int sessionId;
    
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    
    ofFbo recordFbo;
    ofPixels recordPixels;
    ofVideoPlayer frontPlayer;
    ofVideoPlayer sidePlayer;
    ofShader sepiaShader;
    int select;
    
    //float boxPadding = 200;
};
