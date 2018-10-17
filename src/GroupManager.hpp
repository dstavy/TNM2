//
//  GroupManager.hpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#ifndef GroupManager_hpp
#define GroupManager_hpp

#include "ofMain.h"
#include "Group.hpp"
#include "View.hpp"


class GroupManager {
public:
    vector<Group*>groups;
    Group* groupFactory(View::Features features, Group::GroupBy by) {
        // TODO::
        return NULL;
    };
    
    Group* getGroup(View::Features features, Group::GroupBy by) {
        // TODO::
        return NULL;
    };
};
    

#endif /* GroupManager_hpp */
