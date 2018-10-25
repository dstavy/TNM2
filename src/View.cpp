//
//  View.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "View.hpp"
#include "FontUtil.hpp"

void View::drawLettersFront() {
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("BodonPoster");
    ofSetColor(ofColor::white);
    FontUtil::draw("D", font, FontUtil::CENTER, ofVec2f(landmarks[0].x, landmarks[19].y - parts[RIGHT_EAR].height));
    FontUtil::draw("D'", font, FontUtil::CENTER, ofVec2f(landmarks[16].x, landmarks[24].y - parts[LEFT_EAR].height));
    FontUtil::draw("I", font, FontUtil::CENTER, landmarks[1]);
    FontUtil::draw("I'", font, FontUtil::CENTER, landmarks[15]);
    FontUtil::draw("K", font, FontUtil::LEFT, landmarks[3]);
    FontUtil::draw("K'", font, FontUtil::RIGHT, landmarks[13]);
    FontUtil::draw("L", font, FontUtil::CENTER, landmarks[48]);
    FontUtil::draw("L'", font, FontUtil::CENTER, landmarks[54]);
    FontUtil::draw("G", font, FontUtil::CENTER, landmarks[39]);
    FontUtil::draw("G'", font, FontUtil::CENTER, landmarks[42]);
    FontUtil::draw("H", font, FontUtil::CENTER, landmarks[36]);
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[37].x + landmarks[38].x) /2, (landmarks[37].x + landmarks[38].x) /2));
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[43].x + landmarks[44].x) /2, (landmarks[43].x + landmarks[44].x) /2));

    ofSetColor(ofColor::black);
    FontUtil::draw("A", font, FontUtil::ABOVE, ofVec2f(landmarks[27].x, landmarks[27].y - (landmarks[33].y - landmarks[27].y)));
    FontUtil::draw("B", font, FontUtil::BELOW, ofVec2f(landmarks[8].x, landmarks[8].y + (landmarks[57].y - landmarks[8].y)));
    FontUtil::draw("C", font, FontUtil::CENTER, ofVec2f(landmarks[17].x, landmarks[17].y - parts[RIGHT_EAR].height));
    FontUtil::draw("C'", font, FontUtil::CENTER, ofVec2f(landmarks[26].x, landmarks[26].y - parts[LEFT_EAR].height));
    FontUtil::draw("E", font, FontUtil::CENTER, landmarks[21]);
    FontUtil::draw("E'", font, FontUtil::CENTER, landmarks[22]);
    FontUtil::draw("F", font, FontUtil::CENTER, landmarks[17]);
    FontUtil::draw("F'", font, FontUtil::CENTER, landmarks[26]);
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[40].x + landmarks[41].x) /2, (landmarks[40].x + landmarks[41].x) /2));
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[46].x + landmarks[47].x) /2, (landmarks[46].x + landmarks[47].x) /2));
    FontUtil::draw("J", font, FontUtil::CENTER, landmarks[31]);
    FontUtil::draw("J'", font, FontUtil::CENTER, landmarks[35]);
    FontUtil::draw("H'", font, FontUtil::CENTER, landmarks[45]);
}

void View::drawLettersProfile() {
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("BodonPoster");
    ofSetColor(ofColor::white);
    FontUtil::draw("D", font, FontUtil::CENTER, ofVec2f(landmarks[0].x, landmarks[19].y - parts[RIGHT_EAR].height));
    FontUtil::draw("D'", font, FontUtil::CENTER, ofVec2f(landmarks[16].x, landmarks[24].y - parts[LEFT_EAR].height));
    FontUtil::draw("I", font, FontUtil::CENTER, landmarks[1]);
    FontUtil::draw("I'", font, FontUtil::CENTER, landmarks[15]);
    FontUtil::draw("K", font, FontUtil::LEFT, landmarks[3]);
    FontUtil::draw("K'", font, FontUtil::RIGHT, landmarks[13]);
    FontUtil::draw("L", font, FontUtil::CENTER, landmarks[48]);
    FontUtil::draw("L'", font, FontUtil::CENTER, landmarks[54]);
    FontUtil::draw("G", font, FontUtil::CENTER, landmarks[39]);
    FontUtil::draw("G'", font, FontUtil::CENTER, landmarks[42]);
    FontUtil::draw("H", font, FontUtil::CENTER, landmarks[36]);
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[37].x + landmarks[38].x) /2, (landmarks[37].x + landmarks[38].x) /2));
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[43].x + landmarks[44].x) /2, (landmarks[43].x + landmarks[44].x) /2));
    
    ofSetColor(ofColor::black);
    FontUtil::draw("A", font, FontUtil::ABOVE, ofVec2f(landmarks[27].x, landmarks[27].y - (landmarks[33].y - landmarks[27].y)));
    FontUtil::draw("B", font, FontUtil::BELOW, ofVec2f(landmarks[8].x, landmarks[8].y + (landmarks[57].y - landmarks[8].y)));
    FontUtil::draw("C", font, FontUtil::CENTER, ofVec2f(landmarks[17].x, landmarks[17].y - parts[RIGHT_EAR].height));
    FontUtil::draw("C'", font, FontUtil::CENTER, ofVec2f(landmarks[26].x, landmarks[26].y - parts[LEFT_EAR].height));
    FontUtil::draw("E", font, FontUtil::CENTER, landmarks[21]);
    FontUtil::draw("E'", font, FontUtil::CENTER, landmarks[22]);
    FontUtil::draw("F", font, FontUtil::CENTER, landmarks[17]);
    FontUtil::draw("F'", font, FontUtil::CENTER, landmarks[26]);
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[40].x + landmarks[41].x) /2, (landmarks[40].x + landmarks[41].x) /2));
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[46].x + landmarks[47].x) /2, (landmarks[46].x + landmarks[47].x) /2));
    FontUtil::draw("J", font, FontUtil::CENTER, landmarks[31]);
    FontUtil::draw("J'", font, FontUtil::CENTER, landmarks[35]);
    FontUtil::draw("H'", font, FontUtil::CENTER, landmarks[45]);
}

/*
D    [ 1 , (1-(3-1)) ]
D'   [ 17, (17-(15-17)) ]
I    2
I'   16
K    4
K'   14
L    49
L'   55
J    32
J'   36
G    40
G'   43
H    37
H'   46
E    22
E'   23
F    18
F'   27
A    [ 28 , (28-(34-28)) ]
B    [ 28 , (34 +2*(34-28) ]
             O    above 38-39
             O    under 42-42
             O    above 44-45
             O    under 48-47
*/
