//
//  ImageGrid.hpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#ifndef ImageGrid_hpp
#define ImageGrid_hpp

#include "ofMain.h"
#include "Group.hpp"
#include "Tweener.h"
#include "Globals.h"

class ofApp;
class Mugshot;

class ImageGrid  {
public:
	
	enum AnimationStage {
        NO_ANIM,
		INIT,
		DELAY,
		SCALE,
		FLY_IN,
		FADE_IN,
		FADE_OUT,
		DONE,
	};
	
	ImageGrid() {	}

	~ImageGrid() {
		fbo.clear();
	};
	
    void setup(ofApp* app,
			   ofShader* shader,
			   Group* group,
			   int wElement,
			   int hElement,
			   int userPerLevel,
			   ofPoint flyInStartPos,
               float gridX,
               float gridY,
			   float scale = 1.0,
			   int delayLoading = TIME_ROW_STEP*1000.0,
			   string title = "",
			   ofColor bg = ofColor::fromHex(0xe6e0d3)
               );
	
    void update(bool doAnim);
	void doUpdate(bool doAnim);
    void draw();
	
	ofPoint getSize();
	void setScale(float scale) {
		this->scale = scale;
	};
	
	ofColor getBgColor() {
		return bg;
	};
	
	void reset();
	void drawFlyingImage();
	
	
    int PADDING_ROW = 4;
    int SCORE_AREA_HEIGHT = 18;
    int ELEMENT_SIDE_PADDING = 1;
	int HEADER_HEIGHT = 60;
    int Y_SPACING = 4;
	
    
    ofFbo fbo;
	//ofFbo maskFbo;
	ofFbo outputFbo;
	float fboAlpha = 1.0;
    Group *group;
    int w;
    int h;
    int userPerLevel;
    float aspectRatio;
    ofShader* shader;
    string title;
    float scale;
    ofColor bg;
    ofPoint gridLocation = {0,0};
    //bool leftPanel;
	
    void calculateSizes();
    string getTitle();
    void drawHeader();
    void drawRow(int y, vector<User*>::iterator it, int num);
    void drawLeftPanel();
    void drawElement(User* user, int x, int y);
    void drawScoreArea(float score, bool isCurrent, int x, int y);
	
    ofPoint rawSize;
    ofPoint elementSize;
    ofPoint wholeSize;
    bool loading = false;
    int loadingTime = 0;
    int delayLoading = 0;
	
    static ofRectangle& adjustAspectRatio(ofRectangle& box, float aspectRatio);
	void resetLoading();
	
    //vector<User*> users;
    static const int DRAW_SEGMENT = 10;
    static const long ELEMENT_DELAY = 200;
    int currElement = 1;
	TWEEN::Manager tweenManager;
	
	ofPoint currentUserPosition; // target position for flying image
	User* currentUser;
	AnimationStage animStage;
	ofImage flyInImage;
	ofPoint flyInStartPosition; // start-position depends on the feature
	ofPoint flyInImagePosition; // current position
	ofPoint flyingImageSize; // current size
	ofPoint flyingImageRectSize;
	ofPoint flyingImageRectPos;
	ofPoint flyingImageImageOffset;
	
	ofRectangle featureRect;
	
	ofRectangle faceBox;
	ofPoint partScale;
	ofApp* appcontroller;
	int signalOnNextRender = -1;
	
	ofShader maskShader;
	void setupAnimation();
	void startScaleUpFlyingImage();
	void startFlyingAnimation();
	float dummy;
	float imageAlpha = 1.0;
	Mugshot* currentMugshot;
	bool doUpdateNextUpdate = false;
    
    Group* getGroup() {
        return group;
    };
};
#endif /* ImageGrid_hpp */

