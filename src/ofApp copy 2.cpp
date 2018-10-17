#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    sepiaShader.load("shaders/sepia");
 //   #ifdef _SMILE
    // All examples share data files from example-data, so setting data path to this folder
    // This is only relevant for the example apps
    //ofSetDataPathRoot(ofFile(__BASE_FILE__).getEnclosingDirectory()+"../../model/");
    learned_functions = vector<pfunct_type>(4);
    
    // Load SVM data model
    dlib::deserialize(ofToDataPath("data_ecstatic_smile.func")) >> learned_functions[0];
    dlib::deserialize(ofToDataPath("data_small_smile.func")) >> learned_functions[1];
    dlib::deserialize(ofToDataPath("data_o.func")) >> learned_functions[2];
    dlib::deserialize(ofToDataPath("data_neutral.func")) >> learned_functions[3];

    // Setup value filters for the classifer
    neutralValue.setFc(0.04);
    bigSmileValue.setFc(0.04);
    smallSmileValue.setFc(0.04);
    oValue.setFc(0.04);
//#endif
 /*
    auto devs = grabber.listDevices();
    int width;
    int height;
    
    for(auto & d : devs) {
        std::cout << d.id << ": " << d.deviceName << " // " <<
        d.formats.size() << " formats" << std::endl;
        for(auto & f : d.formats) {
            std::cout << "  " << f.width << "x" << f.height << std::endl;
            width = f.width;
            height = f.height;
            for(auto & fps : f.framerates) {
                std::cout << "    " << fps << std::endl;
            }
        }
    }
    // Setup grabber
    grabber.setPixelFormat(OF_PIXELS_MONO);
    
    // TODO:: set device id
    grabber.setup(width,height);
    //grabber.setDesiredFrameRate(30);
    */
    frontPlayer.load("me_front.mov");    // Setup tracker
    frontPlayer.setLoopState(OF_LOOP_NORMAL);
    frontPlayer.setFrame(30);
    frontPlayer.setSpeed(1.0);
    frontPlayer.play();
    
    sidePlayer.load("me_side.mov");    // Setup tracker
    sidePlayer.setLoopState(OF_LOOP_NORMAL);
    sidePlayer.setSpeed(1.0);
    frontPlayer.setFrame(30);
    sidePlayer.play();
    
    tracker.setup();
    //tracker.setFaceRotation(90);
    
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("800k");
    fileName = "testMovie";
    fileExt = ".mov";

    
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

    //ofSetWindowShape(grabber.getWidth(), grabber.getHeight()    );
    bRecording = false;
  //  ofEnableAlphaBlending();
    // allocate (no initializatoin)
    auto boundBoxes = new ofRectangle[2][8];
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

ofRectangle ofApp::getBoundingBox(ofRectangle rec1, ofRectangle rec2) {
    // get min max
    int xmin = min(rec1.getLeft(),rec2.getLeft());
    int ymin= min(rec1.getTop(), rec2.getTop());
    int xmax = max(rec1.getRight(), rec2.getRight());
    int ymax = max(rec1.getBottom(), rec2.getBottom());
    
    return ofRectangle(xmin, ymin, xmax - xmin, ymax - ymin);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofVideoPlayer *player;
    select = ofGetFrameNum() % 2;
    if (select == 0) {
        player = &frontPlayer;
        frontPlayer.setUseTexture(true);
        sidePlayer.setUseTexture(false);
    }
    else {
        player = &sidePlayer;
        sidePlayer.setUseTexture(true);
        frontPlayer.setUseTexture(false);
    }
    
    player->update();
    //frontPlayer.update();
    //sidePlayer.update();
    
    //grabber.update();
    //player.update();
    
    // Update tracker when there are new frames
    if(player->isFrameNew()){
        tracker.update(*player);

        if(tracker.size()){
            ofxFaceTracker2Instance camFace = tracker.getInstances()[0];
            ofxFaceTracker2Landmarks landmarks = camFace.getLandmarks();
            ofPolyline eyeL = landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
            ofPolyline eyeR = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
            ofPolyline nose = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BASE);
            ofPolyline noseB = landmarks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BRIDGE);
            ofPolyline mouth = landmarks.getImageFeature(ofxFaceTracker2Landmarks::OUTER_MOUTH);
            ofPolyline face = landmarks.getImageFeature(ofxFaceTracker2Landmarks::FACE_OUTLINE);
            ofPolyline leb = landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYEBROW);
            ofPolyline reb = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYEBROW);
            ofPolyline all = landmarks.getImageFeature(ofxFaceTracker2Landmarks::ALL_FEATURES);
            ofPolyline eyeLTop= landmarks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE_TOP);
            ofPolyline eyeRTop = landmarks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE_TOP);
            
            eyeL.addVertices(eyeLTop.getVertices());
            boundBoxes[0] = eyeL.getBoundingBox();
            eyeR.addVertices(eyeRTop.getVertices());
            boundBoxes[1] = eyeR.getBoundingBox();
            boundBoxes[2] = mouth.getBoundingBox();
            boundBoxes[3] = leb.getBoundingBox();
            boundBoxes[4] = reb.getBoundingBox();
            nose.addVertices(noseB.getVertices());
            boundBoxes[5] = nose.getBoundingBox();
            //boundBoxes[5] = face.getBoundingBox();
            ofRectangle allRec = face.getBoundingBox();
            allRec.setY(allRec.getTop() - allRec.getHeight()/2);
            allRec.setHeight(allRec.getHeight() * 1.5);
            boundBoxes[6] = allRec;
#ifdef _SMILE
            // Run the classifiers and update the filters
            bigSmileValue.update(learned_functions[0](makeSample()));
            smallSmileValue.update(learned_functions[1](makeSample()));
            oValue.update(learned_functions[2](makeSample()));
            neutralValue.update(learned_functions[3](makeSample()));
#endif
        }
        /*
        if(grabber.isFrameNew() && bRecording){
            bool success = vidRecorder.addFrame(grabber.getPixels());
            if (!success) {
                ofLogWarning("This frame was not added!");
            }
        }
        
        // Check if the video recorder encountered any error while writing video frame or audio smaples.
        if (vidRecorder.hasVideoError()) {
            ofLogWarning("The video recorder failed to write some frames!");
        }
        
        if (vidRecorder.hasAudioError()) {
            ofLogWarning("The video recorder failed to write some audio samples!");
        }
         */
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (select == 0) {
        drawVideo(frontPlayer, 100, 100, 600, 800);
    }
   else {
       drawVideo(sidePlayer, 100, 1000, 600, 800);
    }
    //sidePlayer.draw(1000,0 , 200 ,200);
    /*
    frontPlayer.getTexture().bind();
    // Draw camera image
    //frontPlayer.draw(0, 0);
    sepiaShader.begin();
    //sepiaShader.setUniformTexture("texture0", frontPlayer.getTexture(), 1 );
    //frontPlayer.draw(0, 0);
    int startX = 0;
    int startY = 0;
    int w = 600;
    int h = 800;
    ofScale(0.5);
    frontPlayer.getTexture().drawSubsection(startX, startY, w, h, boundBoxes[6].x - 0.5* (w - boundBoxes[6].width), boundBoxes[6].y -  0.25* (h - boundBoxes[6].height), w, h);
    //ofNoFill();
    //ofSetColor(0, 0, 0, 255); // fill color
    //ofDrawRectangle(0.5* (w - boundBoxes[6].width) - boxPadding, 0.25* (h - boundBoxes[6].height) - boxPadding, boundBoxes[6].width +2* boxPadding, boundBoxes[6].height + 2* boxPadding);
    sepiaShader.end();
    //frontPlayer.getTexture().drawSubsection(0, 0, boundBoxes[6].width, boundBoxes[6].height, boundBoxes[6].x, boundBoxes[6].y,boundBoxes[6].width, boundBoxes[6].height);
    frontPlayer.unbind();
    
    ofPath path;
    ofColor dark(0,0,0,125);
    path.setFillColor(dark);
    path.setStrokeColor(ofColor::black);
    path.setStrokeWidth(10);
    path.rectangle(startX, startY,w, h);
    //boundBoxes[6].x -= boxPadding;
    //boundBoxes[6].y -= boxPadding;
    //boundBoxes[6].width += 2*boxPadding;
    //boundBoxes[6].height += 2*boxPadding;
    int boxPadding  = 50;
    path.rectangle( 0.5* (w - boundBoxes[6].width) - boxPadding,  0.25* (h - boundBoxes[6].height) - boxPadding, 400, 500); //boundBoxes[6].width +2*boxPadding, boundBoxes[6].height +2*boxPadding );
    ofEnableAlphaBlending();
    path.draw();
    ofDisableAlphaBlending();
    */
    // Draw tracker landmarks
    tracker.drawDebug();
    
    // Draw estimated 3d pose
    tracker.drawDebugPose();
    
    ofSetHexColor(0x00ff00);
    ofNoFill();
    
    for (int i = 0; i < 7; i++)
    {
        ofRectangle rec  =  boundBoxes[i];
        ofDrawRectangle (rec);
    }
    // Draw text UI
    ofDrawBitmapStringHighlight("Framerate : "+ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapStringHighlight("Tracker thread framerate : "+ofToString(tracker.getThreadFps()), 10, 40);
    
#ifndef __OPTIMIZE__
    ofSetColor(ofColor::red);
    ofDrawBitmapString("Warning! Run this app in release mode to get proper performance!",10,60);
    ofSetColor(ofColor::white);
#endif
    
//#ifdef _SMILE
    ofPushMatrix();
    ofTranslate(0, 100);
    ofFill();
    for (int i = 0; i < 4; i++) {
        ofSetColor(255);
        
        string str;
        float val;
        switch (i) {
            case 0:
                str = "BIG SMILE";
                val = bigSmileValue.value();
                break;
            case 1:
                str = "SMALL SMILE";
                val = smallSmileValue.value();
                break;
            case 2:
                str = "OOO MOUTH";
                val = oValue.value();
                break;
            case 3:
                str = "NEUTRAL MOUTH";
                val = neutralValue.value();
                break;
        }
        
        ofDrawBitmapStringHighlight(str, 20, 0);
        ofDrawRectangle(20, 20, 300*val, 30);
        
        ofNoFill();
        ofDrawRectangle(20, 20, 300, 30);
        ofFill();
        
        ofTranslate(0, 70);
    }
    
    ofPopMatrix();
//#endif
}


void ofApp::drawVideo(ofVideoPlayer player, int x, int y, int w, int h) {
    if (player.isFrameNew()) {
    ofPushMatrix();
    ofScale(0.5);
    
    player.bind();
    sepiaShader.begin();
    player.getTexture().drawSubsection(x, y, w, h, boundBoxes[6].x - 0.5* (w - boundBoxes[6].width), boundBoxes[6].y -  0.25* (h - boundBoxes[6].height), w, h);
    sepiaShader.end();
    player.unbind();
    
    ofPath path;
    ofColor dark(0,0,0,125);
    path.setFillColor(dark);
    path.setStrokeColor(ofColor::black);
    path.setStrokeWidth(10);
    path.rectangle(x, y,w, h);
    int boxPadding  = 50;
    path.rectangle( x + 0.5* (w - boundBoxes[6].width) - boxPadding,  y + 0.25* (h - boundBoxes[6].height) - boxPadding, 300, 500); //boundBoxes[6].width +2*boxPadding, boundBoxes[6].height +2*boxPadding );
    ofEnableAlphaBlending();
    path.draw();
    ofDisableAlphaBlending();
    ofPopMatrix();
    }
}

// Function that creates a sample for the classifier containing the mouth and eyes
sample_type ofApp::makeSample(){
    auto outer = tracker.getInstances()[0].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::OUTER_MOUTH);
    auto inner = tracker.getInstances()[0].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::INNER_MOUTH);
    
    auto lEye = tracker.getInstances()[0].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
    auto rEye = tracker.getInstances()[0].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
    
    ofVec2f vec = rEye.getCentroid2D() - lEye.getCentroid2D();
    float rot = vec.angle(ofVec2f(1,0));
    
    vector<ofVec2f> relativeMouthPoints;
    
    ofVec2f centroid = outer.getCentroid2D();
    for(ofVec2f p : outer.getVertices()){
        p -= centroid;
        p.rotate(rot);
        p /= vec.length();
        
        relativeMouthPoints.push_back(p);
    }
    
    for(ofVec2f p : inner.getVertices()){
        p -= centroid;
        p.rotate(rot);
        p /= vec.length();
        
        relativeMouthPoints.push_back(p);
    }
    
    sample_type s;
    for(int i=0;i<20;i++){
        s(i*2+0) = relativeMouthPoints[i].x;
        s(i*2+1) = relativeMouthPoints[i].y;
    }
    return s;
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

void ofApp::keyReleased(int key){
   /* if (key == 'r'){
        rotation += 90;
        rotation = rotation % 360;
        tracker.setFaceRotation(rotation);
        ; // do something
    } else */ if (key == 'y'){
        rotation -= 90;
        rotation = rotation % 360;
        tracker.setFaceRotation(rotation);
    } /*else if (key == 'c'){
        ofPixels crop = grabber.getPixels();
        
        for (int i = 0; i < 8; i++)
        {
            ofRectangle rec  =  boundBoxes[i];
            ofImage img;
            int padding = 20;
            img.allocate(rec.width+padding, rec.height+padding, ofImageType::OF_IMAGE_COLOR);

            crop.cropTo(img.getPixelsRef(),rec.x- padding/2,rec.y- padding/2,rec.width+padding,rec.height+padding);
            std::string id = std::to_string(i);
            std::ostringstream oss;
            oss << "img_" << i << ".jpg";
            img.saveImage(oss.str());
            img.clear();
        }
    } else if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            //vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, grabber.getWidth(), grabber.getHeight(), 30, sampleRate, channels);
                      vidRecorder.setup(fileName+ std::to_string(sessionId)+fileExt, grabber.getWidth(), grabber.getHeight(), 30); // no audio
            //            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
            //          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
            
            // Start recording
            vidRecorder.start();
        }*/
        /*
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
         */
   // }
    else if(key=='s'){
        bRecording = false;
        vidRecorder.close();
    }
}
