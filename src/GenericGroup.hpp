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
    GenericGroup(View::Features feature, GroupBy grouBy, int levels) : Group(feature, grouBy, levels ) {};
    void update(vector<User*> newUsers) override;
    ~GenericGroup() {};
};

#endif /* GenericGroup_hpp */
