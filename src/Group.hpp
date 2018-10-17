//
//  Group.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef Group_hpp
#define Group_hpp

#include "ofMain.h"
#include "User.hpp"
#include "View.hpp"

class Group {
public:
    enum GroupBy {
        GENERIC = 0,// does nothing
        COLOR,
        AGE,
        GENDER
    };
    static const int NUM_LEVELS = 4;
    View::Features feature;
    GroupBy grouBy;
    int scores[NUM_LEVELS];
    int getLevel(User* user);
    float getScore(int level);
    float getScore(User* user);
    vector<User*> users[NUM_LEVELS];
    virtual void update(vector<User*> users);
    virtual vector<User*> getGridUser(int num);
    virtual void setup();
};

#endif /* Group_hpp */
