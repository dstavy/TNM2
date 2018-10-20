// ofxFaceTrack2 example: SIMPLE EXAMPLE
//
// This example shows minimal usage of the ofxFaceTracker2 addon,
// doing simple landmark detection and pose estimation
//

#pragma once
#include "ofMain.h"
#include "ofxFaceTracker2.h"
#include "PresentationUpdate.hpp"
#include "GroupManager.hpp"
#include "User.hpp"

class ofApp : public ofBaseApp{
public:
    void setup();
    void exit();
    void update();
    void draw();
    void keyReleased(int key);
    ofRectangle getBoundingBox(ofRectangle rec1, ofRectangle rec2);
    void drawVideo(ofVideoPlayer& player, ofRectangle& face, int x, int y, int w, int h);
    void presentationUpdater();
    void drawTablePage();
    void drawElement(User* user, bool profile, View::Features feature, int xScreen, int yScreen, int w, int h, float aspectRatio);
    ofRectangle& adjustAspectRatio(ofRectangle& box, float aspectRatio);
    void drawGrid(View::Features feature, Group::GroupBy by, bool profile, int x, int y, int w, int h, int userPerLevel);


    
    ofxFaceTracker2 frontTracker;
    ofxFaceTracker2 profileTracker;
    ofVideoPlayer frontPlayer;
    ofVideoPlayer profilePlayer;
    PresentationUpdate presentationUpdate;
    UserMap users;
    ofImage tablePage;
    GroupManager groupManager;
    
    ofShader sepiaShader;
    ofRectangle frontFace;
    ofRectangle profileFace;
    int rotation;
    
    ofPoint  getGridLocation();
    
    ofFbo gridFbo;
    int gridSize = 600;
    //float boxPadding = 200;
    int nextGrid = -1;
    static const long VIDEO_GRID_REFRESH;
    static const long PRESENTATION_UPDATE_REFRESH;
    long lastGridUpdate;
    long lastPresentationUpdate;
    User* currentUser = NULL;
    int frontVideoStartFrame;
    int proflleVideoStartFrame;
};