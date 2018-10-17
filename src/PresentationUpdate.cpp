//
//  PresentationUpdate.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "PresentationUpdate.hpp"
#include "Analyzer.hpp"

long PresentationUpdate::lastUpdate = ofGetSystemTimeMillis();
const string PresentationUpdate::JSON_FILE = "/records/dataset.json";
const string PresentationUpdate::FACE_DIR = "/faces/";
const string PresentationUpdate::MOVIE_DIR = "/Movies/";

ofxJSONElement PresentationUpdate::loadLibrary(string url)
{
    ofxJSONElement datasetJson;
    // Now parse the JSON
    bool parsingSuccessful = datasetJson.open(url);
    
    if (parsingSuccessful)
    {
        ofLogNotice("loadLibrary") << datasetJson.getRawString(true);
    }
    else {
        ofLogNotice("loadLibrary") << "Failed to parse JSON.";
    }
    
    return datasetJson;
/*    for (unsigned int i = 0; i < datasetJson.size(); ++i)
    {
        Json::Value v = datasetJson[i];
        string id = v["id"].asString();
        
        dataset.push_back(RecordedData(v));
    }
 */
}


User* PresentationUpdate::update() {
    User* user = NULL;
    ofFile file;
    if (file.open(JSON_FILE)) {
        if (std::filesystem::last_write_time(file) > lastUpdate) {
            file.close();
            loadLibrary(JSON_FILE);
            ofxJSONElement datasetJson = loadLibrary(JSON_FILE);
            if (datasetJson != nullptr) {
                for (unsigned int i = 0; i < datasetJson.size(); ++i)
                {
                    Json::Value v = datasetJson[i];
                    string id = v["id"].asString();
                    int vScore = v["vScore"].asInt();
                    int xScore = v["xScore"].asInt();
                    map<string,User*>::iterator it = users.find(id);
                    if (it == users.end()) {
                        // not found
                        user = createUser(id, vScore, xScore);
                    } else {
                        updateUser(it->second, vScore, xScore);
                    }
                }
            }
        } else {
           file.close();
        }
    }
    return user;
}


User* PresentationUpdate::createUser(string id, int vScore, int xScore) {
    User* user = new User(id);
    bool profile = false;
    bool res;
    //look if we have face image
    ofFile file;
    string imageFileName = FACE_DIR + "_" + id + "_" + std::to_string(profile) + ".png";
    if (file.doesFileExist(imageFileName)) {
        bool res = Analyzer::faceAnalyze(imageFileName, *frontTracker, *user, profile);
        profile = true;
        imageFileName = FACE_DIR + "_" + id + "_" + std::to_string(profile) + ".png";
        if (file.doesFileExist(imageFileName)) {
            Analyzer::faceAnalyze(imageFileName, *profileTracker, *user, profile);
        }
    } else // we dont have a face file . maybe new
    {
        profile = false;
        string videoFileName = MOVIE_DIR + "_" + id + "_" + std::to_string(profile) + ".mov";
        imageFileName = FACE_DIR + "_" + id + "_" + std::to_string(profile) + ".png";
        if (file.doesFileExist(videoFileName)) {
            file.open(videoFileName);
            if (std::filesystem::last_write_time(file) > lastUpdate) { // new
                file.close();
                frontPlayer->close();
                profilePlayer->close();
                res = Analyzer::videoAnalyze(videoFileName, *frontPlayer, *frontTracker, *user, profile, imageFileName);
                if (res) {
                    user->currentUser = true;
                }
            } else {
                file.close();
            }
        }
        profile = true;
        videoFileName = MOVIE_DIR + "_" + id + "_" + std::to_string(profile) + ".mov";
        imageFileName = FACE_DIR + "_" + id + "_" + std::to_string(profile) + ".png";
        if (file.doesFileExist(videoFileName)) {
            file.open(videoFileName);
            if (std::filesystem::last_write_time(file) > lastUpdate) { // new
                file.close();
                Analyzer::videoAnalyze(videoFileName, *profilePlayer, *profileTracker, *user, profile, imageFileName);
            } else {
                file.close();
            }
        }
    }
    if (res) {
        users.insert(std::pair<string, User*>(id, user));
        return user;
    }
    else {
        delete(user);
        return NULL;
    }
}

void PresentationUpdate::updateUser(User* user, int vScore, int xScore) {
    user->xScore = xScore;
    user->vScore = vScore;
}
