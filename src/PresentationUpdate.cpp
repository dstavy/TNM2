//
//  PresentationUpdate.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "PresentationUpdate.hpp"
#include "Analyzer.hpp"

long PresentationUpdate::lastUpdate = 0;
const string PresentationUpdate::JSON_FILE = "Records/dataset.json";
const string PresentationUpdate::FACE_DIR = "Faces/";
const string PresentationUpdate::MOVIE_DIR = "Movies/";

/*
ofxJSONElement PresentationUpdate::loadLibrary(string url)
{
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
*//*    for (unsigned int i = 0; i < datasetJson.size(); ++i)
    {
        Json::Value v = datasetJson[i];
        string id = v["id"].asString();
        
        dataset.push_back(RecordedData(v));
    }
 *//*
}
*/


User* PresentationUpdate::update() {
    User* user = NULL;
    ofFile file;
    if (file.doesFileExist(JSON_FILE)) {
        file.open(JSON_FILE);
        if (std::filesystem::last_write_time(file) > lastUpdate) {
            long f = std::filesystem::last_write_time(file);
            file.close();
            //loadLibrary(JSON_FILE);
            if (datasetJson.open(JSON_FILE)) {
                frontTracker->setThreaded(false);
                profileTracker->setThreaded(false);
                for (unsigned int i = 0; i < datasetJson.size(); ++i)
                {
                    Json::Value v = datasetJson[i];
                    string id = v["id"].asString();
                    int vScore = v["vScore"].asInt();
                    int xScore = v["xScore"].asInt();
                    UserMap::iterator it = users.find(id);
                    if (it == users.end()) {
                        // not found
                        user = createUser(id);
                        if (user!= NULL) {
                            updateUser(user, vScore, xScore);
                            users.insert(std::pair<string, User*>(id, user));
                        }
                    } else {
                        updateUser(it->second, vScore, xScore);
                    }
                }
                groupManager.update(getUsersList());
                lastUpdate = ofGetSystemTimeMillis();
            }
        } else {
           file.close();
        }
    }
    frontTracker->setThreaded(true);
    profileTracker->setThreaded(true);
    return user;
}

vector<User*> PresentationUpdate::getUsersList() {
    vector<User*> usersOnly;
    std::for_each(users.begin(), users.end(), [&](const std::pair<const string, User*>& ref) {
        usersOnly.push_back(ref.second);
    });
    //sort
    sort(usersOnly.begin(), usersOnly.end(), [](const User* lhs, const User* rhs) {
        return lhs->score < rhs->score;
    });
    return usersOnly;
}

User* PresentationUpdate::createUser(string id) {
    User* user = new User(id);
    bool profile = false;
    bool res = false;
    //look if we have face image
    ofFile file;
    string imageFileName = FACE_DIR + id + "_" + std::to_string(profile) + ".png";
    if (file.doesFileExist(imageFileName)) {
        res = Analyzer::faceAnalyze(imageFileName, *frontTracker, *user, profile);
        profile = true;
        imageFileName = FACE_DIR + id + "_" + std::to_string(profile) + ".png";
        if (file.doesFileExist(imageFileName)) {
            Analyzer::faceAnalyze(imageFileName, *profileTracker, *user, profile);
        }
    } else // we dont have a face file . maybe new
    {
        string videoFileName = MOVIE_DIR + id + std::to_string(profile) + ".mov";
        imageFileName = FACE_DIR + id + std::to_string(profile) + ".png";
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
        videoFileName = MOVIE_DIR + id + std::to_string(profile) + ".mov";
        imageFileName = FACE_DIR + id + std::to_string(profile) + ".png";
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
        user->id = id;
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
    user->score = (float)vScore / (vScore + xScore);
}
