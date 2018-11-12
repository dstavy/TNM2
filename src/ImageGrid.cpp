//
//  ImageGrid.cpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#include "ImageGrid.hpp"
#include "ofxSmartFont.h"

#define FIXED_FLOAT(x) std::fixed <<std::setprecision(2)<<(x)

void ImageGrid::setup(ofShader* shader, Group* group, int wElement, int hElement, int userPerLevel, float scale, int delayLoading, string title, ofColor bg) {
    this->shader = shader;
    this->group = group;
    this->w = wElement;
    this->h = hElement;
    this->userPerLevel = userPerLevel;
    this->title = title;
    this->scale = scale;
    this->bg = bg;
    this->delayLoading = delayLoading;
    //this->leftPanel = leftPanel;
    aspectRatio = (float)w/h;
    
    calculateSizes();
    int fboW = wholeSize.x;
    int fboH = wholeSize.y;
    fbo.allocate(fboW, fboH/*,GL_RGB*/);
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
    ofClear(ofColor::black);
    ofSetColor(bg);
    ofFill();
    fbo.begin();
    //    drawHeader();
    //    y += HEADER_HEIGHT;
    y += currElement * (rawSize.y + Y_SPACING);
    int rawMax = (group->numLevels * userPerLevel) % currElement;
    for(int i = 0; i < rawMax; i++) {
        drawRow(y, users.begin() + (i * userPerLevel), userPerLevel);
        y -= rawSize.y + Y_SPACING;
    }
    int remain = currElement - (rawMax * userPerLevel);
    if (remain > 0) {
         drawRow(y, users.begin() + (remain * userPerLevel), remain);
    }
    fbo.end();
    ofPopStyle();
}

void ImageGrid::calculateSizes() {
    elementSize.x = w + ELEMENT_SIDE_PADDING * 2;
    elementSize.y = h + SCORE_AREA_HEIGHT;
    rawSize.x = PADDING_ROW * 2 + userPerLevel * elementSize.x;
    rawSize.y = PADDING_ROW *2 + elementSize.y;
    wholeSize.x = rawSize.x;
    wholeSize.y = rawSize.y * group->numLevels + Y_SPACING * (group->numLevels- 1);
    //    wholeSize.y = HEADER_HEIGHT + lineSize.y * group->numLevels + Y_SPACING * (group->numLevels- 1);
}

void ImageGrid::draw(int x, int y) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale);
    if (loading) {
        int section = floor((float)(ofGetElapsedTimeMillis() - loadingTime) / delayLoading) * DRAW_SEGMENT;
        fbo.getTextureReference().drawSubsection(0, 0, wholeSize.x, section, 0, 0, wholeSize.x, section);
        if ((wholeSize.y - section) <= DRAW_SEGMENT) {
            loading = false;
        }
    }
    else {
        fbo.draw(0,0);
    }
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

void ImageGrid::drawRow(int y, vector<User*>::iterator it, int num) {
    int x = 0;
    ofDrawRectangle(x, y, rawSize.x, rawSize.y);
    y += PADDING_ROW;
    x += PADDING_ROW;
    for (int j = 0; j < num; j++) {
        drawElement(*it, x, y);
        it++;
        x += elementSize.x;
    }
}

void ImageGrid::drawElement(User* user, int x, int y) {
    if (user != NULL) {
        View& view = user->getView(group->profile);
        if (view.isActive()) {
            ofImage& face = view.getImage();
            ofRectangle box(view.getBounderyBox(group->feature));
            if (box.width > 0) {
                box = adjustAspectRatio(box, aspectRatio);
                face.bind();
                shader->begin();
                if (user->isCurrent) {
                     shader->setUniform1f("factor", 0.0);// color
                } else {
                    shader->setUniform1f("factor", ofRandom(0.75, 1.0));
                }
                face.drawSubsection(x + ELEMENT_SIDE_PADDING, y, w, h, box.x, box.y, box.width, box.height);
                shader->end();
                face.unbind();
                drawScoreArea(user->getFactrorScore(), user->isCurrent, x , y + h);
            }
        }
    }
}

void ImageGrid::drawScoreArea(float score, bool isCurrent, int x, int y) {
    shared_ptr<ofxSmartFont> font;
    if (isCurrent) {
        font = ofxSmartFont::get("AmericanTypewriter700"); // change to bold
        ofSetColor(ofColor::lightGrey);
        ofDrawRectangle(x, y, w, SCORE_AREA_HEIGHT);
    } else {
        font = ofxSmartFont::get("AmericanTypewriter");
    }
        //ofColor::fromHex(0xf1efe3);
    ofSetColor(ofColor::black); //TODO: change
    std::stringstream buffer;
    buffer << FIXED_FLOAT(score);
    string sScore = buffer.str();
    int hs = font->height(sScore);
    int ws = font->width(sScore);
    font->draw(sScore, x + (w - ws)/2, y + SCORE_AREA_HEIGHT/2 + hs/2);
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
