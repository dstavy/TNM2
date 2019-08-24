//
//  Gender.cpp
//  TNM2
//
//  Created by Dan Stavy on 23/08/2019.
//

#include "Gender.h"

Gender getGenderFromString(std::string genderString) {
    Gender gender = None;
    if(genderString.compare("male") == 0) {
        gender = Male;
    } else if(genderString.compare("female") == 0) {
        gender = Female;
    }
    return gender;
}

std::string getStringFromGender(Gender gender)
{
    switch(gender) {
        case Male:
            return "male";
        case Female:
            return "female";
        default:
            return "";
    }
}

