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
	GroupManager() {}
	~GroupManager() {
		// TODO:
		// cleanup groups we created!		
	}
	
    void update(vector<User*> users);
    void exit();
    Group* groupFactory(View::Features feature, Group::GroupBy by, bool profile, int levels);
    Group* getGroup(View::Features feature, Group::GroupBy by, bool profile);
};
    

#endif /* GroupManager_hpp */
