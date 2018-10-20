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
#include "GenericGroup.hpp"
#include "View.hpp"

class GroupManager {
private:
    vector<Group*>groups;
    vector<User*> users;
public:
    void update(vector<User*> users);
    void exit();
    bool groupFactory(View::Features feature, Group::GroupBy by);
    Group* getGroup(View::Features feature, Group::GroupBy by);
};
    

#endif /* GroupManager_hpp */
