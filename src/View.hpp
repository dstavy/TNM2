//
//  View.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef View_hpp
#define View_hpp

#include "ofMain.h"
#include "ofxFaceTracker2.h"

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
       // SAVED_IMAGE
    };
    
    static inline const string featureToString(Features f)
    {
        switch (f)
        {
            case HEAD:      return "Visage";
            case FORHEAD:   return "Front";
            case EYES:      return "Yeux";
            case NOSE:      return "Nez";
            case MOUTH:     return "Bouche";
            case LEFT_EAR:
            case RIGHT_EAR:
                            return "Oreille";
            default:        return "";
        }
    }
    
    static inline const string proflieToString(bool p)
    {
        if (p) {
            return "Profil";
        } else {
            return "Frontale";
        }
    }
    
    struct Letter {
        string s;
        int color;
    };
    /*
    const map<int, Letter> pointToLetterFront = {
        {1,  {"D", 0 },
        {16,  {"D'", 0 },
        {2,  {"I", 0 },
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
};
     */
    static const int FACE_EXTENDED_PADDING = 150;
    static const int LANDMARKS_NUM = 5;
    static const int PARTS_NUM = 7;
    static const string FACES_DIR ;
    //ofxFaceTracker2Landmarks landmarks2;
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
