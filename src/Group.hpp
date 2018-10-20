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
    static const int NUM_LEVELS = 5;
    View::Features feature;
    GroupBy grouBy;
    //int getLevel(User* user);
    float getScore(int level) {
        return levelScores[level];
    };
    float getScore(User* user);
    vector<User*> users[NUM_LEVELS];
    float levelScores[NUM_LEVELS];
    virtual void update(vector<User*> newUsers) = 0;
    void getGridUsers(int usersPerLevel, vector<User*>& outUsers);
    Group(View::Features feature, GroupBy grouBy)
    {
        this->feature = feature;
        this->grouBy = grouBy;
    };
    
    virtual ~Group() = default;
    
    void clear() {
        for (int i = 0; i < NUM_LEVELS; i++) {
            users[i].clear();
            levelScores[i] = 0.;
        }
    };
};

#endif /* Group_hpp */
