//
//  VideoTracker.cpp
//  TNM2
//
//  Created by Dan Stavy on 09/10/2018.
//

#include "Analyzer.hpp"

#define SEC_5 = 30 * 5;
const int Analyzer::FACE_EXTENDED_PADDING = 150;

//--------------------------------------------------------------
bool Analyzer::videoAnalyze(string fileName, ofVideoPlayer& player, ofxFaceTracker2& tracker, User& user, bool profile, string outImage){
    tracker.setFaceRotation(90);
    player.load(fileName);    // Setup tracker
    player.play();
    tracker.setup();
    int start =  0;
    
    player.setUseTexture(false);
    int i = 0;
    for (i = 0; i < player.getTotalNumFrames(); i++) {
        player.setFrame(i);
        tracker.update(player);
        if(tracker.size()){
            continue;
        }
        else {
           // if (i > 30) {
                i--;
                break;
          //  }
        }
    }
    player.setFrame(i);
    tracker.update(player);
    if (faceInflate(tracker, user, profile)) {
        ofPixels pixels;
        player.getPixels();
        
        // TODO:: crop
        ofImage image;
        image.setFromPixels(pixels);
        image.rotate90(90);
        ofRectangle rec = user.getView(profile).parts[View::HEAD];
        image.crop(rec.x - FACE_EXTENDED_PADDING, rec.y - FACE_EXTENDED_PADDING, rec.width + 2*FACE_EXTENDED_PADDING, rec.height + 2*FACE_EXTENDED_PADDING);
        image.save(outImage);
        return true;
    }
    return false;
}
    /*
        //string out = View::FACES_DIR + std::to_string(user.id) + "_" + std::to_string(profile) + ".png";
        image.save(outImage);
        if(tracker.size()){
            ofxFaceTracker2Instance camFace = tracker.getInstances()[0];
          //  if (headingforwad && !proflie || headingside && profile)
            {
                if (start!= 0) {
                    start = i;
                }
                ofxFaceTracker2Landmarks landmarks = camFace.getLandmarks();
                ofPolyline eyeL = landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
                ofPolyline eyeR = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
                //ofPolyline eyeLTop= landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE_TOP);
                //ofPolyline eyeRTop = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE_TOP);
                //ofPolyline nose = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BASE);
                ofPolyline noseB = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BRIDGE);
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
                nose.addVertex(landmarks.getImagePoint(19).x, landmarks.getImagePoint(19).y, 0);
                nose.addVertex(landmarks.getImagePoint(24).x, landmarks.getImagePoint(24).y, 0);
                nose.addVertex(landmarks.getImagePoint(51).x, landmarks.getImagePoint(51).y, 0);
                view.parts[View::NOSE] = nose.getBoundingBox();
                
                ofPolyline forhead;
                forhead.addVertex(landmarks.getImagePoint(0).x, landmarks.getImagePoint(41).y, 0);
                forhead.addVertex(landmarks.getImagePoint(16).x, landmarks.getImagePoint(46).y, 0);
                forhead.addVertex(landmarks.getImagePoint(16).x, landmarks.getImagePoint(46).y - face.height/4, 0);
                forhead.addVertex(landmarks.getImagePoint(0).x, landmarks.getImagePoint(41).y - face.height/4, 0);
                view.parts[View::FORHEAD] = nose.getBoundingBox();
                
                ofRectangle head(face);
                head.y -= face.height/4;
                head.setHeight(1.25 * face.height);
                view.parts[View::HEAD] = head;
            
                
                 // TODO:: add
                //view->landmarks[] = landmarks.getImagePoint(<#int i#>);
                ofPixels pixels;
                player.getPixels();
                
                // TODO:: crop
                ofImage image;
                image.setFromPixels(pixels);
                
                //string out = View::FACES_DIR + std::to_string(user.id) + "_" + std::to_string(profile) + ".png";
                image.save(outImage);
                
                if (!profile) {
                    float faceLength = landmarks.getImagePoint(27).distance(landmarks.getImagePoint(8));
                    //take mesures
                    user.measures[View::FORHEAD] = 0.0; // no meaning to devide by highet
                    user.measures[View::EYES] = view.parts[View::EYES].height / faceLength;
                    user.measures[View::NOSE] = view.parts[View::NOSE].height / faceLength;
                    user.measures[View::MOUTH] = view.parts[View::MOUTH].height / faceLength;
                }
                return i;
            }
        }
    }
    return -1;
}
*/
    
bool Analyzer::faceAnalyze(string fileName, ofxFaceTracker2& tracker, User& user, bool profile)  {
    ofImage image;
    image.load(fileName);
    tracker.setFaceRotation(0);
    tracker.update(image);
    return faceInflate(tracker, user, profile);
}

bool Analyzer::faceInflate(ofxFaceTracker2& tracker, User& user, bool profile) {
    View view = user.getView(profile);
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
        ofPolyline noseB = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BRIDGE);
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
        nose.addVertex(landmarks.getImagePoint(19).x, landmarks.getImagePoint(19).y, 0);
        nose.addVertex(landmarks.getImagePoint(24).x, landmarks.getImagePoint(24).y, 0);
        nose.addVertex(landmarks.getImagePoint(51).x, landmarks.getImagePoint(51).y, 0);
        view.parts[View::NOSE] = nose.getBoundingBox();
        
        ofPolyline forhead;
        forhead.addVertex(landmarks.getImagePoint(0).x, landmarks.getImagePoint(41).y, 0);
        forhead.addVertex(landmarks.getImagePoint(16).x, landmarks.getImagePoint(46).y, 0);
        forhead.addVertex(landmarks.getImagePoint(16).x, landmarks.getImagePoint(46).y - face.height/4, 0);
        forhead.addVertex(landmarks.getImagePoint(0).x, landmarks.getImagePoint(41).y - face.height/4, 0);
        view.parts[View::FORHEAD] = nose.getBoundingBox();
        
        ofRectangle head(face);
        head.y -= face.height/4;
        head.setHeight(1.25 * face.height);
        view.parts[View::HEAD] = head;
        
        if (!profile) {
            float faceLength = landmarks.getImagePoint(27).distance(landmarks.getImagePoint(8));
            //take mesures
            user.measures[View::FORHEAD] = 0.0; // no meaning to devide by highet
            user.measures[View::EYES] = view.parts[View::EYES].height / faceLength;
            user.measures[View::NOSE] = view.parts[View::NOSE].height / faceLength;
            user.measures[View::MOUTH] = view.parts[View::MOUTH].height / faceLength;
        }
        return true;
    }
    return false;
}
