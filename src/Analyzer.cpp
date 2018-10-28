//
//  VideoTracker.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "Analyzer.hpp"

#define SEC_5 = 30 * 5;
//const int Analyzer::FACE_EXTENDED_PADDING = 150;

//--------------------------------------------------------------
bool Analyzer::videoAnalyze(string fileName, ofVideoPlayer& player, ofxFaceTracker2& tracker, User& user, bool profile, string outImage){
    //tracker.setFaceRotation(90);
    player.load(fileName);    // Setup tracker
    player.play();
    tracker.setup();
    int start =  0;
    int countDown = 30; // after 30 frames not detecting stops
    float profileThreshold = 0.2;
    float frontThreshold = 0.1;
    ofVec3f bestFront(1.,1.,1.);
    ofVec3f middle(0.,0., 0.);
    float bestProflie = 0.;
    ofVec3f translation;
    ofVec3f scale;
    ofQuaternion rotation;
    ofQuaternion so;
    int bestFrame = 0;
    int lastFrame = -1;
    
    player.setUseTexture(false);
    int i = 0;
    for (i = 0; i < player.getTotalNumFrames(); i++) {
        player.setFrame(i);
        tracker.update(player);
        if(!tracker.size()){
            countDown--;
            if (countDown < 0) {
                break;
            }
        }
        else {
            ofxFaceTracker2Instance camFace = tracker.getInstances()[0];
            camFace.getPoseMatrix().decompose(translation, rotation, scale, so);
            if (!profile) {
                if (bestFront.distance(middle) > rotation.asVec3().distance(middle))
                {
                    bestFront = rotation.asVec3();
                    bestFrame = i;
                }
            } else {
                if (bestProflie < rotation.asVec3().x) {
                    bestProflie = rotation.asVec3().x;
                    bestFrame = i;
                }
            }
        }
    }
        
    lastFrame = i;
    player.setFrame(bestFrame);
    tracker.update(player);
    if (faceInflate(tracker, user, profile)) {
        ofPixels pixels;
        player.getPixels();
        
        // TODO:: crop
        View& view = user.getView(profile);
        ofImage& image = view.getImage();
        image.setFromPixels(pixels);
        image.rotate90(90);
        ofRectangle& rec = view.getBounderyBox(View::HEAD);
        image.crop(rec.x - View::FACE_EXTENDED_PADDING, rec.y - View::FACE_EXTENDED_PADDING, rec.width + 2 * View::FACE_EXTENDED_PADDING, rec.height + 2 * View::FACE_EXTENDED_PADDING);
        image.save(outImage);
        return true;
    }
    return false;
}

bool Analyzer::faceAnalyze(string fileName, ofxFaceTracker2& tracker, User& user, bool profile)  {
    ofImage& image = user.getView(profile).getImage();
    image.setUseTexture(true);
    if (image.load(fileName))
    {
        // tracker.setFaceRotation(0);
        image.update();
        tracker.update(image);
        return faceInflate(tracker, user, profile);
    } else
    {
        ofLogNotice("cant load " + fileName);
    }
    return false;
}

bool Analyzer::faceInflate(ofxFaceTracker2& tracker, User& user, bool profile) {
    View& view = user.getView(profile);
    if(tracker.size()){
        ofxFaceTracker2Instance camFace = tracker.getInstances()[0];
        //  if (headingforwad && !proflie || headingside && profile)
        //{
        ofxFaceTracker2Landmarks landmarks = camFace.getLandmarks();
        ofPolyline eyeL = landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
        ofPolyline eyeR = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
        //ofPolyline eyeLTop= landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE_TOP);
        //ofPolyline eyeRTop = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE_TOP);
        //ofPolyline nose = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BASE);
        //ofPolyline noseB = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BRIDGE);
        //ofPolyline mouth = landmarks.getImageFeature(ofxFaceTracker2Landmarks::OUTER_MOUTH);
        //ofPolyline face = landmarks.getImageFeature(ofxFaceTracker2Landmarks::FACE_OUTLINE);
        ofPolyline leb = landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYEBROW);
        ofPolyline reb = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYEBROW);
        //ofPolyline all = landmarks.getImageFeature(ofxFaceTracker2Landmarks::ALL_FEATURES);
        
        ofRectangle face = camFace.getBoundingBox();
        
        eyeL.addVertices(leb.getVertices());
        eyeR.addVertices(reb.getVertices());
        eyeL.addVertices(eyeR.getVertices());
        view.parts[View::EYES] = eyeL.getBoundingBox();
        
        ofPolyline mouth;
        mouth.addVertex(landmarks.getImagePoint(33).x, landmarks.getImagePoint(33).y, 0);
        mouth.addVertex(landmarks.getImagePoint(6).x, landmarks.getImagePoint(6).y, 0);
        mouth.addVertex(landmarks.getImagePoint(10).x, landmarks.getImagePoint(10).y, 0);
        view.parts[View::MOUTH] = mouth.getBoundingBox();
        
        ofPolyline nose;
        nose.addVertex(landmarks.getImagePoint(20).x, landmarks.getImagePoint(20).y, 0);
        nose.addVertex(landmarks.getImagePoint(23).x, landmarks.getImagePoint(23).y, 0);
        nose.addVertex(landmarks.getImagePoint(52).x, landmarks.getImagePoint(52).y, 0);
        nose.addVertex(landmarks.getImagePoint(50).x, landmarks.getImagePoint(50).y, 0);
        view.parts[View::NOSE] = nose.getBoundingBox();
        
        ofPolyline forhead;
        forhead.addVertex(landmarks.getImagePoint(0).x, landmarks.getImagePoint(41).y - face.height * 0.6, 0);
        forhead.addVertex(landmarks.getImagePoint(16).x, landmarks.getImagePoint(46).y - face.height * 0.6, 0);
        forhead.addVertex(landmarks.getImagePoint(16).x, landmarks.getImagePoint(46).y, 0);
        forhead.addVertex(landmarks.getImagePoint(0).x, landmarks.getImagePoint(41).y, 0);
        view.parts[View::FORHEAD] = forhead.getBoundingBox();
        
        ofRectangle head(face);
        head.y -= 0.3 * face.height; //MAX(0, face.height/2); TODO:: poptential bug if goes below 0. only effect imported pics
        head.setHeight(1.3 * face.height);
        view.parts[View::HEAD] = head;
        
        if (!profile) {
            /*
            float faceLength = landmarks.getImagePoint(27).distance(landmarks.getImagePoint(8));
            //take mesures
            user.measures[View::FORHEAD] = 0.0; // no meaning to devide by highet
            user.measures[View::EYES] = view.parts[View::EYES].height / faceLength;
            user.measures[View::NOSE] = view.parts[View::NOSE].height / faceLength;
            user.measures[View::MOUTH] = view.parts[View::MOUTH].height / faceLength;
             */
        } else {
            ofVec3f translation;
            ofVec3f scale;
            ofQuaternion rotation;
            ofQuaternion so;
            camFace.getPoseMatrix().decompose(translation, rotation, scale, so);
            float rotAdd = EAR_ROTATION_FACTOR * rotation.asVec3().x;
            //right ear
            ofRectangle earR;
            earR.setY(landmarks.getImagePoint(0).y);
            earR.setHeight((landmarks.getImagePoint(2).y - earR.getY()));
            earR.setWidth(MAX((landmarks.getImagePoint(2).y - earR.getY()) * rotAdd / 2, 0));
            earR.setX(landmarks.getImagePoint(2 ).x - earR.width);
            view.parts[View::RIGHT_EAR] = earR;
            // left ear
            // not doing it
            /*
            ofRectangle earL;
            earL.setX(landmarks.getImagePoint(14).x);
            earL.setY(landmarks.getImagePoint(16).y);
            earL.setWidth(MAX((landmarks.getImagePoint(14).y - earL.getY()) * (-rotAdd) /2, 0));
            earL.setHeight((landmarks.getImagePoint(14).y - earL.getY()));
            view.parts[View::LEFT_EAR] = earL;
            */
        }
        view.landmarks = landmarks.getImagePoints();
        view.setActive(true);
        return true;
    }
    view.setActive(false);
    return false;
}
