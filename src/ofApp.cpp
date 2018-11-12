#include "ofApp.h"
#include "ofxSmartFont.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
    ofBackground(0,0,0,255);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
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
    
    frontTracker.setThreaded(false);
    profileTracker.setThreaded(false);
    frontTracker.setup("Model/shape_predictor_68_face_landmarks.dat");
    profileTracker.setup("Model/shape_predictor_68_face_landmarks.dat");
    frontTracker.setThreaded(false);
    profileTracker.setThreaded(false);
    
    //frontTracker.setFaceRotation(90);
    //fprofileTracker.setFaceRotation(90);
    
    // here you et the groups and grids
    //if you use same grid more then once but diffrent scales just change the scale before you draw;
    
    // Groups:
    // EYES
    // NOSE
    // MOUTH
    // FORHEAD
    // HEAD
    
    Group* group = groupManager.groupFactory(
                                         View::FORHEAD, // fragment
                                         Group::GENERIC, // type of group
                                         false, // is profile?
                                         7); //number of levels
    grids[0].setup(&sepiaShader, // shader
                   group, // newly created group
                   204, 112, // width and height of element
                   1, // user per level
                   1); // scale
	
   // grids[0].Y_SPACING = 0;
    //grids[0].SCORE_AREA_HEIGHT = 5;
    
    //
    //    g = groupManager.groupFactory(
    //                                  View::NOSE,
    //                                  Group::GENERIC,
    //                                  false,
    //                                  4);
    //    grids[1].setup(&sepiaShader,g, 105, 145, 6, 1);
    
    group = groupManager.groupFactory(
                                  View::HEAD,
                                  Group::GENERIC,
                                  false,
                                  4);
    grids[1].setup(&sepiaShader, group, 110, 110, 5, 1);
    
    
    group = groupManager.groupFactory(
                                  View::NOSE,
                                  Group::GENERIC,
                                  false,
                                  5);
    grids[2].setup(&sepiaShader, group, 80, 80, 7, 1);
    
    group = groupManager.groupFactory(
                                  View::MOUTH,
                                  Group::GENERIC,
                                  false,
                                  3);
    grids[3].setup(&sepiaShader, group, 110, 110, 5, 1);
    
    group = groupManager.groupFactory(
                                  View::EYES,
                                  Group::GENERIC,
                                  false,
                                  6);
    grids[4].setup(&sepiaShader, group, 196, 87, 3, 1);
    
    currentUser = NULL;
    //update
    presentationUpdate.setup(&users, &frontPlayer, &profilePlayer, &frontTracker, &profileTracker, &groupManager);
    currentUser = presentationUpdate.update();
    //bool randSelect = false;
    if (currentUser == NULL) {
        currentUser = getRandomUser();
        //randSelect = true;
    }
    currMugshot = new Mugshot(&sepiaShader, currentUser);
    mugshots.insert(mugshots.begin(), currMugshot);
    //for (int i = 0; i< NUM_MUGSHOT; i++) {
    //    mugshots.pushback(new Mugshot(&sepiaShader, currentUser);
    //}
    
    if (currentUser != NULL) {
        currentUser->isCurrent = true;
        grids[0].update();
        grids[1].update();
        grids[2].update();
        grids[3].update();
        grids[4].update();
        //grids[5].update();
        //grids[6].update();
        
        currMugshot->animate(0);
        currMugshot->update(View::HEAD);
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
    
    cam.enableOrtho();
    cam.setNearClip(-1000000);
    cam.setFarClip(1000000);
    cam.setVFlip(true);
    //cam.lookAt(glm::vec3(2880, 540,0));
   // cam.setPosition(2880, 540, cam.getZ());
    //cam.move(2880, 540, cam.getZ());
    camScale = cam.getScale();
	mugshotIsLeft = false;
	
	outputFbo.allocate(SCREEN_WIDTH*2, 1080, GL_RGB);
	currentFeatureToFocus = View::Features::INVALID;
}

//--------------------------------------------------------------
void ofApp::exit(){
    frontPlayer.close();
    profilePlayer.close();
    
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
    
    groupManager.exit();
}

//--------------------------------------------------------------
void ofApp::update(){
    User* user = NULL;
    bool newUser = false;
    cam.setScale(camScale);
	
    if ((ofGetElapsedTimeMillis() -  lastPresentationUpdate) > PRESENTATION_UPDATE_REFRESH) {
        lastPresentationUpdate = ofGetElapsedTimeMillis();
        user = presentationUpdate.update();
    } else if ((ofGetElapsedTimeMillis() -  lastUserUpdate) > CURRENT_USER_REFRESH) {
        user = getRandomUser();
    }
	
    if(user != NULL) {
        currentUser->isCurrent = false;
        currentUser = user;
        currentUser->isCurrent = true;
        grids[0].update();
        grids[1].update();
        grids[2].update();
        grids[3].update();
        grids[4].update();
        
        currMugshot = new Mugshot(&sepiaShader, currentUser);
        mugshots.insert(mugshots.begin(), currMugshot);
        currMugshot->update(View::HEAD); // change to none
        animateMagshots();
        lastUserUpdate = ofGetElapsedTimeMillis();
    }
    
    if (currentUser != NULL) {
        if ((ofGetElapsedTimeMillis() -  lastMugshotUpdate) > MUGSHOT_REFRESH) {
            lastMugshotUpdate = ofGetElapsedTimeMillis();
            curFeature = selectNextFeature(curFeature);
            currMugshot->update(curFeature);
            
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

void ofApp::animateMagshots() {
    float delay = 0;
    for (auto it = mugshots.begin(); it != mugshots.end(); ++it)
    {
        (*it)->animate(delay);
        delay += ANIM_DELAY;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

	outputFbo.begin();
	ofClear(ofColor::aquamarine);
	
	
//    drawBg();
	
    if (currentUser != NULL) {

        /*      //if (frontTracker.size()) {
         drawVideo(frontPlayer, frontFace, 100, 100, 600, 600);
         //}
         //if (profileTracker.size()) {
         drawVideo(profilePlayer, profileFace, 100, 1000, 600, 600);
         //}
         }
         */

		drawMugshotPage();
		
//		cam.begin();
		
		drawGridPage();
		/*
		 ofPushMatrix();
		 //ofScale(0.5, 0.5);;
		 //ofTranslate(1920, 0);
		 grids[0].draw(50, 50);
		 grids[1].draw(450, 50);
		 grids[2].draw(850, 50);
		 grids[3].draw(50, 410);
		 grids[4].draw(450, 500);
		 grids[5].draw(850, 600);
		 grids[6].draw(1200, 50);
		 ofPopMatrix();
		 */
		
//		cam.end();
    }
	
	
    outputFbo.end();
	
	// draw outputFbo
	ofPushStyle();
	ofPushMatrix();
	ofScale(0.4);

	int x = 100;
	int y = 100;
	ofSetColor(ofColor::white);
	outputFbo.draw(x, y);
	
	ofSetColor(ofColor::red);
	ofSetLineWidth(5);
	ofNoFill();
	ofDrawRectangle(x, y, SCREEN_WIDTH, outputFbo.getHeight());
	ofDrawRectangle(x+SCREEN_WIDTH, y, SCREEN_WIDTH, outputFbo.getHeight());
	
	ofPopMatrix();
	ofPopStyle();
	
	// draw debug strings
//	ofDrawBitmapString(ofToString(cam.getX()) + "  " + ofToString(cam.getY()) + "  " + ofToString(cam.getZ()), 50, 50);
//	ofDrawBitmapString(ofToString(cam.getFov()) + "  " + ofToString(cam.getDistance()) + "  " + ofToString(cam.getScale()), 50, 70);
	
	// ofDrawBitmapString(ofToString(cam.sets) + "  " + ofToString(cam.getY()) + "  " + //ofToString(cam.getZ()), 50, 90);
}

void ofApp::drawBg() {
	
//	mugshotPage.draw(800, 100);

	ofPushMatrix();
	
	if (mugshotIsLeft) {
		ofTranslate(SCREEN_WIDTH, 0);
	}
	
	ofTranslate(30, 30);
	
	// ofTranslate(30, -1304);
	ofScale(0.66);
	table_bg.draw(0, 0);
	ofPopMatrix();
}

void ofApp::drawMugshotPage() {
	
	ofPushMatrix();
	
	if (!mugshotIsLeft) {
		ofTranslate(SCREEN_WIDTH, 0);
	}
	
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
  
    //mugshotPage.draw(500, 0);
  //  for (int i =  mugshots.size() -1; i >= 0; i--) {
    //    mugshots[i]->draw();
    //}
	
	ofPopMatrix();
}

void ofApp::drawGridPage() {
	
    ofPushMatrix();
	
	if (mugshotIsLeft) {
		ofTranslate(SCREEN_WIDTH, 0);
	}
	
	ofTranslate(30, gridY);
	
    // ofTranslate(30, -1304);
    ofScale(0.66);
	
	
    table_bg.draw(0, 0);
	
    grids[0].draw(494, 904); // FOREHEAD
    grids[1].draw(2055, 1266); // HEAD
    grids[2].draw(748, 1230); // NOSE
    grids[3].draw(1403, 849); // MOUTH
    grids[4].draw(90, 2770); // EYES
    
    
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

void ofApp::drawVideo(ofVideoPlayer& player, ofRectangle& face, int x, int y, int w, int h) {
    if (player.isFrameNew()) {
        ofPushMatrix();
        ofScale(0.5);
        int boxSize = 400;
        ofColor dark(0,0,0,125);
        ofPath path;
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
         */
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
    if (key == 'f') {
        ofToggleFullscreen();
    }
    else if (key == 'a') {
        grids[0].resetLoading(); // FOREHEAD
        grids[1].resetLoading(); // HEAD
        grids[2].resetLoading(); // NOSE
        grids[3].resetLoading(); // MOUTH
        grids[4].resetLoading(); // EYES
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
	} else if (key == '1') {
		setFeatureToFocues(View::Features::HEAD);
	} else if (key == '2') {
		setFeatureToFocues(View::Features::FORHEAD);
	} else if (key == '3') {
		setFeatureToFocues(View::Features::NOSE);
	} else if (key == '4') {
		setFeatureToFocues(View::Features::MOUTH);
	} else if (key == '5') {
		setFeatureToFocues(View::Features::EYES);
	} else if (key == '6') {
		setFeatureToFocues(View::Features::LEFT_EAR);
	} else if (key == '7') {
		setFeatureToFocues(View::Features::RIGHT_EAR);
	}
}

void ofApp::setFeatureToFocues(View::Features feature) {
	currentFeatureToFocus = feature;
	float toY = View::getLocationForFeature(currentFeatureToFocus);
	
	int anim_time = 2;
	int delay = 0;
	
	if (gridTween != nullptr) {
		gridTween->clear();
	}
	
	gridTween = tweenManager.addTween(gridY, gridY, toY, anim_time, delay, TWEEN::Ease::Sinusoidal::Out);
	gridTween->start();
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

// font setup here!!
void ofApp::setupFonts()
{
    static const string FONT_DIR = "Assets/fonts/";
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Med.otf", 14, "AmericanTypewriter");
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Bold.otf", 14, "AmericanTypewriter700");
    ofxSmartFont::add(FONT_DIR + "AmericanTypewriterStd-Light.otf", 14, "AmericanTypewriter300");
    ofxSmartFont::add(FONT_DIR + "Bodoni Poster.otf", 16, "BodonPoster");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 600.ttf", 18, "CrimsonText600");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 600italic.ttf", 18, "CrimsonText600I");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 700.ttf", 18, "CrimsonText700");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 700italic.ttf", 18, "CrimsonText700I");
    ofxSmartFont::add(FONT_DIR + "Crimson Text italic.ttf", 18, "CrimsonTextI");
    ofxSmartFont::add(FONT_DIR + "Crimson Text regular.ttf", 18, "CrimsonRegular");
    ofxSmartFont::add(FONT_DIR + "Crimson Text 700.ttf", 20, "CrimsonText700Mugshot");
}

View::Features ofApp::selectNextFeature(View::Features feature) {
	// TODO: we need to avoid a feature is animated twice!
    int f = feature;
    while (f == feature) {
        f = (int)(floor(ofRandom(5)));
    }
    return (View::Features)f;
}
