//
//  GroupManager.cpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#include "GroupManager.hpp"



void GroupManager::update(vector<User*> users) {
    for(auto const& group: groups) {
        group->update(users);
    }
}

void GroupManager::exit() {
    for(auto const& group: groups) {
        delete(group);
    }
}

Group* GroupManager::getGroup(View::Features feature, Group::GroupBy by) {
    for(auto const& group: groups) {
        if ((group->grouBy == by) && (group->feature == feature )) {
            return group;
        }
        return NULL;
    }
}

Group* GroupManager::groupFactory(View::Features feature, Group::GroupBy by, int levels) {
  
    Group* group = getGroup(feature, by);
    if (group != NULL) {
        return group;
    }
    else {
        switch ((int)by) {
            case Group::GENERIC:
                Group* g  = new GenericGroup(feature, by, levels);
                groups.push_back(g);
                return g;
        }
    }
    return NULL;
}

