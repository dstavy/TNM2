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
#include "GroupManager.hpp"
#include "Gender.h"
#include "FaceApi.hpp"

// his class check the videos. add a new user and update scores
class PresentationUpdate {
public:
    static const string JSON_FILE;
    static const string FACE_DIR;
    static const string MOVIE_DIR;
    static const string SEQ_IMAGE_DIR;
    static const string IMAGE_EXT;
    static const string IMAGE_SUF;
	static const string JSON_FILE_LOCAL;
	
    string frontVideo;
    string profileVideo;
    ofVideoPlayer* frontPlayer;
    ofVideoPlayer* profilePlayer;
    ofxFaceTracker2* frontTracker;
    ofxFaceTracker2* profileTracker;
    GroupManager* groupManager;
    UserMap* users;
	bool firstUpdate = false;
    
    void setup(UserMap* users, ofVideoPlayer* frontPlayer, ofVideoPlayer* profilePlayer, ofxFaceTracker2* frontTracker, ofxFaceTracker2* profileTracker, GroupManager* groupManager) {
        this->users = users;
        this->frontPlayer = frontPlayer;
        this->profilePlayer = profilePlayer;
        this->frontTracker = frontTracker;
        this->profileTracker = profileTracker;
        this->groupManager = groupManager;
        this->lastUpdate = 0;
    };
    
    User* update();
	
private:
    long lastUpdate;
    //ofxJSONElement loadLibrary(string url);
    //ofxJSONElement datasetJson;
    User* createUser(string id);
    static void updateUser(User* user, float points);
	static void setUser(User* user, float score, int rounds, string timestamp, float shouldersWidth, float torsoLength, float totalHeight, float headHeight, float armLength, int age, Gender gender, float beard,string hairColor, Glasses glasses);
    void getUsersList(vector<User*>& usersOnly);
    ofxJSONElement datasetJson;
    static bool saveUserImage(string fileName, View& view);
    FaceApi faceApi;
};
#endif /* PresentationUpdate_hpp */
