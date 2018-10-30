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
        NOSE,
        MOUTH,
        EYES,
        LEFT_EAR,
        RIGHT_EAR,
    };
    /*
    View(bool profile) {
        this->profile = profile;
    };
    */
    static inline const string featureToString(Features f)
    {
        switch (f)
        {
            case HEAD:      return "Face";// return "Visage";
            case FORHEAD:   return "Forehead";// return "Front";
            case EYES:      return "Eyes";// return "Yeux";
            case NOSE:      return "Nose";// return "Nez";
            case MOUTH:     return "Mouth";// return "Bouche";
            case LEFT_EAR:
            case RIGHT_EAR: 
                            return "Ear";
            default:        return "";
        }
    };
    
    static inline const string proflieToString(bool p)
    {
        if (p) {
            return "Side";
        } else {
            return "Front";
        }
    };
   
    static const int FACE_EXTENDED_PADDING = 150;
    static const int LANDMARKS_NUM = 5;
    static const int PARTS_NUM = 7;
    static const string FACES_DIR ;
    //ofxFaceTracker2Landmarks landmarks2;
    static const glm::vec4 partPeddings[PARTS_NUM];
    char letter[LANDMARKS_NUM];
    ofRectangle parts[PARTS_NUM];
    ofImage face;
    bool profile;
	vector<glm::vec2> landmarks;
    bool active = false;
    
    ofImage& getImage() {
        return face;
    };
    
	vector<glm::vec2>& getLandmarks() {
        return landmarks;
    };
    
    bool loadImage(string path) {
        bool ret = face.load(path);
        if (ret)
        {
            face.update();
        }
        return ret;
    };
    
    ofRectangle& getBounderyBox(Features feature) {
        return parts[feature];
    };
    
    bool isActive()
    {
        return active;
    };
    
    void setActive(bool active)
    {
        this->active = active;
    };
};

#endif /* View_hpp */
