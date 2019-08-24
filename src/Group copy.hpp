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

class GroupFilter {
    Gender gender;
    int mimAge = -1;
    int maxAge = -1;
    bool beard;
};

class Group {
public:
	
    enum GroupBy {
        GENERIC = 0,// does nothing
        AGE
    };
	
	Group(View::Features feature,
		  GroupBy groupBy,
		  bool profile,
		  int levels,
          GroupFilter& filter)
	{
		this->feature = feature;
		this->grouBy = groupBy;
		this->profile = profile;
		this->numLevels = levels;
        this->filter = filter;
		
		users = new vector<User*>[numLevels];
		//levelScores = new float[numLevels];
	};
	
	virtual ~Group() {
//		delete(users);
	//	delete(levelScores);
	};
	
	/*
	float getScore(int level) {
		return levelScores[level];
	};
     */
    
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
			//levelScores[i] = 0.;
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
    //float* levelScores;
    GroupFilter filter;Z
};

#endif /* Group_hpp */
