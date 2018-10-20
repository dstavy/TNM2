//
//  GenericGroup.cpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#include "GenericGroup.hpp"

void GenericGroup::update(vector<User*> newUsers) {
    Group::clear();
    for(auto const& user: newUsers) {
        int level = floor(user->score/ (1.0/NUM_LEVELS));
        users[level].push_back(user);
        levelScores[level] += user->score;
    }
    
    for (int i = 0; i < NUM_LEVELS; i++) {
        levelScores[i] / users[i].size();
    }
}
