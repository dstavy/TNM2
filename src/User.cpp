//
//  User.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "User.hpp"

Json::Value User::toJson()
{
    Json::Value v;
 
    v["id"] = id;
    v["score"] = score;
    v["rounds"] = rounds;
    v["timestamp"] = timestamp;
    v["totalHeight"] = totalHeight;
    v["headHeight"] = headHeight;
    v["torsoLength"] = torsoLength;
    v["shouldersWidth"] = shouldersWidth;
    v["armLength"] = armLength;
    v["age"] = age;
    v["gender"]  = getStringFromGender(gender);
    v["hairColor"]  = hairColor;
    v["beard"] = beard;
    v["glasses"] = getBooleanFromGlasses(glasses);
    
    return v;
}

