//
//  User.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef User_hpp
#define User_hpp

#include "ofMain.h"
#include "View.hpp"
#include "Gender.h"
#include "Glasses.h"


static const float BEARD_THRESHOLD = 0.5f;
static const float BALD_THRESHOLD = 0.65f;

class User {
public:
    string id;
    float score = 0.0;
    int rounds = 0;
    string timestamp = "";
	int shouldersWidth;
	float torsoLength;
	float lowerArm = 0.0;
	float totalHeight;
	float headHeight;

    View frontView;
    View profileView;
    //int measures[5];
    bool isCurrent = false;
    int age = 0;
    Gender gender = None;
    string hairColor;
    float beard = 0.0;
    Glasses glasses = DontCare;
    
    User(string id) {
        this->id = id;
    };
    
    bool getBeardBoolean() {
        if (beard > BEARD_THRESHOLD) {
            return true;
        }
        return false;
    };
        
    View& getView(bool profile) {
        if(profile) return profileView;
        else return frontView;
    };
};

typedef map<string, User*> UserMap;

#endif /* User_hpp */
