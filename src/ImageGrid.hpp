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

class ImageGrid  {
public:
	
	enum AnimationStage {
		FADE_OUT = 0,
		FLY_IN,
		FADE_IN,
	};
	
	ImageGrid() {}

	~ImageGrid() {
		fbo.clear();
	};
	
    void setup(ofShader* shader,
			   Group* group,
			   int wElement,
			   int hElement,
			   int userPerLevel,
			   ofPoint flyInStartPos,
			   float scale = 1.0,
			   int delayLoading = 100,
			   string title = "",
			   ofColor bg = ofColor::fromHex(0xe6e0d3));
	
    void update();
    void draw(int x, int y);
	
	ofPoint getSize();
	void setScale(float scale) {
		this->scale = scale;
	};
	
	ofColor getBgColor() {
		return bg;
	};
	
	void reset();
	
	
    int PADDING_ROW = 4;
    int SCORE_AREA_HEIGHT = 18;
    int ELEMENT_SIDE_PADDING = 1;
	int HEADER_HEIGHT = 60;
    int Y_SPACING = 4;
	
    
    ofFbo fbo;
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
    void resetLoading() {
        loading = true;
        loadingTime = ofGetElapsedTimeMillis();
		fboAlpha = 1.0;
		animStage = FADE_IN;
    };
	
    //vector<User*> users;
    static const int DRAW_SEGMENT = 10;
    static const long ELEMENT_DELAY = 200;
    int currElement = 1;
	TWEEN::Manager tweenManager;
	
	ofPoint currentUserPosition;
	User* currentUser;
	AnimationStage animStage;
	ofImage flyInImage;
	ofPoint flyInImagePosition;
	ofRectangle featureRect;
	ofPoint flyInStartPosition; // start-position depends on the feature
};
#endif /* ImageGrid_hpp */

