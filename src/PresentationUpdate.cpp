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
//#defne USE_MOVIE

const string PresentationUpdate::JSON_FILE_LOCAL = "records/dataset.json";
const string PresentationUpdate::JSON_FILE = "../../../The-Normalizing-Machine/bin/Data/records/dataset.json";
const string PresentationUpdate::FACE_DIR = "Faces/";
const string PresentationUpdate::MOVIE_DIR = "Movies/";
const string PresentationUpdate::SEQ_IMAGE_DIR = "../../../The-Normalizing-Machine/bin/Data/SeqImg/";
#ifdef NO_RELEASE_BERLIN
const string PresentationUpdate::IMAGE_EXT = "jpg";
#else
const string PresentationUpdate::IMAGE_EXT = "jpeg";
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
		json = JSON_FILE;
#endif
		firstUpdate = true;
	}
	else {
#ifdef NO_RELEASE_BERLIN
		json = JSON_FILE_LOCAL;
#else
		json = JSON_FILE;
#endif
	}
	
    if (file.doesFileExist(json)) {
		
        file.open(json);
		
        if (std::filesystem::last_write_time(file) > lastUpdate) {
            long f = std::filesystem::last_write_time(file);
            file.close();
            if (datasetJson.open(json)) {
                //frontTracker->setThreaded(false);
               // profileTracker->setThreaded(false);
				int i = datasetJson.size() - MAX_USERS;
                if (i < 0) {
                    i = 0;
                }
				
                for (; i < datasetJson.size(); ++i) {
					
                    Json::Value v = datasetJson[i];
                    string id = v["id"].asString();
                    int vScore = v["vScore"].asInt();
                    int xScore = v["xScore"].asInt();
					float shouldersWidth = v["shouldersWidth"].asFloat();
					float torsoLength = v["torsoLength"].asFloat();
					float totalHeight = v["totalHeight"].asFloat();
                    float headHeight = v["headHeight"].asFloat();
					float armLength = v["armLength"].asFloat();
                    
                    int age = v["age"].asInt();
                    Gender gender = getGenderFromString(v["gender"].asString());
                    float beard = v["beard"].asFloat();
                    string hairColor = v["hairColor"].asString();

                    if (id.size() > 0) {
						UserMap::iterator it = users->find(id);
						if (it == users->end()) {
							// not found
							User* tmp = createUser(id);
							if (tmp!= NULL) {
								
								// set data
								setUser(tmp,
										vScore,
										xScore,
										shouldersWidth,
										torsoLength,
										totalHeight,
										headHeight,
										armLength,
                                        age,
                                        gender,
                                        beard,
                                        hairColor
                                        );
								
								users->insert(std::pair<string, User*>(id, tmp));
								user = tmp;
								updated = true;
							} else {
								ofLogError() << "could not create new user with id" << id;
							}
						}
						else {
							updateUser(it->second, vScore, xScore);
						}
						
						//ofLogNotice() << "user with id: " << id;
						
					} else {
						//
						ofLogError() << "json-entry without id!";
					}
                } // for (iterating json)
			} else {
				ofLogError() << "could not open json!";
			}
			
            if (updated) {
				
				ofLogNotice() << "sorting users";
				
                vector<User*> sortedUsers;
                getUsersList(sortedUsers);
				// set factor score
                if (!sortedUsers.empty()) {
                    User::setHighestScore(sortedUsers[sortedUsers.size() -1]->score
                                          + 0.001);//we dont want 1
                    User::setLowestScore(sortedUsers[0]->score - 0.001); // we dont want 0
                    groupManager->update(sortedUsers);
                }
            }
        } else {
			// file not udpated!
           file.close();
        }
        lastUpdate = ofGetSystemTimeMillis();
	} else {
		// file does not exist
	}
	
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
#ifdef USE_MOVIE
        string videoFileName = MOVIE_DIR + id + "_" + std::to_string(profile) + ".mov";
        imageFileName = FACE_DIR + id + "_" + std::to_string(profile) + IMAGE_SUF;
        if (file.doesFileExist(videoFileName)) {
            file.open(videoFileName);
            if (std::filesystem::last_write_time(file) > lastUpdate) { // new
                user = new User(id);
                file.close();
                frontPlayer->close();
                profilePlayer->close();
                res = Analyzer::videoAnalyze(videoFileName, *frontPlayer, *profileTracker, *user, profile, imageFileName);
                if (res) {
                    //user->currentUser = true;
                }
                profile = true;
                videoFileName = MOVIE_DIR + id + "_" + std::to_string(profile) + ".mov";
                imageFileName = FACE_DIR + id + "_" + std::to_string(profile) + IMAGE_SUF;
                if (file.doesFileExist(videoFileName)) {
                    file.open(videoFileName);
                    if (std::filesystem::last_write_time(file) > lastUpdate) { // new
                        file.close();
                        Analyzer::videoAnalyze(videoFileName, *profilePlayer, *profileTracker, *user, profile, imageFileName);
                    } else {
                        file.close();
                    }
                }
            } else {
                file.close();
            }
        }
#endif
        user = new User(id);
       // for (bool profileb : { false, true }) {
		bool profileb = false;
        bool resb;
        //some path, may be absolute or relative to bin/data
        string path = SEQ_IMAGE_DIR + id;//  +"_" + std::to_string(profileb);
        ofDirectory dir(path);
        if (dir.exists()) {
            //only show png files
            dir.allowExt(IMAGE_EXT);
            //populate the directory object
            dir.listDir();
            ofImage image;
            float highScore = 10;
            int selected = -1;
            
            //go through and print out all the paths
            for(int i = 0; i < dir.size(); i++){
                ofLogNotice(dir.getPath(i));
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

void PresentationUpdate::updateUser(User* user, int vScore, int xScore) {
    user->xScore = xScore;
    user->vScore = vScore;
    user->score = (float)vScore / (vScore + xScore + 1); // avoid getting 100%
}

void PresentationUpdate::setUser(User* user, int vScore, int xScore, float shouldersWidth, float torsoLength, float totalHeight, float headHeight, float armLength, int age, Gender gender, float beard,string hairColor)
{
	user->shouldersWidth = shouldersWidth;
	user->torsoLength = torsoLength;
	user->totalHeight = totalHeight;
	user->headHeight = headHeight;
	user->lowerArm = armLength;
    user->age = age;
    user->gender = gender;
    user->beard = beard;
    user->hairColor = hairColor;
    
	updateUser(user, vScore, xScore);
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
