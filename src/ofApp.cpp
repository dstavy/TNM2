#include "ofApp.h"

const long ofApp::VIDEO_GRID_REFRESH = 2000;
const long ofApp::PRESENTATION_UPDATE_REFRESH = 3000000;

//--------------------------------------------------------------
void ofApp::setup(){
    //ofBackground(0,0,0,0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    //glDisable(GL_DEPTH_TEST);
    
    if (!tablePage.load("Assets/table_page.png"))
    {
        ofLogNotice("cant load Assets/table_page.png");
    }
    if (!sepiaShader.load("Shaders/sepia")) {
        ofLogNotice("cant load Shaders/sepia");
    }

    //frontPlayer.load("me_front.mov");    // Setup tracker
    frontPlayer.setLoopState(OF_LOOP_NORMAL);
    frontPlayer.setFrame(30);
    frontPlayer.setSpeed(1.0);
    //frontPlayer.play();
    
    //profilePlayer.load("me_side.mov");    // Setup tracker
    profilePlayer.setLoopState(OF_LOOP_NORMAL);
    profilePlayer.setSpeed(1.0);
    frontPlayer.setFrame(30);
    //profilePlayer.play();
    
    frontTracker.setup("Model/shape_predictor_68_face_landmarks.dat");
    profileTracker.setup("Model/shape_predictor_68_face_landmarks.dat");
    
    //frontTracker.setFaceRotation(90);
    //fprofileTracker.setFaceRotation(90);
  
    Group* g = groupManager.groupFactory(View::EYES, Group::GENERIC, 7);
    currentUser = NULL;
    presentationUpdate.setup(users, &frontPlayer, &profilePlayer, &frontTracker, &profileTracker, &groupManager);
    currentUser = presentationUpdate.update();
    
    grids[0].setup(&sepiaShader,g, false, 165, 80, 4, "test", 1.0);
    grids[0].update();
    //Group* g = groupManager.getGroup(View::HEAD, Group::GENERIC);
    //vector<User*> gridUsers(0);
    //g->getGridUsers(1, gridUsers);

    gridSize = 600;
    nextGrid = -1;
    gridFbo.allocate(gridSize, gridSize);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    //gridFbo.begin();
    //ofClear(0,0,0,0);
    //gridFbo.end();
  //  ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit(){
    frontPlayer.close();
    profilePlayer.close();
    
    // delete users
    for(std::map<std::string, User*>::iterator itr = users.begin(); itr != users.end(); itr++)
    {
        delete (itr->second);
    }
    
    groupManager.exit();
}

//--------------------------------------------------------------
void ofApp::update(){
    if ((ofGetElapsedTimeMillis() -  lastPresentationUpdate) > PRESENTATION_UPDATE_REFRESH) {
        lastPresentationUpdate = ofGetElapsedTimeMillis();
        presentationUpdater();
    }
    if (currentUser != NULL) {
        frontPlayer.update();
        profilePlayer.update();
    
        // Update tracker when there are new frames
        if(frontPlayer.isFrameNew()){
            frontTracker.update(frontPlayer);
            if(frontTracker.size()) {
                ofxFaceTracker2Instance camFace = frontTracker.getInstances()[0];
                frontFace = camFace.getBoundingBox();
                frontFace.setY(frontFace.getTop() -  frontFace.getHeight()/2);
                frontFace.setHeight(frontFace.getHeight() * 1.5);
            }
        }
        // Update tracker when there are new frames
        if(profilePlayer.isFrameNew()){
            profileTracker.update(frontPlayer);
            if(profileTracker.size()) {
                ofxFaceTracker2Instance camFace = profileTracker.getInstances()[0];
                profileFace = camFace.getBoundingBox();
                profileFace.setY(profileFace.getTop() -  profileFace.getHeight()/2);
                profileFace.setHeight(profileFace.getHeight() * 1.5);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (currentUser != NULL) {
        //if (frontTracker.size()) {
        drawVideo(frontPlayer, frontFace, 100, 100, 600, 600);
        //}
       //if (profileTracker.size()) {
        drawVideo(profilePlayer, profileFace, 100, 1000, 600, 600);
       //}
    }
    drawTablePage();
    //drawGrid(View::FORHEAD, Group::GENERIC, false, 165, 80, 142, 118, 4);
    grids[0].draw(600, 80);
}

void ofApp::drawTablePage() {
    tablePage.draw(0,0, 1920,1080);
}

void ofApp::drawVideo(ofVideoPlayer& player, ofRectangle& face, int x, int y, int w, int h) {
    if (player.isFrameNew()) {
    ofPushMatrix();
    ofScale(0.5);
    int boxSize = 400;
    ofColor dark(0,0,0,125);
        ofPath path;
        
    player.bind();
    sepiaShader.begin();
    player.getTexture().drawSubsection(x, y, w, h, face.x - 0.5* (w - face.width), face.y -  0.25* (h - face.height), w, h);
        if ((ofGetElapsedTimeMillis() -  lastGridUpdate) > VIDEO_GRID_REFRESH) {
            lastGridUpdate = ofGetElapsedTimeMillis();
            ofPoint p = getGridLocation();
         //   ofTranslate(1000, 100);
          gridFbo.begin();
           // ofPath path;
            //ofFill();
            //path.setFillColor(dark);
            //path.setStrokeColor(ofColor::black);
            //path.setStrokeWidth(10);
            //path.rectangle(0, 0, gridSize, gridSize);
            //path.rectangle(gridSize/3, gridSize/3, gridSize/3, gridSize/3);
            //ofEnableAlphaBlending();
            //ofEnableAlphaBlending() ;
           // glBlendFunc( GL_ONE, GL_ZERO ) ;
          // path.draw();
           // ofDisableAlphaBlending();
            //float aspect = boundBoxes[6].height /boundBoxes[6].width;
            
            player.getTexture().drawSubsection(p.x, p.y, gridSize/3, gridSize/3, face.x - 0.5*(face.height- face.width), face.y , face.height, face.height);
           gridFbo.end();
          gridFbo.draw ( 800, 100);
        }
    sepiaShader.end();
    player.unbind();
    
    //ofPath path;
    path.clear();
    path.setFillColor(dark);
    path.setStrokeColor(ofColor::black);
    path.setStrokeWidth(10);
    path.rectangle(x, y,w, h);
    int boxPadding  = 50;
    path.rectangle( x + 0.5 * (w - boxSize),  y + 0.25 * (h - boxSize), boxSize, boxSize); //boundBoxes[6].width +2*boxPadding, boundBoxes[6].height +2*boxPadding );
    ofEnableAlphaBlending();
    path.draw();
    ofDisableAlphaBlending();
    ofPopMatrix();
    }
}

ofPoint ofApp::getGridLocation() {
    ofPoint p(0,0);
    int recSize = gridSize/3;
    nextGrid = (++nextGrid % 8);
    switch (nextGrid) {
        case 0:
        case 1:
        case 2:
            return ofPoint(nextGrid * recSize,0);
        case 3:
            return ofPoint(2 * recSize, recSize);
        case 4:
        case 5:
        case 6:
            return ofPoint((6 -nextGrid) * recSize, 2 * recSize);
        case 7:
            return ofPoint(0 , recSize);
    }
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
        frontTracker.setFaceRotation(rotation);
        profileTracker.setFaceRotation(rotation);
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
}

void ofApp::presentationUpdater()
{
    User* user = presentationUpdate.update();
    if (user != NULL) {
        currentUser = user;
        frontPlayer.setUseTexture(true);
        profilePlayer.setUseTexture(true);
        // TODO::deal with start frame
        frontPlayer.play();
        profilePlayer.play();
        //frontPlayer.firstFrame();
    }
}
