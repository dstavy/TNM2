//
//  FontUtil.hpp
//  TNM2
//
//  Created by Dan Stavy on 25/10/2018.
//

#ifndef FontUtil_hpp
#define FontUtil_hpp

#include "ofMain.h"
#include "ofxSmartFont.h"

class FontUtil {
    public:
    enum Align {
        CENTER = 0,
        LEFT,
        RIGHT,
        ABOVE,
        BELOW,
        BELOW_RIGHT,
        BELOW_LEFT
    };
    static void draw(string text, shared_ptr<ofxSmartFont> font, Align align, ofVec2f p);
};
#endif /* FontUtil_hpp */
