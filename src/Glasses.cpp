//
//  Glasses.cpp
//  TNM2
//
//  Created by Dan Stavy on 01/09/2019.
//
#include "Glasses.h"

Glasses getGlassesFromString(std::string isGlasses) {
    if (isGlasses.compare("NoGlasses") == 0) {
        return Off;
    }
    return On;
};

Glasses getGlassesFromBoolean(bool isGlasses) {
    if (isGlasses) {
        return On;
    }
    return Off;
};

bool getBooleanFromGlasses(Glasses glasses) {
    if (glasses == Glasses::On) {
        return true;
    }
    return false;
};
