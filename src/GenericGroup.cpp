//
//  GenericGroup.cpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#include "GenericGroup.hpp"

void GenericGroup::update(vector<User*>& newUsers) {
	
    Group::clear();
    Group::update(newUsers);
	
 //   cout << User::highestScore << " " << User::lowestScore <<"\n";
    for(auto const& user: newUsers) {
      // if (user->getView(!profile).isActive()) {
			int level = floor(user->getFactrorScore() / (1.0/numLevels));
            users[level].push_back(user);
            levelScores[level] += user->getFactrorScore();
		
   			cout<< user->score << " " <<user->getFactrorScore() << " " << level <<"\n";
      //  }
    }

    for (int i = 0; i < numLevels; i++) {
        levelScores[i] /= users[i].size();
    }
}
