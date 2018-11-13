//
//  Group.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "Group.hpp"

User* Group::getGridUsers(int usersPerLevel, vector<User*>& outUsers, int& outIndex) {
   /* bool currentFound = false;
    
    for (int i = 0; i < numLevels; i++) {
        // if empty level leave empty
        if (users[i].empty()) {
            for(int k = 0; k < usersPerLevel; k++) {
                outUsers.push_back(nullptr);
            }
        }
        else {
            float ratio = (float)users[i].size() / usersPerLevel - 0.001;// so we dont get to index plus one;
            int index = 0;
            int nextIndex = 0;
            for(int j = 0; j < usersPerLevel; j++) {
                int nextIndex = floor(ratio * (j + 1));
                if (nextIndex <= index) {
                    nextIndex = MIN(index+1, users[i].size() -1);
                }
                bool put = false;
                if (!currentFound) {
                    for (int k = index +1; k < nextIndex; k++) {
                        if (users[i].at(k)->currentUser) {
                            outUsers.push_back(users[i].at(k));
                            currentFound = true;
                            put = true;
                        }
                    }
                }
                if (!put) {
                    outUsers.push_back(users[i].at(index));
                    if (users[i].at(index)->currentUser) {
                        currentFound = true;
                    }
                }
                index = nextIndex;
            }
        }
    }
    */
	
	// do a simple colection based on number of users
    int numberNeeded = numLevels * usersPerLevel;
    //float ratio = (float)allUsers.size() / (numberNeeded - 1);
    float increment = 1.0/(numberNeeded -1);
    
    for (int j =0; j <numberNeeded; j++) {
        //ofLerp(0, allUsers.size(), j*increment);
        outUsers.push_back(allUsers[round(ofLerp(0, allUsers.size() -1, j*increment))]);
    }
	
	
    // Iterate over all elements in Vector
	bool found = false;
    int i = 0;
    for(; i < allUsers.size(); i++)
    {
        if (allUsers[i]->isCurrent)
        {
            found = true;
            break;
        }
    }
	
    if(found) { // add current User
		int index = round(ofMap(i, 0, allUsers.size() -1, 0, numberNeeded - 1));
		outUsers[index] = allUsers[i];
		outIndex = index;
		return allUsers[i];
    }
	
	outIndex = -1;
	return nullptr;
}

float Group::getScore(User* user) {
    for (int i = 0; i < numLevels; i++) {
        if (std::find(users[i].begin(), users[i].end(), user) != users[i].end()) {
            return levelScores[i];
        }
    }
    return 0.0;
}
