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
	
	Group(View::Features feature, GroupBy grouBy, bool profile, int levels)
	{
		this->feature = feature;
		this->grouBy = grouBy;
		this->profile = profile;
		this->numLevels = levels;
		
		users = new vector<User*>[numLevels];
		levelScores = new float[numLevels];
	};
	
	virtual ~Group() {
//		delete(users);
//		delete(levelScores);
	};
	
	
	float getScore(int level) {
		return levelScores[level];
	};

	//int getLevel(User* user);
	float getScore(User* user);
	
	virtual void update(vector<User*>& newUsers) {
		allUsers = newUsers;
	};
	
	// returns current user
	User* getGridUsers(int usersPerLevel, vector<User*>& outUsers, int& outIndex);
	
	void clear() {
		for (int i = 0; i < numLevels; i++) {
			users[i].clear();
			levelScores[i] = 0.;
		}
	};
	
	View::Features getFeature() {
		return feature;
	}
	
    //static const int NUM_LEVELS = 5;
    View::Features feature;
    GroupBy grouBy;
    int numLevels;
    bool profile;
	vector<User*>* users;
    vector<User*> allUsers;
    float* levelScores;
};

#endif /* Group_hpp */
