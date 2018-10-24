//
//  ImageGrid.cpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#include "ImageGrid.hpp"
#include "ofxSmartFont.h"

void ImageGrid::setup(ofShader* shader, Group* group, int wElement, int hElement, int userPerLevel, float scale, string title, ofColor bg) {
    this->shader = shader;
    this->group = group;
    this->w = wElement;
    this->h = hElement;
    this->userPerLevel = userPerLevel;
    this->title = title;
    this->scale = scale;
    this->bg = bg;
    //this->leftPanel = leftPanel;
    aspectRatio = w/h;
    
    calculateSizes();
    int fboW = wholeSize.x;
    int fboH = wholeSize.y;
    fbo.allocate(fboW, fboH);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    getSize();
}

// call this to calculate where to draw next
ofPoint ImageGrid::getSize() {
    return ofPoint(wholeSize.x/scale, wholeSize.y/scale);
    ofLogNotice("Grid:" + getTitle() + "size: " + ofToString(wholeSize.x/scale) + " " + ofToString(wholeSize.y/scale));
}

void ImageGrid::update() {
    int x = 0;
    int y = 0;
    vector<User*> users(0);
    group->getGridUsers(userPerLevel, users);
    ofPushStyle();
    ofSetColor(bg);
    ofFill();
    fbo.begin();
    drawHeader();
    y += HEADER_HEIGHT;
    vector<User*>::iterator it;
    for(int i = group->numLevels -1; i >=0; i--) {
        drawRow(y, users.begin() + (i * userPerLevel));
        y += lineSize.y + Y_SPACING;
    }
    fbo.end();
    ofPopStyle();
}

void ImageGrid::calculateSizes() {
    elementSize.x = w + ELEMENT_SIDE_PADDING * 2;
    elementSize.y = h + SCORE_AREA_HEIGHT;
    lineSize.x = PADDING_ROW * 2 + userPerLevel * elementSize.x;
    lineSize.y = PADDING_ROW *2 + elementSize.y;
    wholeSize.x = lineSize.x;
    wholeSize.y = HEADER_HEIGHT + lineSize.y * group->numLevels + Y_SPACING * (group->numLevels- 1);
}

void ImageGrid::draw(int x, int y) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale);
    fbo.draw(0,0);
    ofPopMatrix();
}

string ImageGrid::getTitle() {
    return View::featureToString(group->feature) + " / " + View::proflieToString(group->profile);
}
                
void ImageGrid::drawHeader() {
    //ofSetColor(bg);
    ofDrawRectangle(0,0, wholeSize.x, HEADER_HEIGHT);
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("BodonPoster");
    ofSetColor(0,0,0); //(ofColor::black);
    string title = getTitle();
    int hs = font->height(title);
    int ws = font->width(title);
    font->draw(title, wholeSize.x / 2 - ws /2, HEADER_HEIGHT/2 + hs /2);
    ofSetColor(bg);
}

void ImageGrid::drawRow(int y, vector<User*>::iterator it) {
    int x = 0;
    ofDrawRectangle(x, y, lineSize.x, lineSize.y);
    y += PADDING_ROW;
    x += PADDING_ROW;
    for (int j = 0; j < userPerLevel; j++) {
        drawElement(*it, x, y);
        it++;
        x += elementSize.x;
    }
}

void ImageGrid::drawElement(User* user, int x, int y) {
    if (user != NULL) {
        View& view = user->getView(group->profile);
        ofImage& face = view.getImage();
        ofRectangle& box = view.getBounderyBox(group->feature);
        box = adjustAspectRatio(box, aspectRatio);
        face.bind();
        shader->begin();
        shader->setUniform1f("factor", ofRandom(0.7, 1.0)); // SET A UNIFORM
        face.drawSubsection(x + ELEMENT_SIDE_PADDING, y, w, h, box.x, box.y, box.width, box.height);
        shader->end();
        face.unbind();
        drawScoreArea(user->score, user->currentUser, x , y + h);
    }
}

void ImageGrid::drawScoreArea(float score, bool isCurrent, int x, int y) {
    shared_ptr<ofxSmartFont> font;
    if (isCurrent) {
        font = ofxSmartFont::get("AmericanTypewriter"); // change to bold
        ofSetColor(ofColor::lightGrey);
        ofDrawRectangle(x, y, w, SCORE_AREA_HEIGHT);
    } else {
        font = ofxSmartFont::get("AmericanTypewriter");
    }
    
    ofSetHexColor(0x242124);
    string sScore = ofToString(std::floor(score * 100.) / 100.);
    int hs = font->height(sScore);
    int ws = font->width(sScore);
    font->draw(sScore, x + w - ws, y + SCORE_AREA_HEIGHT/2 + hs/2);
    ofSetColor(bg);
}

ofRectangle& ImageGrid::adjustAspectRatio(ofRectangle& box, float aspectRatio) { // x/y
    float imageAspectRatio = (float)box.width/box.height;
    int w = box.width;
    int h = box.height;
    if (imageAspectRatio > aspectRatio) {
        h *=  imageAspectRatio / aspectRatio;
    } else if (imageAspectRatio < aspectRatio) {
        w *=  aspectRatio / imageAspectRatio;
    }
    
    box.y -= (h - box.height) /2;
    box.x -= (w - box.width) /2;
    box.height = h;
    box.width = w;
    
    return box;
}
