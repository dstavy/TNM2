//
//  GenericGroup.hpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#ifndef AgeGroup_hpp
#define AgeGroup_hpp

#include "ofMain.h"
#include "Group.hpp"
#include "Gender.h"

class AgeGroup : public Group {
public:
    AgeGroup(View::Features feature,
				 GroupBy groupBy,
				 bool profile,
				 int levels,
                 GroupFilter& filtere) : Group(feature, groupBy, profile, levels, filter)
	{};
	~AgeGroup() {};
	
    void update(vector<User*>& newUsers) override;
    Gender gender = None;
};

#endif /* GendrGroup_hpp */
