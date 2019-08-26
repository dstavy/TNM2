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
	
    class GroupFilter {
        public:
        Gender gender = None;
        int minAge = -1;
        int maxAge = -1;
        bool beard = false;
        string hairColor = "";
        
        bool inFilter(User* user) {
            if (this->gender != None && this->gender != user->gender) {
                 return false;
            }
            
            if (this->beard == true && user->getBeardBoolean() == false)  {
                return false;
            }
            
            if (this->minAge != -1 && user->age <  this->minAge)  {
                 return false;
            }
            
            if (this->maxAge != -1 && user->age >  this->maxAge)  {
                return false;
            }
            if (!this->hairColor.empty() && this->hairColor.compare(user->hairColor) != 0)  {
                return false;
            }
            return true;// put your condition here
        };
    };
    
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
		
		//users = new vector<User*>[numLevels];
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
	/*
	void clear() {
		for (int i = 0; i < numLevels; i++) {
			users[i].clear();
			//levelScores[i] = 0.;
		}
	};
	*/
	View::Features getFeature() {
		return feature;
	}
    
    GroupFilter getFilter()
    {
        return filter;
    }
	
    //static const int NUM_LEVELS = 5;
    View::Features feature;
    GroupBy grouBy;
    int numLevels;
    bool profile;
	//vector<User*>* users;
    vector<User*> allUsers;
    //float* levelScores;
    GroupFilter filter;
    void sortUsersByAge();
    void filterGroup(GroupFilter& filter);
};

#endif /* Group_hpp */
