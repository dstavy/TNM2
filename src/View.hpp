//
//  View.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef View_hpp
#define View_hpp

#include "ofMain.h"

class View {
public:
    enum Features {
        HEAD = 0,
        FORHEAD,
        EYES,
        NOSE,
        MOUTH,
        LEFT_EAR,
        RIGHT_EAR,
        SAVED_IMAGE
    };
    
    static const int LANDMARKS_NUM = 5;
    static const int PARTS_NUM = 7;
    static const string FACES_DIR ;
    static const glm::vec4 partPeddings[PARTS_NUM];
    char letter[LANDMARKS_NUM];
    ofRectangle parts[PARTS_NUM];
    ofPoint landmarks[LANDMARKS_NUM];
    ofImage face;
    bool profile;
    
    ofImage& getImage() {
        return face;
    }
    
    ofRectangle& getBounderyBox(Features feature) {
        return parts[feature];
    }
};

#endif /* View_hpp */
