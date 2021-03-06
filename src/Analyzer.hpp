//
//  VideoTracker.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef Analyzer_hpp
#define Analyzer_hpp

#include "ofMain.h"
#include "ofxFaceTracker2.h"
#include "View.hpp"
#include "User.hpp"

// this class check the videos. add a new user and update scores
class Analyzer {
public:
    static const int EAR_ROTATION_FACTOR = 5;
	static const int  NO_FOUND = std::numeric_limits<float>::min();
    static bool videoAnalyze(string fileName, ofVideoPlayer& player, ofxFaceTracker2& tracker, User& user, bool profile, string outImage);
    static bool faceAnalyze(string fileName, ofxFaceTracker2& tracker, User& user, bool profile);
    static float getFaceScore(ofImage& image,ofxFaceTracker2& tracker, bool profile);
private:
    static bool faceInflate(ofxFaceTracker2& tracker, View& view, bool profile);
};

#endif /* Analyzer_hpp */
