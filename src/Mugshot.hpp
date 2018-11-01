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

class Mugshot  {
public:
    static const int MG_WIDTH = 614;
    static const int MG_HEIGHT = 342;
    static const int MG_SPACE = 2;

    //static const float MG_ASPECT_RATIO = 0.953271; //(float)(MG_WIDTH - MG_SPACE)/2. / MG_HEIGHT;
    void setup(ofShader* shader);
    void update(User* user, View::Features feature);
    void draw(int x, int y);
#ifdef TARGET_OSX
    static void drawLettersFront(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font);
    static void drawLettersProfile(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font);
#endif
#ifdef TARGET_WIN32
    static void drawLettersFront(ofRectangle* parts, vector<glm::vec2>& landmarks, shared_ptr<ofxSmartFont> font);
    static void drawLettersProfile(ofRectangle* parts, vector<glm::vec2>& landmarks, shared_ptr<ofxSmartFont> font);
#endif
    static void drawDottedLine(ofVec2f start, ofVec2f end);
    ~Mugshot() {
        fbo.clear();
    };
    
    ofFbo fbo;
    User* user;
    View::Features feature;
    ofShader* shader;
};

#endif /* Mugshotx_hpp */
