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


static const float BEARD_THRESHOLD = 0.5;
static const float BALD_THRESHOLD = 0.65;

class User {
public:
    static float highestScore;
    static float lowestScore;
    string id;
    float selectionScore;
    float score;
    int vScore;
    int xScore;
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
    User(string id) {
        this->id = id;
    };
    
    bool getBeardBoolean() {
        if (beard > BEARD_THRESHOLD) {
            return true;
        }
        return false;
    };
    
    static void setHighestScore(float score) {
        highestScore  = score;
    };
    
    static void setLowestScore(float score) {
        lowestScore  = score;
    };
    
    float getFactrorScore() {
        return (ofMap(score, User::lowestScore, User::highestScore, 0., 1.));
    };
        
        
    View& getView(bool profile) {
        if(profile) return profileView;
        else return frontView;
    };
};

typedef map<string, User*> UserMap;

#endif /* User_hpp */
