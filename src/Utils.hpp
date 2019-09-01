//
//  Utils.hpp
//  TNM2
//
//  Created by inx on 12.11.18.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <string>
#include "User.hpp"

std::string meterToCMDashMM(float meter);
bool saveUsersToJson(UserMap* users);

#endif /* Utils_hpp */
