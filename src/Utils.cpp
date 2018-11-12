//
//  Utils.cpp
//  TNM2
//
//  Created by inx on 12.11.18.
//

#include "Utils.hpp"
#include "ofImage.h"

std::string meterToCMDashMM(float meter) {
	
	float height = 1.234567;
	if (height > 1.0) {
		height -= 1.0;
	}
	std::string hs = ofToString(height);
	return hs.substr(2, 2)+"-"+hs.substr(4, 1);
}
