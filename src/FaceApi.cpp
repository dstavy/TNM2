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
        float colorConfidence = 0.0;
        for (Json::ArrayIndex i = 0; i < json.size(); ++i)
        {
            user->age  = json[i]["faceAttributes"]["age"].asInt();
            user->gender = getGenderFromString(json[i]["faceAttributes"]["gender"].asString());
            float beard = json[i]["faceAttributes"]["facialHair"]["beard"].asFloat();
            user->beard = beard;
            if (json[i]["faceAttributes"]["hair"]["invisible"].asBool()) {
                user->hairColor = "invisible";
            }
            else if (json[i]["faceAttributes"]["hair"]["bald"].asFloat() > BALD_THRESHOLD) {
                user->hairColor = "bold";
            } else {
                for (Json::ArrayIndex j = 0; j < json[i]["faceAttributes"]["hair"]["hairColor"].size(); ++j)
                {
                    if (json[i]["faceAttributes"]["hair"]["hairColor"][i]["confidence"].asFloat() > colorConfidence) {
                        user->hairColor = json[i]["faceAttributes"]["hair"]["hairColor"][i]["color"].asString();
                    }
                }
            }
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Failed get face api response.");
    }
    ofLog(OF_LOG_NOTICE, response.data);
}
