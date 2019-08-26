#include "ofApp.h"
#include "ofxSmartFont.h"
#include "Globals.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // here also goes what ever else you need to setup of course
#ifdef NO_RELEASE_BERLIN
    cam.removeAllInteractions();
    cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY,OF_MOUSE_BUTTON_LEFT);
    // cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
    
    cam.enableOrtho();
    cam.enableMouseInput();
    cam.setNearClip(-1000000);
    cam.setFarClip(1000000);
    cam.setVFlip(true);
#endif
    
    float screenOffset = SCREEN_WIDTH/2.;
    ofBackground(0,0,0,255);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    ofSeedRandom();
    //glDisable(GL_DEPTH_TEST);
    
    setupFonts();
    
    if (!tablePage.load("Assets/dark.png")) //"Assets/table_page.png"
    {
        ofLogNotice("cant load Assets/dark.png"); //"Assets/table_page.png"
    }
    
    if (!mugshotPage.load("Assets/sample_sheet_vintage.png")) //"Assets/table_page.png"
    {
        ofLogNotice("cant load Assets/sample_sheet_vintage.png"); //"Assets/table_page.png"
    }
    
    if (!tableHeader.load("Assets/table_header.png")) //"Assets/table_page.png"
    {
        ofLogNotice("cant load Assets/table_header.png"); //"Assets/table_page.png"
    }
    
    if (!table_bg.load("Assets/table_grid_bg.png")) //"Assets/table_page.png"
    {
        ofLogNotice("cant load Assets/table_grid_bg.png"); //"Assets/table_page.png"
    }
    float gridScaleW = SCREEN_WIDTH / table_bg.getWidth();
    float gridScaleH = SCREEN_HEIGHT / table_bg.getHeight();
    gridScale = MIN(gridScaleW,gridScaleH);
    table_bg.resize(gridScale*table_bg.getWidth(), gridScale*table_bg.getHeight());
    
    if (!sepiaShader.load("shaders/sepia")) {
        ofLogNotice("cant load shaders/sepia");
    }
    
    frontTracker.setThreaded(false);
    profileTracker.setThreaded(false);
    frontTracker.setup("Model/shape_predictor_68_face_landmarks.dat");
    profileTracker.setup("Model/shape_predictor_5_face_landmarks.dat");
    frontTracker.setThreaded(false);
    profileTracker.setThreaded(false);
#if not defined NO_RELEASE_BERLIN
    frontTracker.setFaceDetectorImageSize(735000);
#endif
    
    
    // here you et the groups and grids
    //if you use same grid more then once but diffrent scales just change the scale before you draw;
    
    // Groups:
    // 	HEAD = 0,
    //  MOUTH
    //	NOSE,
    //	FORHEAD,
    //	EYES,
    
#ifdef MUGSHOT_IS_LEFT
    screenOffset -= SCREEN_WIDTH;
#else
    screenOffset += SCREEN_WIDTH - MG_WIDTH;
#endif
    GenericGroup::GroupFilter emptyFilter;
    //emptyFilter.beard  = true;
    Group* group = groupManager.groupFactory(
                                             View::FORHEAD, // fragment
                                             Group::GENERIC, // type of group
                                             false, // is profile?
                                             7, // user per level
                                             emptyFilter); //number of levels
    
    
    ImageGrid* a = new ImageGrid();
    
    a->setup(this,
             &sepiaShader, // shader
             group, // newly created group
             150,60, // width and height of element in image pix
             1,
             {screenOffset , MG_Y_START}, // start position for flying in image
             490.0, 876.0, // postion of grid
             gridScale); // scale
    
    grids[View::FORHEAD].push_back(a);
    
    group = groupManager.groupFactory(
                                      View::HEAD,
                                      Group::GENERIC,
                                      false,
                                      4,
                                      emptyFilter);
    ImageGrid* b = new ImageGrid();
    
    b->setup(this,
             &sepiaShader,
             group,
             110, 102,
             5,
             {screenOffset , MG_Y_START}, // start position for flying in image
             926.0, 560.0,
             gridScale);
    
    grids[View::HEAD].push_back(b);
    
    group = groupManager.groupFactory(
                                      View::NOSE,
                                      Group::GENERIC,
                                      false,
                                      5,
                                      emptyFilter);
    
    ImageGrid* c = new ImageGrid();
    c->setup(this,
             &sepiaShader,
             group,
             80, 82,
             7,
             {screenOffset , MG_Y_START}, //{1668 / 1.0, 97 / 1.0}, // start position for flying in image
             493.0, 812.0,
             gridScale);
    
    grids[View::NOSE].push_back(c);
    
    group = groupManager.groupFactory(
                                      View::MOUTH,
                                      Group::GENERIC,
                                      false,
                                      3,
                                      emptyFilter);
    
    ImageGrid* d = new ImageGrid();
    d->setup(this,
             &sepiaShader,
             group,
             110, 110,
             5,
             {screenOffset , MG_Y_START}, //{1215 / 1.0, 47 / 1.0}, // start position for flying in image
             470.0, 870.0,
             gridScale);
    
    grids[View::MOUTH].push_back(d);
    
    group = groupManager.groupFactory(
                                      View::EYES,
                                      Group::GENERIC,
                                      false,
                                      6,
                                      emptyFilter);
    
    ImageGrid* e = new ImageGrid();
    e->setup(this,
             &sepiaShader,
             group,
             196, 87,
             3,
             {screenOffset , MG_Y_START}, //{2058 / 1.0, -73 / 1.0}, // start position for flying in image
             59.0, 1828.0,
             gridScale);
    
    grids[View::EYES].push_back(e);
    
    currentUser = NULL;
    //update
    presentationUpdate.setup(&users, NULL, NULL, &frontTracker, &frontTracker, &groupManager);
    
    //	selectNextUser(true);
    currentUser = presentationUpdate.update();
    //bool randSelect = false;
    if (currentUser == NULL) {
        currentUser = getRandomUser();
        //randSelect = true;
    }
    currMugshot = new Mugshot(&sepiaShader, currentUser, this);
    mugshots.insert(mugshots.begin(), currMugshot);
    //for (int i = 0; i< NUM_MUGSHOT; i++) {
    //    mugshots.pushback(new Mugshot(&sepiaShader, currentUser);
    //}
    
    if (currentUser != NULL) {
        currentUser->isCurrent = true;
        
        //        grids[0].update();
        //        grids[1].update();
        //        grids[2].update();
        //        grids[3].update();
        //        grids[4].update();
        //grids[5].update();
        //grids[6].update();
        
        
        currMugshot->update(View::HEAD);
        currMugshot->animate(0);
        
        // if animation is done (Mugshot::ANIMATION_TIME = 1)
    }
    //if (randSelect) {
    //    currentUser = NULL; // return to null after rendering random user
    //}
    
    gridSize = 600;
    nextGrid = -1;
    //gridFbo.allocate(gridSize, gridSize);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    //gridFbo.begin();
    //ofClear(0,0,0,0);
    //gridFbo.end();
    //  ofEnableAlphaBlending();
    
    cam.removeAllInteractions();
    cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY,OF_MOUSE_BUTTON_LEFT);
    cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
    /*
    cam.enableOrtho();
    cam.setNearClip(-1000000);
    cam.setFarClip(1000000);
    cam.setVFlip(true);
    //cam.lookAt(glm::vec3(2880, 540,0));
    // cam.setPosition(2880, 540, cam.getZ());
    //cam.move(2880, 540, cam.getZ());
    camScale = cam.getScale();
    //mugshotIsLeft = false;
    */
    outputFbo.allocate(SCREEN_WIDTH*2, SCREEN_HEIGHT, GL_RGB);
    currentFeatureToFocus = View::Features::INVALID;
    
#ifdef TARGET_WIN32
#ifdef DO_WATCHDOG
    wdr = make_unique<WatchDog_Responder>(true, 9000, "../../watchdog");
#endif
#endif
}

//--------------------------------------------------------------
void ofApp::exit(){
    // frontPlayer.close();
    // profilePlayer.close();
    
    for (auto & mugshot : mugshots) {
        delete(mugshot);;
    }
    mugshots.clear();
    
    // delete users
    for(std::map<std::string, User*>::iterator itr = users.begin(); itr != users.end(); itr++)
    {
        delete (itr->second);
    }
    users.clear();
    
    for (auto & gridv : grids) {
        for (auto & grid : gridv) {
            delete grid;
        }
    }
    groupManager.exit();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    bool newUser = false;
   // cam.setScale(camScale);
    for (auto gridv : grids)
    {
        for (auto & grid : gridv) {
            grid->update();
        }
        
        /*
         // go over all the gread of same feature and randomly selcet one thst fits user filter
         ImageGrid *grid;
         // todo:: this will run for ever if we dont have any grid that poass filter
         do {
         int rand = (int)ofRandom(0, gridv.size());
         grid = gridv.at(rand);
         } while (!grid->getGroup()->getFilter().inFilter(currentUser));
         */
        // grids[View::HEAD].update();
        // grids[View::MOUTH].update();
        // grids[View::NOSE].update();
        //  grids[View::FORHEAD].update();
        //  grids[View::EYES].update();
    }
    
    if (rejectedNextUser == NORMAL) {
        selectNextUser();
        lastPresentationUpdate = ofGetElapsedTimeMillis();
    } else if (rejectedNextUser == RANDOM) {
        selectNextUser(true);
        lastUserUpdate = ofGetElapsedTimeMillis();
    }
    
    if ((ofGetElapsedTimeMillis() -  lastPresentationUpdate) > PRESENTATION_UPDATE_REFRESH) {
        
        selectNextUser();
        
        lastPresentationUpdate = ofGetElapsedTimeMillis();
        
    } else if ((ofGetElapsedTimeMillis() -  lastUserUpdate) > CURRENT_USER_REFRESH) {
        // new random user
        selectNextUser(true);
        
        lastUserUpdate = ofGetElapsedTimeMillis();
    }
    
    if (currentUser != NULL) {
        
        // only update automatically if intro-animation is done
        if (autoupdateFeatures && currMugshot->m_introAnimationDone) {
            
            if ((ofGetElapsedTimeMillis() -  lastMugshotUpdate) > MUGSHOT_REFRESH) {
                selectNewFeature();
            }
        }
    }
    
    if (curFeature == View::Features::INVALID) {
        // check for idle-update
        if ((ofGetElapsedTimeMillis() - lastIdleUpdate) > IDLE_REFRESH) {
            lastIdleFeature = selectRandomFeature(lastIdleFeature);
            setFeatureToFocus(lastIdleFeature, TIME_GIRD_MOVE_IDLE);
            lastIdleUpdate = ofGetElapsedTimeMillis();
        }
    }
    
    
    /*  frontPlayer.update();
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
     */
}

void ofApp::selectNextUser(bool random) {
    
    
    if (curFeature != View::Features::INVALID) {
        // check if an animation is running
        for (ImageGrid *grid : grids[curFeature])
        {
            if (grid->animStage != ImageGrid::AnimationStage::DONE) {
                
                // return, will try next turn...
                rejectedNextUser = random ? RANDOM : NORMAL;
                return;
            }
        }
    }
    
    
    ofLogNotice() << "next user: " << random;
    
    rejectedNextUser = NONE;
    User* user = NULL;
    
    if (random) {
        user = getRandomUser();
    } else {
        user = presentationUpdate.update();
    }
    
    if(user != NULL) {
        
        ofLogNotice() << "got a user: " << user->id;
        
        //?
        if (currentUser != nullptr) {
            currentUser->isCurrent = false;
        }
        
        currentUser = user;
        currentUser->isCurrent = true;
        
        //		grids[0].update();
        //		grids[1].update();
        //		grids[2].update();
        //		grids[3].update();
        //		grids[4].update();
        
        currMugshot = new Mugshot(&sepiaShader, currentUser, this);
        mugshots.insert(mugshots.begin(), currMugshot);
        currMugshot->update(View::Features::INVALID); // change to none
        animateMagshots();
    }
}

void ofApp::mugshotIntroAnimationDone() {
    selectNewFeature();
}

void ofApp::selectNewFeature() {
    selectFeature(currMugshot->selectNextFeature());
}

void ofApp::selectFeature(View::Features feature) {
    
    // ask mugshot for its next feature
    lastIdleFeature = curFeature;
    curFeature = feature;
    currMugshot->update(curFeature);
    
    if (curFeature != View::Features::INVALID) {
        
        setFeatureToFocus(curFeature, TIME_GIRD_MOVE_FEATURE);
        
        // we need to draw that part
        View& view = currentUser->getView(false);
        partImage = view.getImage();
        featureRect = ofRectangle(view.parts[curFeature]);
        if ((int)curFeature < 5) {
            // need to cleanup grid
            // todo check if it works
            for (ImageGrid *grid : grids[curFeature])  {
                grid->reset(); // this will call "update" when fade-out is done!
            }
            
        } else {
            ofLogError() << "grid index out of bounds!";
        }
    } else {
        partImage = ofImage();
        lastIdleUpdate = ofGetElapsedTimeMillis();
    }
    
    lastMugshotUpdate = ofGetElapsedTimeMillis();
}


void ofApp::animateMagshots() {
    float delay = 0;
    for (auto it = mugshots.begin(); it != mugshots.end(); ++it)
    {
        (*it)->animate(delay);
        delay += ANIM_DELAY;
    }
}

void ofApp::signalCurrentMugshotImageOff() {
    if (currMugshot) {
        currMugshot->resetFaceImage();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    outputFbo.begin();
    
    ofClear(ofColor::black);
    drawMugshotPage();
    drawGridPage();
    
    outputFbo.end();
    
    // draw outputFbo
    ofPushStyle();
    ofPushMatrix();
    {
        if (scaleOutput) {
            ofScale(0.4);
        }
        
        int x = 0;
        int y = 0;
        ofSetColor(ofColor::white);
        outputFbo.draw(x, y);
#ifdef DRAW_DEBUG
        ofSetColor(ofColor::red);
        ofSetLineWidth(5);
        ofNoFill();
        ofDrawRectangle(x, y, SCREEN_WIDTH, outputFbo.getHeight());
        ofDrawRectangle(x+SCREEN_WIDTH, y, SCREEN_WIDTH, outputFbo.getHeight());
#endif
        
        
#ifdef DO_BLACK_FRAME
        // draw black-bar at right
        ofSetColor(0);
        int width = 60;
        
#ifdef DO_BLACK_FRAME_ADJUST
        // adjust settings by setting define
        // use cursor-keys to adjust (very fine steps)
        ofTranslate(ofGetWidth() - width + blackBarX, ofGetHeight() / 2.0);
        ofRotateDeg(blackBarRotation);
#else
        // settings for berlin exhibition!
        ofTranslate(ofGetWidth() - width + 5.02002, ofGetHeight() / 2.0);
        ofRotateDeg(0.86);
#endif
        
        ofDrawRectangle(0, -ofGetHeight()/2.0, width, ofGetHeight());
#endif
    }
    ofPopMatrix();
    ofPopStyle();
    
    if (doShowDebug) {
        // draw debug strings
        ofDrawBitmapString(ofToString(cam.getX()) + "  " + ofToString(cam.getY()) + "  " + ofToString(cam.getZ()), 50, 50);
        ofDrawBitmapString(ofToString(cam.getFov()) + "  " + ofToString(cam.getDistance()) + "  " + ofToString(cam.getScale()), 50, 70);
        
        ofDrawBitmapString("auto feature: " + ofToString(autoupdateFeatures), 50, 110);
    }
}
/*
 void ofApp::drawBg() {
 
 //	mugshotPage.draw(800, 100);
 ofPushMatrix();
 
 #ifndef MUGSHOT_IS_LEFT
 ofTranslate(SCREEN_WIDTH, 0);
 #endif
 
 ofTranslate(30, 30);
 
 // ofTranslate(30, -1304);
 ofScale(0.66);
 table_bg.draw(0, 0);
 ofPopMatrix();
 }
 */

void ofApp::drawMugshotPage() {
    
    ofPushMatrix();
    {
#ifndef MUGSHOT_IS_LEFT
        ofTranslate(SCREEN_WIDTH, 0);
#endif
        
        vector<Mugshot*>::iterator i = mugshots.end();
        while (i != mugshots.begin())
        {
            --i;
            if (!((*i)->draw())) {
                ofLogNotice() << "delete mugshot!";
                delete(*i);
                mugshots.erase(i);
            }
        }
    }
    ofPopMatrix();
}

void ofApp::drawGridPage() {
    
    ofPushMatrix();
    {
#ifdef MUGSHOT_IS_LEFT
        ofTranslate(SCREEN_WIDTH, 0);
#endif
        
        ///ofTranslate(30, gridY);
        
        //ofPushMatrix();
        //{
        //ofScale(GRID_SCALE_TABLE);
        
        table_bg.draw(0, 0);
        //}
        // ofPopMatrix();
        // ofScale(GRID_SCALE_TABLE);//
        
        // be aware of render order!
        //grids[View::HEAD].draw(2055, 1266); // HEAD
        //grids[View::MOUTH].draw(1403, 849); // MOUTH
        //grids[View::NOSE].draw(748, 1230); // NOSE
        //grids[View::FORHEAD].draw(494, 904); // FOREHEAD
        //grids[View::EYES].draw(90, 2770); // EYES
        
        // unscaled version
        for (auto & gridv : grids)
        {
            for (auto & grid : gridv) {
                grid->draw();
            }
        }
        //grids[View::HEAD].draw();//2055, 1266); // HEAD
        //grids[View::MOUTH].draw();//1403, 849); // MOUTH
        //grids[View::NOSE].draw();//748, 1230); // NOSE
        //grids[View::FORHEAD].draw();//494, 904); // FOREHEAD
        //grids[View::EYES].draw();//90, 2770); // EYES
        
        
        //    tableHeader.draw(0, 0);
        //    grids[0].draw(0, tableHeader.getHeight());
        //    grids[1].draw(grids[0].getSize()[0]+25, tableHeader.getHeight());
        //    grids[2].draw(grids[0].getSize()[0] + grids[1].getSize()[0]+50, tableHeader.getHeight());
        //    grids[3].draw(grids[0].getSize()[0] + grids[1].getSize()[0]+ grids[2].getSize()[0]+80, tableHeader.getHeight());
        //    grids[3].draw(50, 780);
        //    grids[4].draw(0, tableHeader.getHeight()+grids[0].getSize()[1]+5);
        //    grids[5].draw(grids[0].getSize()[0]+25, tableHeader.getHeight()+grids[1].getSize()[1]+5);
        //    grids[6].draw(grids[0].getSize()[0] + grids[1].getSize()[0]+50, tableHeader.getHeight()+grids[2].getSize()[1]+5);
        //    grids[4].draw(845, 910);
        //    grids[5].draw(1630, 1070);
        //    grids[6].draw(2290, 53);
        
        //    ofTranslate(0, 0);
        //    ofScale(0.66);
        
        //    grids[0].draw(50, 50);
        //    grids[1].draw(840, 46);
        //    grids[2].draw(1630, 50);
        //    grids[3].draw(50, 780);
        //    grids[4].draw(845, 910);
        //    grids[5].draw(1630, 1070);
        //    grids[6].draw(2290, 53);
    }
    ofPopMatrix();
}


User* ofApp::getRandomUser() {
    int randomUser = (int)floor(ofRandom(users.size()));
    int index = 0;
    for (UserMap::iterator it=users.begin(); it!=users.end(); ++it, index++) {
        if (index == randomUser)
        {
            return (it->second);
        }
    }
    return NULL;
}
/*///
 void ofApp::drawVideo(ofVideoPlayer& player, ofRectangle& face, int x, int y, int w, int h) {
 if (player.isFrameNew()) {
 ofPushMatrix();
 ofScale(0.5);
 int boxSize = 400;
 ofColor dark(0,0,0,125);
 ofPath path;
 *///
/*
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
 */
/*///
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
 *///
void ofApp::keyReleased(int key){
    
    if (key == OF_KEY_UP) {
        
        blackBarRotation += 0.05;
        ofLogNotice() << "rot: " << blackBarRotation;
    }
    else if (key == OF_KEY_DOWN) {
        
        blackBarRotation -= 0.05;
        ofLogNotice() << "rot: " << blackBarRotation;
    }
    else if (key == OF_KEY_LEFT) {
        
        blackBarX -= 0.1;
        ofLogNotice() << "blackBarX: " << blackBarX;
    }
    else if (key == OF_KEY_RIGHT) {
        
        blackBarX += 0.1;
        ofLogNotice() << "blackBarX: " << blackBarX;
    }
    else if (key == 'f') {
        ofToggleFullscreen();
    }
    else if (key == 'a') {
        for (auto & gridv : grids )
        {
            for (auto & grid : gridv) {
                grid->resetLoading();
            }
        }
    } else if (key == 'd') {
        /*
         glm::vec3 scaleOut(1., 1., 1.);
         glm::vec3 scaleIn(.5, .5 , .5);
         tweenManager.clear();
         auto t0 = tweenManager.addTween(camScale, scaleOut,scaleIn, 5, 0 , TWEEN::Ease::Sinusoidal::Out);
         auto t1 = tweenManager.addTween(camScale, scaleIn ,scaleOut, 5, 0 ,TWEEN::Ease::Sinusoidal::In);
         t0->addChain(t1);
         t0->autoDelete( false );
         t1->autoDelete( false );
         t0->start();
         // glm::vec3 scale= cam.getScale();
         // cam.setScale(glm::vec3(scale.x - 0.01, scale.y - 0.01, scale.z - 0.010));
         */
    } else if (key == 'w') {
        autoupdateFeatures = !autoupdateFeatures;
    } else if (key == 's') {
        scaleOutput = !scaleOutput;
    } else if (key == '1') {
        selectFeature(View::Features::HEAD);
    } else if (key == '2') {
        selectFeature(View::Features::FORHEAD);
    } else if (key == '3') {
        selectFeature(View::Features::NOSE);
    } else if (key == '4') {
        selectFeature(View::Features::MOUTH);
    } else if (key == '5') {
        selectFeature(View::Features::EYES);
    } else if (key == 'u') {
        // select new random user
        selectNextUser();
    }
    else if (key == 'r') {
        // reset features
        if (currMugshot != nullptr) {
            currMugshot->resetFeatures();
        }
    }
    else if (key == 'g') {
        doShowDebug = !doShowDebug;
    }
}

void ofApp::setFeatureToFocus(View::Features feature, float animTime) {
    currentFeatureToFocus = feature;
    float toY = View::getLocationForFeature(currentFeatureToFocus);
    float toX = View::getLocationForFeature(currentFeatureToFocus);
    
    /*
     if (gridTween != nullptr) {
     gridTween->clear();
     }
     
     gridTween = tweenManager.addTween(gridY,
     gridY,
     toY,
     animTime,
     TIME_GIRD_MOVE_DELAY,
     TWEEN::Ease::Quadratic::Out);
     gridTween->start();
     */
}

void ofApp::presentationUpdater()
{
    User* user = presentationUpdate.update();
    if (user != NULL) {
        currentUser = user;
        //frontPlayer.setUseTexture(true);
        //profilePlayer.setUseTexture(true);
        // TODO::deal with start frame
        //frontPlayer.play();
        //profilePlayer.play();
        //frontPlayer.firstFrame();
    }
}

// font setup here!!
void ofApp::setupFonts()
{
    static const string FONT_DIR = "Assets/fonts/";
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Med.otf", 14, "AmericanTypewriter");
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Med.otf", 12, "AmericanTypewriter12");
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Bold.otf", 14, "AmericanTypewriter700");
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Light.otf", 14, "AmericanTypewriter300");
    ofxSmartFont::add(FONT_DIR + "Bodoni Poster.otf", 16, "BodonPoster");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 600.ttf", 18, "CrimsonText600");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 600italic.ttf", 18, "CrimsonText600I");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 700.ttf", 18, "CrimsonText700");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 700italic.ttf", 18, "CrimsonText700I");
    ofxSmartFont::add(FONT_DIR + "Crimson Text italic.ttf", 18, "CrimsonTextI");
    ofxSmartFont::add(FONT_DIR + "Crimson Text regular.ttf", 12, "CrimsonRegular");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 700.ttf", 12, "CrimsonText700Mugshot");
}

View::Features ofApp::selectRandomFeature(View::Features feature) {
    // TODO: we need to avoid a feature is animated twice!
    int f = feature;
    while (f == feature) {
        f = (int)(floor(ofRandom(5)));
    }
    return (View::Features)f;
}
