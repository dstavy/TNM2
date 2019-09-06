//
//  PresentationUpdate.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "PresentationUpdate.hpp"
#include "Analyzer.hpp"
#include "Globals.h"
#include "Gender.h"
#include "Utils.hpp"

//#defne USE_MOVIE

const string PresentationUpdate::JSON_FILE_LOCAL = "records/dataset.json";
const string PresentationUpdate::JSON_FILE_LOCAL_SAVE = "records/dataset.json";
const string PresentationUpdate::JSON_FILE = "../../../The-Normalizing-Machine/bin/Data/records/dataset.json";
const string PresentationUpdate::FACE_DIR = "Faces/";
const string PresentationUpdate::MOVIE_DIR = "Movies/";
const string PresentationUpdate::SEQ_IMAGE_DIR = "D:/capture/";//Images/";
const string PresentationUpdate::JSON_NEW_USER_FILE = "records/user.json";

#ifdef NO_RELEASE_BERLIN
const string PresentationUpdate::IMAGE_EXT = "jpg";
#else
const string PresentationUpdate::IMAGE_EXT = "jpg";
#endif
const string PresentationUpdate::IMAGE_SUF = "." + IMAGE_EXT;
#define MAX_USERS 300

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
	string json;
    bool updated = false;

	if (!firstUpdate) {
#ifdef NO_RELEASE_BERLIN
		json = JSON_FILE_LOCAL;
#else
		json = JSON_FILE_LOCAL;
#endif
		firstUpdate = true;
	}
	else {
#ifdef NO_RELEASE_BERLIN
		json = JSON_NEW_USER_FILE;
#else
		json = JSON_NEW_USER_FILE;
#endif
	}
	ofLogNotice() << " XXXX XXX XX" << json << endl;

    if (file.doesFileExist(json)) {
        file.open(json);
		
		time_t t = std::filesystem::last_write_time(file);
		cout << "ttt" << t << endl;
        if (t > lastUpdate) {
			lastUpdate = t;

            std::time_t f = std::filesystem::last_write_time(file);
            ofLogNotice() << "file time: " << f << " last update: "  << lastUpdate;
            file.close();
            if (datasetJson.open(json)) {
                //frontTracker->setThreaded(false);
               // profileTracker->setThreaded(false);
				int i = datasetJson["users"].size() - MAX_USERS;
                if (i < 0) {
                    i = 0;
                }
				
                for (; i < datasetJson["users"].size(); ++i) {
                    Json::Value v = datasetJson["users"][i];
                    string id = v["id"].asString();
                    float score = v["score"].asFloat();
                    int rounds = v["rounds"].asInt();
                    string timestamp = v["timestamp"].asString();
					float shouldersWidth = v["shouldersWidth"].asFloat();
					float torsoLength = v["torsoLength"].asFloat();
					float totalHeight = v["totalHeight"].asFloat();
                    float headHeight = v["headHeight"].asFloat();
					float armLength = v["armLength"].asFloat();
                    
                    int age = 0;
                    Gender gender = None;
                    float beard = 0.0;
                    string hairColor;
                    Glasses glasses = DontCare;
                    
                    if (v["face_api"].size() > 0) {
                        ofxJSONElement faceV = v["face_api"][0]["faceAttributes"];
                        age  =  faceV["age"].asInt();
                        gender =  getGenderFromString(faceV["gender"].asString());
                        beard = faceV["facialHair"]["beard"].asFloat();
                        glasses = getGlassesFromString(faceV["glasses"].asString());
                        if (faceV["hair"]["invisible"].asBool()) {
                            hairColor = "invisible";
                        }
                        else if (faceV["hair"]["bald"].asFloat() > BALD_THRESHOLD) {
                            hairColor = "bold";
                        } else if (faceV["hair"]["hairColor"].size() > 0) {
                            hairColor = faceV["hair"]["hairColor"][0]["color"].asString();
                        }
                    }
                    else { // old user
                        age = v["age"].asInt();
                        gender = getGenderFromString(v["gender"].asString());
                        beard = v["beard"].asFloat();
                        hairColor = v["hairColor"].asString();
                        glasses = getGlassesFromBoolean(v["glasses"].asBool());
                    }
                    if (id.size() > 0) {
						//UserMap::iterator it = users->find(id);
						//if (it == users->end()) {
							// not found
							User* tmp = createUser(id);
							if (tmp != NULL) {
								// set data
								setUser(tmp,
										0.5, // med
                                        rounds,
                                        timestamp,
										shouldersWidth,
										torsoLength,
										totalHeight,
										headHeight,
										armLength,
                                        age,
                                        gender,
                                        beard,
                                        hairColor,
                                        glasses
                                        );
								
                                if (age == 0) {
                                    string imageFileName = FACE_DIR + id + "_" + std::to_string(0) + IMAGE_SUF;
                                    FaceApi faceApi;
                                    faceApi.analyzeFace(imageFileName, tmp);
                                }
								users->insert(std::pair<string, User*>(id, tmp));
								user = tmp;
							} else {
								ofLogError() << "could not create new user with id" << id;
							}
						}
                        else {
                            ofLogError() << "json-entry without id!";
                        }
					float points = .99;
					for (int j = 0; j < v["selections"].size(); j++)
					{
						UserMap::iterator it = users->find(v["selections"][j].asString());
						if (it != users->end()) {
							updateUser(it->second, points);
						}
						points -= .33;
					}
                } // for (iterating ids
                
                ofLogNotice() << "sorting users";
                
                vector<User*> sortedUsers;
                getUsersList(sortedUsers);
                // set factor score
                if (!sortedUsers.empty()) {
                    groupManager->update(sortedUsers);
                }
			} else {
				ofLogError() << "could not open json!";
			}
        } else {
			// file not udpated!
           file.close();
        }
	} else {
        ofLogError() << "could not open " + file.getAbsolutePath();
	}
	
    saveUsersToJson(users);
   // frontTracker->setThreaded(true);
   // profileTracker->setThreaded(true);

    return user;
}
void PresentationUpdate::getUsersList(vector<User*>& usersOnly) {
    std::for_each(users->begin(), users->end(), [&](const std::pair<const string, User*>& ref) {
        usersOnly.push_back(ref.second);
    });
    //sort users by score
    sort(usersOnly.begin(), usersOnly.end(), [](const User* lhs, const User* rhs) {
        return lhs->score < rhs->score;
    });
    //set refoctor values on users
    //return usersOnly;
}

User* PresentationUpdate::createUser(string id) {
    User* user = NULL;
    bool profile = false;
    bool res = false;
    //look if we have face image
    ofFile file;
    string imageFileName = FACE_DIR + id + "_" + std::to_string(profile) + IMAGE_SUF;
    if (file.doesFileExist(imageFileName)) {
        user = new User(id);
        res = Analyzer::faceAnalyze(imageFileName, *frontTracker, *user, profile);
        profile = true;
        imageFileName = FACE_DIR + id + "_" + std::to_string(profile) + IMAGE_SUF;
        if (file.doesFileExist(imageFileName)) {
            Analyzer::faceAnalyze(imageFileName, *profileTracker, *user, profile);
        }
    } else // we dont have a face file . maybe new
    {
        profile = false;
        user = new User(id);
       // for (bool profileb : { false, true }) {
		bool profileb = false;
        bool resb;
        //some path, may be absolute or relative to bin/data
        string path = SEQ_IMAGE_DIR;// + id;//  +"_" + std::to_string(profileb);
        ofDirectory dir(path);
		ofLogError() << "HERHERH";

		if (dir.exists()) {
			ofLogError() << "HERHERH";
            //only show png files
            dir.allowExt(IMAGE_EXT);
            //populate the directory object
            dir.listDir();
            ofImage image;
            float highScore = 10;
            int selected = -1;
            int numPhoto = 0;
            //go through and print out all the paths
            for(int i = 0; i < dir.size(); i++){
               // ofLogNotice(dir.getPath(i));
                if (dir.getName(i).rfind(id, 0) == 0) {
                    numPhoto++;
                    if (image.load(dir.getPath(i))) {
                        image.update();
                        float score = Analyzer::getFaceScore(image, *frontTracker, profileb);
                        if (score != Analyzer::NO_FOUND) {
                            score = abs(score);
                            if (!profile && score < highScore) {
                                highScore = score;
                                selected = i;
                            }
                            image.clear();
                        }
                    }
                    if (numPhoto >= 5) {
                        break;
                    }
                }
            }
            if (selected >= 0) {
                string sourcImage = dir.getPath(selected);
                resb = Analyzer::faceAnalyze(dir.getPath(selected), *profileTracker, *user, profileb);
                if (resb) {
                    string outImage = FACE_DIR + id + "_" + std::to_string(profileb) + IMAGE_SUF;
                    resb = saveUserImage(outImage, user->getView(profileb));
                    resb = Analyzer::faceAnalyze(outImage, *profileTracker, *user, profileb); // do again on smaller image
                    if (resb && !profileb) {
                        //user->currentUser = true;
                        faceApi.analyzeFace(outImage, user); // try small image if good enugh.
                        // todo: this syncronized call and should change to unsyncronizd
                        res = resb;
                    }
            }
            //dir.remove(true);
            }
        }
    }
    
    if (res) {
        user->id = id;
        return user;
    }
    else {
        if (user != NULL) {
            delete(user);
            user = NULL;
        }
        return NULL;
    }
}

void PresentationUpdate::updateUser(User* user, float points) {
    user->rounds++;
    user->score = points / user->rounds + user->score * (1. - 1./user->rounds);
    // (vScore + xScore + 1); // avoid getting 100%
}

void PresentationUpdate::setUser(User* user, float score, int rounds, string timestamp, float shouldersWidth, float torsoLength, float totalHeight, float headHeight, float armLength, int age, Gender gender, float beard,string hairColor, Glasses glasses)
{
    user->score = score;
    user->rounds = rounds;
    user->timestamp = timestamp;
	user->shouldersWidth = shouldersWidth;
	user->torsoLength = torsoLength;
	user->totalHeight = totalHeight;
	user->headHeight = headHeight;
	user->armLength = armLength;
    user->age = age;
    user->gender = gender;
    user->beard = beard;
    user->hairColor = hairColor;
    user->glasses = glasses;
    
	//updateUser(user, points, rounds);
}

bool PresentationUpdate::saveUserImage(string fileName, View& view) {
    ofImage  image = view.getImage();
    ofRectangle& rec = view.getBounderyBox(View::HEAD);
    int newW  = MIN(View::FACE_EXTENDED_PADDING * rec.width, image.getWidth());
    int newH  = MIN(View::FACE_EXTENDED_PADDING * rec.height, image.getHeight());
    int moveX = MAX(0, (newW - rec.width ) /2);
    int moveY = MAX((newH - rec.height ) /2, 0);
    image.crop(MAX(0, rec.x -moveX), MAX(0, rec.y - moveY), newW, newH);
    return image.save(fileName);

	// todo:: BUG WHEN FACE IS TOO SMALL
}
