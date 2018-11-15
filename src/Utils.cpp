//
//  Utils.cpp
//  TNM2
//
//  Created by inx on 12.11.18.
//

#include "Utils.hpp"
#include "ofImage.h"

std::string meterToCMDashMM(float meter) {
	
	float height = meter;
	if (height > 1.0) {
		height -= 1.0;
	}
	
	std::string hs = ofToString(height);
	
	if (hs == "0") {
		return "";
	}
	
	if (hs.length() > 4) {
		return hs.substr(2, 2)+"-"+hs.substr(4, 1);
	} else {
		
	}
	
	return hs;
	
}
