//
//  FaceApi.hpp
//  TNM2
//
//  Created by Dan Stavy on 21/08/2019.
//

#ifndef FaceApi_hpp
#define FaceApi_hpp

#include "ofMain.h"
#include "User.hpp"
// https://westus.dev.cognitive.microsoft.com/docs/services/563879b61984550e40cbbe8d/operations/563879b61984550f30395236
// https://westcentralus.dev.cognitive.microsoft.com/docs/services/563879b61984550e40cbbe8d/operations/563879b61984550f30395236/console

class FaceApi {
    
public:
    FaceApi();
    ~FaceApi() {
        ofUnregisterURLNotification(this);
    };
    void analyzeFace(string image, User* usr);

    void urlResponse(ofHttpResponse &httpResponse);
    const string api_key = "24e48e2605994bcea11c18e2a1eae0c6";
    const string action_url = "https://tnm.cognitiveservices.azure.com/face/v1.0/detect?returnFaceId=false&returnFaceLandmarks=false&returnFaceAttributes=age,gender,facialHair,glasses,hair&recognitionModel=recognition_01&returnRecognitionModel=false&detectionModel=detection_01";
    ofURLFileLoader loader;
    ofHttpRequest request;
    int requestId = 0;
    User* user;
};


#endif /* FaceApi_hpp */
