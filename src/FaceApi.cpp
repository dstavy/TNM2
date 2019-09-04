//
//  FaceApi.cpp
//  TNM2
//
//  Created by Dan Stavy on 21/08/2019.
//

#include "FaceApi.hpp"
#include "ofxJSON.h"
#include "Gender.h"

FaceApi::FaceApi() {
    user = NULL;
    request.url = action_url;
    request.method = request.POST;
    request.contentType = "application/octet-stream";
    std::map<std::string, std::string> headersFileds =
    {
        {"Ocp-Apim-Subscription-Key", api_key},
        //{"url","https://upload.wikimedia.org/wikipedia/commons/c/c3/RH_Louise_Lillian_Gish//.jpg"}
    };
    request.headers = headersFileds;
    ofRegisterURLNotification(this);
}

void FaceApi::analyzeFace(string image, User* u) {
    this->user = u;
    request.body = ofBufferFromFile(image);
    // requestId = loader.handleRequestAsync(request);
    ofHttpResponse response = loader.handleRequest(request);
    urlResponse(response);
}

void FaceApi::urlResponse(ofHttpResponse &response){
    ofxJSONElement json;
    if(response.request.getId() == requestId && response.status==200 ){  // ok
        // Now parse the JSON
        bool parsingSuccessful = json.openFromBuffer(response.data);
        
        if (parsingSuccessful)
        {
            ofLogNotice("face json") << json.getRawString(true);
        } else {
            ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
            return;
        }
       
		if (json.size() > 0) 
        {
			ofxJSONElement faceV = v["face_api"][0]["faceAttributes"];
			user->age = faceV["age"].asInt();
			user->gender = getGenderFromString(faceV["gender"].asString());
			user->beard = faceV["facialHair"]["beard"].asFloat();
			user->glasses = getGlassesFromString(faceV["glasses"].asString());
			if (faceV["hair"]["invisible"].asBool()) {
				user->hairColor = "invisible";
			}
			else if (faceV["hair"]["bald"].asFloat() > BALD_THRESHOLD) {
				user->hairColor = "bold";
			}
			else if (faceV["hair"]["hairColor"].size() > 0) {
				user->hairColor = faceV["hair"]["hairColor"][0]["color"].asString();
			}
		}
    }
    else {
        ofLog(OF_LOG_ERROR, "Failed get face api response.");
    }
    ofLog(OF_LOG_NOTICE, response.data);
}
