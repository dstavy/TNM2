//
//  FontUtil.cpp
//  TNM2
//
//  Created by Dan Stavy on 25/10/2018.
//

#include "FontUtil.hpp"

void FontUtil::draw(string text, shared_ptr<ofxSmartFont> font, Align align, ofVec2f p){
    int w = font->width(text);
    int h = font->height(text);
    
    switch((int)align) {
        case CENTER:
            font->draw(text, p.x -w/2, p.y + h /2);
            return;
        case LEFT:
            font->draw(text, p.x -w, p.y + h /2);
            return;
        case RIGHT:
            font->draw(text, p.x, p.y + h /2);
            return;
        case ABOVE:
            font->draw(text, p.x -w/2, p.y - h);
            return;
        case BELOW:
            font->draw(text, p.x -w/2, p.y);
            return;
    }
}
