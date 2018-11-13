//
//  View.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef View_hpp
#define View_hpp

#include <string>

#include "ofImage.h"
#include "ofRectangle.h"
#include "ofxFaceTracker2.h"

using namespace std;

static float gridlocation[] = {-450.0, -400.0, -450.0, -30.0, -1300.0, -300.0, 0.0};

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
		INVALID,
    };
    
    //View() {};
    
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
    
    static constexpr float FACE_EXTENDED_PADDING = 1.3;
    static const int LANDMARKS_NUM = 5;
    static const int PARTS_NUM = 7;
    static const string FACES_DIR ;
    static const glm::vec4 partPeddings[PARTS_NUM];
	
	static const float getLocationForFeature(Features f) {
		return gridlocation[f];
	}
	
	//ofxFaceTracker2Landmarks landmarks2;
    char letter[LANDMARKS_NUM];
    ofRectangle parts[PARTS_NUM];
    ofImage face;
    bool profile;
	
#ifdef TARGET_OSX
    vector<ofVec2f> landmarks;
    
    vector<ofVec2f>& getLandmarks() {
        return landmarks;
    };
#elif defined TARGET_WIN32
    vector<glm::vec2> landmarks;
    
    vector<glm::vec2>& getLandmarks() {
        return landmarks;
    };
#endif
    
    bool active = false;
    
    ofImage& getImage() {
        return face;
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
