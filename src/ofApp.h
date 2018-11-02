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
#include "ImageGrid.hpp"
#include "Mugshot.hpp"


class ofApp : public ofBaseApp{
public:
    static const int NUM_IMAGE_GRIDS = 8;
    //static const int NUM_GROUPS = 8;
    const long VIDEO_GRID_REFRESH = 2000;
    static const long PRESENTATION_UPDATE_REFRESH = 30000;
    static const int MUGSHOT_REFRESH = 10000;
    void setup();
    void exit();
    void update();
    void draw();
    void keyReleased(int key);
    ofRectangle getBoundingBox(ofRectangle rec1, ofRectangle rec2);
    void drawVideo(ofVideoPlayer& player, ofRectangle& face, int x, int y, int w, int h);
    void presentationUpdater();
    void drawMugshotPage();
    User* getRandomUser();
    void drawGridPage();
	void drawBg();


    ofxFaceTracker2 frontTracker;
    ofxFaceTracker2 profileTracker;
    ofVideoPlayer frontPlayer;
    ofVideoPlayer profilePlayer;
    PresentationUpdate presentationUpdate;
    UserMap users;
    ofImage tablePage;
    ofImage mugshotPage;
    ofImage tableHeader;
    ofImage table_bg;
    GroupManager groupManager;
    
    ofShader sepiaShader;
    ofRectangle frontFace;
    ofRectangle profileFace;
    int rotation;
    ImageGrid grids[NUM_IMAGE_GRIDS];
    Mugshot mugshot;
    
    ofPoint  getGridLocation();
    
    //ofFbo gridFbo;
    int gridSize = 600;
    //float boxPadding = 200;
    int nextGrid = -1;
    long lastGridUpdate;
    long lastPresentationUpdate;
    User* currentUser = NULL;
    int frontVideoStartFrame;
    int proflleVideoStartFrame;
    static void setupFonts();
    static View::Features selectNextFeature(View::Features feature);
    View::Features curFeature;
    long lastMugshotUpdate = 0;
    //fonts
};
