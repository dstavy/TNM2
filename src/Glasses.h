//
//  Glasses.h
//  TNM2
//
//  Created by Dan Stavy on 31/08/2019.
//

#ifndef Glasses_h
#define Glasses_h
#include <string>


enum Glasses {
    DontCare,
    On,
    Off
};

Glasses getGlassesFromString(std::string isGlasses);

Glasses getGlassesFromBoolean(bool isGlasses);
bool getBooleanFromGlasses(Glasses glasses);

#endif /* Glasses_h */
