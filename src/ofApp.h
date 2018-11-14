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
#include "Tweener.h"
#include "Globals.h"


class ofApp : public ofBaseApp{
public:
	
	enum RejectedNextUser {
		NONE,
		NORMAL,
		RANDOM,
	};
	
    static const int NUM_IMAGE_GRIDS = 10;
    //static const int NUM_GROUPS = 8;
    const long VIDEO_GRID_REFRESH = 2000;
    static const long PRESENTATION_UPDATE_REFRESH = 30000;
    static const int MUGSHOT_REFRESH = 15000;
    static const int CURRENT_USER_REFRESH = 10000;// 120000
	static const int IDLE_REFRESH = 5000;
	
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
	
	void setFeatureToFocus(View::Features, float);
	void selectNewFeature();
	void selectFeature(View::Features);
	void selectNextUser(bool random = false);


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
    Mugshot* currMugshot;
    
    ofPoint  getGridLocation();
	void mugshotIntroAnimationDone();
	void signalCurrentMugshotImageOff();
	Mugshot* getCurrentMugshot() {
		return currMugshot;
	}
    
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
    static View::Features selectRandomFeature(View::Features feature);
    View::Features curFeature;
	View::Features lastIdleFeature;
    long lastMugshotUpdate = 0;
    long lastUserUpdate = 0;
	long lastIdleUpdate = 0;
    ofEasyCam cam;
    TWEEN::Manager tweenManager;
    glm::vec3 camScale;
    vector<Mugshot*> mugshots;
    static const int ANIM_DELAY = 0.2;
    void animateMagshots();
	
	bool mugshotIsLeft;
	
	View::Features currentFeatureToFocus;
	float gridY = 0.0;
	shared_ptr<TWEEN::Tween> gridTween;
	ofImage partImage;
	ofRectangle featureRect;
	bool autoupdateFeatures = true;
	//
	ofFbo outputFbo;
	bool scaleOutput = true;
	
	RejectedNextUser rejectedNextUser = NONE;	
};
