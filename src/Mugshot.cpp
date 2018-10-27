//
//  Mugshotx.cpp
//  TNM2
//
//  Created by Dan Stavy on 26/10/2018.
//

#include "Mugshot.hpp"
#include "ImageGrid.hpp"
#include "FontUtil.hpp"

void Mugshot::setup(ofShader* shader) {
    fbo.allocate(MG_WIDTH, MG_HEIGHT/*,GL_RGB*/);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    this->shader = shader;
}

void Mugshot::update(User* user, View::Features feature) {
     int x = 0;
     float aspectRatio = (float)(MG_WIDTH - MG_SPACE)/2. / MG_HEIGHT;
     shared_ptr<ofxSmartFont> font = ofxSmartFont::get("BodonPosterMugshot");
     bool profile = true;
     int mugSize = (MG_WIDTH - MG_SPACE) / 2;
     fbo.begin();
    
    View& view = user->getView(profile);
    if (view.isActive()) {
         ofImage& face = view.getImage();
         ofRectangle& faceRec(view.parts[View::HEAD]);
         faceRec.x += faceRec.x/2;
         ofRectangle& box = ImageGrid::adjustAspectRatio(faceRec, aspectRatio);
         face.bind();
         //shader->begin();
        // shader->setUniform1f("factor", 0.9); // SET A UNIFORM
         face.drawSubsection(0, 0, mugSize ,MG_HEIGHT, box.x, box.y, box.width, box.height);
        // shader->end();
         face.unbind();
         ofPushMatrix();
         ofScale( (float)mugSize/box.width, (float)MG_HEIGHT/box.height);
         ofTranslate(-box.x, -box.y);
         drawLettersProfile(view.parts, view.getLandmarks(), font);
         ofPopMatrix();
     }
    
     profile = false;
      view = user->getView(profile);
     if (view.isActive()) {
         ofImage& face = view.getImage();
         ofRectangle& box = ImageGrid::adjustAspectRatio(view.parts[View::HEAD], aspectRatio);
         face.bind();
         //shader->begin();
        // shader->setUniform1f("factor", 0.9); // SET A UNIFORM
         face.drawSubsection(mugSize + MG_SPACE, 0, mugSize, MG_HEIGHT, box.x, box.y, box.width, box.height);
        // shader->end();
         face.unbind();
         ofPushMatrix();
         ofTranslate(mugSize + MG_SPACE, 0);
         ofScale( (float)mugSize/box.width, (float)MG_HEIGHT/box.height);
         ofTranslate(-box.x, -box.y);
         //ofTranslate(mugSize + MG_SPACE, 0);
         drawLettersFront(view.parts, view.getLandmarks(), font);
         ofPopMatrix();
     }
     fbo.end();
    
}

void Mugshot::drawLettersFront(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font) {
    ofSetColor(ofColor::white);
    FontUtil::draw("D", font, FontUtil::CENTER, ofVec2f(landmarks[0].x, landmarks[0].y - (landmarks[3].y - landmarks[0].y)));
    FontUtil::draw("D'", font, FontUtil::CENTER, ofVec2f(landmarks[16].x, landmarks[16].y - (landmarks[13].y - landmarks[16].y)));
    FontUtil::draw("I", font, FontUtil::CENTER, landmarks[1]);
    FontUtil::draw("I'", font, FontUtil::CENTER, landmarks[15]);
    FontUtil::draw("K", font, FontUtil::LEFT, landmarks[3]);
    FontUtil::draw("K'", font, FontUtil::RIGHT, landmarks[13]);
    FontUtil::draw("L", font, FontUtil::LEFT, landmarks[48]);
    FontUtil::draw("L'", font, FontUtil::RIGHT, landmarks[54]);
    FontUtil::draw("G", font, FontUtil::LEFT, landmarks[27]);
    FontUtil::draw("G'", font, FontUtil::RIGHT, landmarks[27]);
    FontUtil::draw("H", font, FontUtil::LEFT, landmarks[36]);
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[37].x + landmarks[38].x) /2, MAX(landmarks[37].y, landmarks[38].y)));
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[43].x + landmarks[44].x) /2, MAX(landmarks[43].y,landmarks[44].y)));
    
    ofSetColor(ofColor::black);
    ofVec2f a(landmarks[27].x, landmarks[27].y - (landmarks[51].y - landmarks[27].y));
    FontUtil::draw("A", font, FontUtil::ABOVE, a);
    ofVec2f b(landmarks[27].x, landmarks[8].y );
    FontUtil::draw("B", font, FontUtil::BELOW, b);
    FontUtil::draw("C", font, FontUtil::BELOW, ofVec2f(landmarks[17].x, landmarks[17].y - (landmarks[2].y - landmarks[0].y)));
    FontUtil::draw("C'", font, FontUtil::BELOW, ofVec2f(landmarks[26].x, landmarks[26].y -(landmarks[14].y - landmarks[16].y)));
    FontUtil::draw("E", font, FontUtil::RIGHT, landmarks[21]);
    FontUtil::draw("E'", font, FontUtil::LEFT, landmarks[22]);
    FontUtil::draw("F", font, FontUtil::LEFT, landmarks[17]);
    FontUtil::draw("F'", font, FontUtil::RIGHT, landmarks[26]);
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[40].x + landmarks[41].x) /2, MIN(landmarks[40].y, landmarks[41].y)));
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[46].x + landmarks[47].x) /2, MIN(landmarks[46].y, landmarks[47].y)));
    FontUtil::draw("J", font, FontUtil::LEFT, landmarks[31]);
    FontUtil::draw("J'", font, FontUtil::RIGHT, landmarks[35]);
    FontUtil::draw("H'", font, FontUtil::RIGHT, landmarks[45]);
    ofDrawLine( a, b);
}

void Mugshot::drawLettersProfile(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font) {
    ofSetColor(ofColor::white);
    FontUtil::draw("A", font, FontUtil::LEFT, ofVec2f(landmarks[27].x, landmarks[27].y - (landmarks[51].y - landmarks[27].y)));
    FontUtil::draw("D", font, FontUtil::LEFT, landmarks[31]);
    FontUtil::draw("G'", font, FontUtil::LEFT, landmarks[42]);
    FontUtil::draw("B", font, FontUtil::ABOVE, landmarks[22]);
    FontUtil::draw("H", font, FontUtil::RIGHT, landmarks[45]);
    
    ofSetColor(ofColor::black);
    FontUtil::draw("F", font, FontUtil::ABOVE, landmarks[8]);
    FontUtil::draw("E", font, FontUtil::RIGHT, landmarks[35]);
    FontUtil::draw("L", font, FontUtil::ABOVE, ofVec2f((landmarks[43].x + landmarks[44].x) /2, MAX(landmarks[43].y, landmarks[44].y)));
    FontUtil::draw("I'", font, FontUtil::ABOVE, ofVec2f(landmarks[45].x, landmarks[29].y));
    
   // ofSetLineWidth(1);
    ofDrawLine(landmarks[15], landmarks[45]);
    ofDrawLine(landmarks[15], ofVec2f(landmarks[45].x, landmarks[29].y));
    ofSetColor(ofColor::white);
}

void Mugshot::drawDottedLine(ofVec2f start, ofVec2f end) {
    /* select black for all lines  */
    ofSetLineWidth(4);
    glColor3f (0.0, 0.0, 0.0);
    
    glEnable (GL_LINE_STIPPLE);
    
    glLineStipple (4, 0xAAAA);  /*  first parameter set the dash length in px  */
    glBegin(GL_LINES);
    glVertex2f (start.x, start.y); /* starting points */
    glVertex2f (end.x, end.y); /* end points */
    glEnd();
    glDisable (GL_LINE_STIPPLE);
    glFlush ();
    
    //ofPopMatrix();
}

void Mugshot::draw(int x, int y) {
    fbo.draw(x, y);
}
