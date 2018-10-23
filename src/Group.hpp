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
    //static const int NUM_LEVELS = 5;
    View::Features feature;
    GroupBy grouBy;
    int numLevels;
    bool profile;
    //int getLevel(User* user);
    float getScore(int level) {
        return levelScores[level];
    };
    float getScore(User* user);
    vector<User*>* users;
    float* levelScores;
    virtual void update(vector<User*> newUsers) = 0;
    void getGridUsers(int usersPerLevel, vector<User*>& outUsers);
    Group(View::Features feature, GroupBy grouBy, bool profile, int levels)
    {
        this->feature = feature;
        this->grouBy = grouBy;
        this->numLevels = levels;
        this->profile = profile;
        users = new vector<User*>[numLevels];
        levelScores = new float[numLevels];
    };
    
    virtual ~Group() {
        //delete(users);
        //delete(levelScores);
    };
    
    void clear() {
        for (int i = 0; i < numLevels; i++) {
            users[i].clear();
            levelScores[i] = 0.;
        }
    };
};

#endif /* Group_hpp */
