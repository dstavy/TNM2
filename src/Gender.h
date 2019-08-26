//
//  Gender.h
//  TNM2
//
//  Created by Dan Stavy on 22/08/2019.
//

#ifndef Gender_h
#define Gender_h
#include <string>

enum Gender {
    None = 0,
    Male,
    Female
};

Gender getGenderFromString(std::string genderString);
std::string getStringFromGender(Gender gender);

#endif /* Gender_h */
