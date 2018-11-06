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
	float totalHeight;
    View frontView;
    View profileView;
    //int measures[5];
    bool isCurrent = false;
    
    User(string id) {
        this->id = id;
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
