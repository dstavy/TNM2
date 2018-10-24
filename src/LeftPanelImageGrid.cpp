//
//  LeftPanelImageGrid.cpp
//  TNM2
//
//  Created by Dan Stavy on 24/10/2018.
//

#include "LeftPanelImageGrid.hpp"
#include "ofxSmartFont.h"

void LeftPanelImageGrid::setup() {
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    std::cout <<"Max texture size: " << maxTextureSize << std::endl;
    bigFbo.allocate(100, 700);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    bigFbo.begin();
    ofClear(0,0,0,255);
    bigFbo.end();
    //getSize();
};

void LeftPanelImageGrid::update() {
    int y = 0;
    grid->update();
    ofFill();
    ofSetColor(grid->getBgColor());
    bigFbo.begin();
    ofDrawRectangle(0, 0, LEFT_PANEL_WIDTH, ImageGrid::HEADER_HEIGHT);
    string letter;
    for(int i = 0; i < grid->group->numLevels; i++) {
        letter =(char) i + 10;
        drawPanel(y, letter);
        y += grid->lineSize.y + ImageGrid::Y_SPACING;
    }
    grid->fbo.draw(LEFT_PANEL_WIDTH, 0);
    bigFbo.end();
}

void LeftPanelImageGrid::drawPanel(int y, string text) {
    ofDrawRectangle(0, y, LEFT_PANEL_WIDTH, grid->lineSize.y);
    ofSetColor(ofColor::black);
    ofSetLineWidth(LINE_WIDTH);
    ofDrawLine(LEFT_PANEL_WIDTH - LINE_WIDTH, y + grid->PADDING_ROW , LEFT_PANEL_WIDTH - LINE_WIDTH, grid->lineSize.y -  grid->PADDING_ROW);
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("CrimsonRegular");
    int hs = font->height(text);
    int ws = font->width(text);
    font->draw(text, LEFT_PANEL_WIDTH /2 - ws /2, y + grid->lineSize.y/2 + hs/2);
    ofSetColor(grid->getBgColor());
}

ofPoint LeftPanelImageGrid::getSize() {
    ofPoint size;
    size.x = LEFT_PANEL_WIDTH + grid->wholeSize.x;
    size.y = grid->wholeSize.y;
}

void LeftPanelImageGrid::draw(int x, int y) {
    ofPoint size = getSize();
    ofPushMatrix();
    ofTranslate(x,y);
    //ofScale(scale);
    bigFbo.getTexture().drawSubsection(0, 0, size.x, size.y, 0, 0, size.x, size.y);
    ofPopMatrix();
}
