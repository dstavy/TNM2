//
//  GroupManager.cpp
//  TNM2
//
//  Created by Dan Stavy on 17/10/2018.
//

#include "GroupManager.hpp"
#include "AgeGroup.hpp"

void GroupManager::update(vector<User*>& users) {
    for(auto const& group: groups) {
        group->update(users);
    }
}

void GroupManager::exit() {
    for(auto const& group: groups) {
        delete(group);
    }
}

Group* GroupManager::getGroup(View::Features feature, Group::GroupBy by, bool profile) {
	
    for(auto const& group: groups) {
        if ((group->grouBy == by) && (group->feature == feature ) && (group->profile == profile )) {
            return group;
        }
    }
	
	return NULL;
}

Group* GroupManager::groupFactory(View::Features feature, Group::GroupBy by, bool profile, int levels, GenericGroup::GroupFilter& filter) {
  /*
    Group* group = getGroup(feature, by, profile);
    if (group != NULL) {
        return group;
    }
  */
	// create new group
    Group* g = NULL;
	switch ((int)by) {
        case Group::AGE:
             g  = new AgeGroup(feature, by, profile, levels, filter);
            break;
        case Group::GENERIC:
            default:
           g  = new GenericGroup(feature, by, profile, levels, filter);
            break;
        }
        groups.push_back(g);
        return g;

	
	ofLogNotice() << "did not create group for :" << by;
    return NULL;
}
