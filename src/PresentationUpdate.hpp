//
//  PresentationUpdate.hpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#ifndef PresentationUpdate_hpp
#define PresentationUpdate_hpp

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxFaceTracker2.h"
#include "User.hpp"

// his class check the videos. add a new user and update scores
class PresentationUpdate {
public:
    static const string JSON_FILE;
    static const string FACE_DIR;
    static const string MOVIE_DIR;
    static long lastUpdate;
    string frontVideo;
    string profileVideo;
    ofVideoPlayer* frontPlayer;
    ofVideoPlayer* profilePlayer;
    ofxFaceTracker2* frontTracker;
    ofxFaceTracker2* profileTracker;
    map<string, User*> users;
    
    void setup(map<string, User*> users, ofVideoPlayer* frontPlayer, ofVideoPlayer* profilePlayer, ofxFaceTracker2* frontTracker, ofxFaceTracker2* profileTracker) {
        this->users = users;
        this->frontPlayer = frontPlayer;
        this->profilePlayer = profilePlayer;
        this->frontTracker = frontTracker;
        this->profileTracker = profileTracker;
    };
    
    User* update();
private:
    static ofxJSONElement loadLibrary(string url);
    User* createUser(string id, int vScore, int xScore);
    static void updateUser(User* user, int vScore, int xScore);
};
#endif /* PresentationUpdate_hpp */
