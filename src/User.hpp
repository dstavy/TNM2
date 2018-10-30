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
    int measures[5];
    bool currentUser = false;
    
    User(string id) {
        this->id = id;
    };
    
    View& getView(bool profile) {
        if(profile) return profileView;
        else return frontView;
    };
};

typedef map<string, User*> UserMap;

#endif /* User_hpp */
