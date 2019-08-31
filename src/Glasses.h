//
//  Glasses.h
//  TNM2
//
//  Created by Dan Stavy on 31/08/2019.
//

#ifndef Glasses_h
#define Glasses_h

enum Glasses {
    DontCare,
    On,
    Off
};

Glasses getGlassesFromString(string isGlasses) {
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


#endif /* Glasses_h */
