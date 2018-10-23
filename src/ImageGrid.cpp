//
//  ImageGrid.cpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#include "ImageGrid.hpp"

void ImageGrid::setup(ofShader* shader, Group* group, bool profile, int wElement, int hElement, int userPerLevel, string title, float scale, ofColor bg) {
    this->shader = shader;
    this->group = group;
    this->profile = profile;
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
}

// call this to calculate where eto drasw next
ofPoint ImageGrid::getSize() {
    return ofPoint(wholeSize.x/scale, wholeSize.y/scale);
}

void ImageGrid::update() {
    int x = 0;
    int y = 0;
    currUser = 0;
    vector<User*> users(0);
    group->getGridUsers(userPerLevel, users);
    fbo.begin();
    ofPushStyle();
    ofSetColor(ofColor::antiqueWhite);
    ofFill();
    for(int i = group->numLevels -1; i >=0; i--) {
        drawLine(y, users);
        y += lineSize.y + Y_SPACING;
    }
    ofPopStyle();
    fbo.end();
}

void ImageGrid::calculateSizes() {
    elementSize.x = w + ELEMENT_SIDE_PADDING * 2;
    elementSize.y = h + SCORE_AREA_HEIGHT;
    lineSize.x = PADDING_LINE_SIDE_TOP * 2 + userPerLevel * elementSize.x;
    lineSize.y = PADDING_LINE_SIDE_TOP + elementSize.y;
    wholeSize.x = lineSize.x;
    wholeSize.y = lineSize.y * group->numLevels + Y_SPACING * (group->numLevels- 1);
}

void ImageGrid::draw(int x, int y) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale);
    fbo.draw(0,0);
    ofPopMatrix();
}

void ImageGrid::drawLine(int y, vector<User*> users) {
    int x = 0;
    ofDrawRectangle(x, y, lineSize.x, lineSize.y);
    y += PADDING_LINE_SIDE_TOP;
    x += PADDING_LINE_SIDE_TOP;
    for (int j = 0; j < userPerLevel; j++) {
        drawElement(users[currUser], x, y);
        currUser++;
        x += elementSize.x;
    }
}

void ImageGrid::drawElement(User* user, int x, int y) {
    if (user != NULL) {
        View& view = user->getView(profile);
        ofImage& face = view.getImage();
        ofRectangle& box = view.getBounderyBox(group->feature);
        box = adjustAspectRatio(box, aspectRatio);
        face.bind();
        shader->begin();
        shader->setUniform1f("factor", ofRandom(0.7, 1.0)); // SET A UNIFORM
        face.drawSubsection(x + ELEMENT_SIDE_PADDING, y, w, h, box.x, box.y, box.width, box.height);
        shader->end();
        face.unbind();
        drawScoreArea(x , y - h);
    }
}

void ImageGrid::drawScoreArea(int x, int y) {
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
