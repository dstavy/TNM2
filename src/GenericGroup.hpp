//
//  GenericGroup.hpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#ifndef GenericGroup_hpp
#define GenericGroup_hpp

#include "ofMain.h"
#include "Group.hpp"

class GenericGroup : public Group {
public:
    GenericGroup(View::Features feature,
				 GroupBy groupBy,
				 bool profile,
				 int levels,
                 GroupFilter& filter) : Group(feature, groupBy, profile, levels, filter )
	{};
	~GenericGroup() {};
	
    void update(vector<User*>& newUsers) override;
};

#endif /* GenericGroup_hpp */
