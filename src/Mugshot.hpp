//
//  Mugshotx.hpp
//  TNM2
//
//  Created by Dan Stavy on 26/10/2018.
//

#ifndef Mugshotx_hpp
#define Mugshotx_hpp

#include "ofMain.h"
#include "View.hpp"
#include "User.hpp"
#include "ofxSmartFont.h"
#include "Tweener.h"

class ofApp;

static const int MG_WIDTH = 306; // width of one mugshot
static const int MG_HEIGHT = 342;
static constexpr float MG_ASPECT_RATIO = (float)MG_WIDTH/(float)MG_HEIGHT;
static const int MG_SPACE = 2;
static const int MG_X_MOVE = 500;
static const int MG_Y_MOVE = 1000;
static const int START_X =  132;
static const int START_Y =  0;
static const int MG_CARD_INSET_X = 82;
static const int DROP_SHADOW_W = 20;
static const int DROP_SHADOW_H = 18;
static const float MG_Y_START = 238 + DROP_SHADOW_H;

class Mugshot  {
public:
	static const int ANIMATION_TIME = 1;
	static const int TOTAL_WIDTH = 816;
	
	static void drawDottedLine(ofVec2f start, ofVec2f end);
	
	//
	Mugshot(ofShader* shader, User* user, ofApp* app);
	~Mugshot() {
		fbo.clear();
		facecutFbo.clear();
		bgFbo.clear();
	};
	
    void drawBackground(User* user);
    void update(View::Features feature);
    bool draw();
    void animate(float delay);
	View::Features selectNextFeature();
	void resetFeatures();
	void introAnimationDone();
	void resetFaceImage() {
//		face = ofImage();
		featureImageAlpha = 0.0;
	}
	void calculateRectangles();
	void drawOverlay();
	void drawFacecutFbo();
	void drawFbo();
	
	void showDarkFrame() {
		featureFrameAlpha = 1.0;
	}
	void fadeOutDarkFrame();
	
#ifdef TARGET_OSX
	
    static void drawLettersFront(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font);
    static void drawLettersProfile(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font);
	
#elif defined TARGET_WIN32
	
    static void drawLettersFront(ofRectangle* parts, vector<glm::vec2>& landmarks, shared_ptr<ofxSmartFont> font);
    static void drawLettersProfile(ofRectangle* parts, vector<glm::vec2>& landmarks, shared_ptr<ofxSmartFont> font);
	
#endif

	
	int x = 0;
	int y = 0;
	int animateCounter = 0;
	float rotation = 0;
	float scale = 1;
	glm::vec3 pos;
	//static const float MG_ASPECT_RATIO = 0.953271; //(float)(MG_WIDTH - MG_SPACE)/2. / MG_HEIGHT;
    
    ofFbo fbo; // overall composition
	ofFbo bgFbo;
	ofFbo facecutFbo;
	ofFbo faceoverlayFbo;
    User* user;
    View::Features currentFeature = View::Features::INVALID;
    ofShader* shader;
    TWEEN::Manager tweenManager;
    bool firstTime = true;
	
	vector<int> partsToVisit;
	ofApp* appcontroller;
	bool m_introAnimationDone = false;
	ofImage face;
	ofRectangle faceBox;
	ofRectangle featureRect;
	ofPoint partScale;
	
	float featureImageAlpha = 0.0;
	float featureFrameAlpha = 0.0;
};

#endif /* Mugshotx_hpp */
