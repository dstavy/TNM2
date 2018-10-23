//
//  Group.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "Group.hpp"

void Group::getGridUsers(int usersPerLevel, vector<User*>& outUsers) {
    bool currentFound = false;
    
    for (int i = 0; i < numLevels; i++) {
        // if empty level leave empty
        if (users[i].empty()) {
            for(int k = 0; k < usersPerLevel; k++) {
                outUsers.push_back(nullptr);
            }
        }
        else {
            float ratio = (float)users[i].size() / usersPerLevel;
            int index = 0;
            int nextIndex = 0;
            for(int j = 0; j < usersPerLevel; j++) {
                int nextIndex = floor(ratio * j);
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
}

float Group::getScore(User* user) {
    for (int i = 0; i < numLevels; i++) {
        if (std::find(users[i].begin(), users[i].end(), user) != users[i].end());
        return levelScores[i];
    }
    return 0.0;
}
