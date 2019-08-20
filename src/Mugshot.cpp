//
//  Mugshotx.cpp
//  TNM2
//
//  Created by Dan Stavy on 26/10/2018.
//

#include "Mugshot.hpp"
#include "ImageGrid.hpp"
#include "FontUtil.hpp"

#include "Utils.hpp"
#include "ofApp.h"
#include "Globals.h"


const static ofPoint height_pos = {140, 85, 0};
const static ofPoint eng_height_pos = {124, 115, 0};
const static ofPoint out_a_pos = {140, 145, 0};
const static ofPoint trunk_pos = {124, 175, 0};

const static ofPoint head_length_pos = {280, 85, 0};
const static ofPoint head_width_pos = {280, 115, 0};
const static ofPoint cheek_width_pos = {280, 145, 0};
const static ofPoint r_ear_length_pos = {280, 175, 0};

const static ofPoint cubit_pos = {428, 175, 0};

const static ofPoint age_pos = {666, 85, 0};
const static ofPoint apparent_age_pos = {720, 115, 0};


const static ofPoint forehead_height_pos = {121, 692, 0};
const static ofPoint forehead_width_pos = {123, 725, 0};

const static ofPoint chin_pos = {438, 760, 0};


const static ofPoint examined_1_pos = {580, 790, 0};
const static ofPoint examined_2_pos = {700, 795, 0};

const static ofPoint examined_at_pos = {139, 794, 0};

static ofImage bgImage;

static void myCompleteFunc(void* ptr) {
	if (ptr) {
		Mugshot* ms = (Mugshot*)ptr;
		ms->introAnimationDone();
	}
}


Mugshot::Mugshot(ofShader* shader, User* user, ofApp* app) {
	
	this->shader = shader;
	this->user = user;
	this->appcontroller = app;
	
	if (!bgImage.isAllocated()) {
		bgImage.load("Assets/sample_sheet.png");
	}
	
	firstTime = true;
	x = START_X;
    y = -TOTAL_WIDTH;//ofGetHeight();

	
	bgFbo.allocate(bgImage.getWidth(), bgImage.getHeight());
	drawBackground(user);
	
	// darkened face
	facecutFbo.allocate(MG_WIDTH, MG_HEIGHT, GL_RGB);
	facecutFbo.getTexture().getTextureData().bFlipTexture = true;
	
	//----------------------------------------
	// render overlay fbo
	// letter overlay
	faceoverlayFbo.allocate(MG_WIDTH, MG_HEIGHT);
	
	//----------------------------------------
	// setup fbo
	fbo.allocate(bgImage.getWidth(), bgImage.getHeight());
    // Clear the FBO's
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
	
    resetFeatures();
}

void Mugshot::resetFeatures() {
	partsToVisit.clear();
	for (int i=4; i>=0; i--) {
		partsToVisit.push_back(i);
	}
}

View::Features Mugshot::selectNextFeature() {
	
	if (partsToVisit.size() == 0) {
		resetFeatures();
//		return View::Features::INVALID;
	}
	
	if (false) {
		View::Features new_feature = (View::Features)partsToVisit[0];
		partsToVisit.erase (partsToVisit.begin());
		return new_feature;
	} else {
		
		int id = (int)(floor(ofRandom(partsToVisit.size())));
		View::Features new_feature = (View::Features)partsToVisit[id];
		partsToVisit.erase (partsToVisit.begin()+id);
		
		return new_feature;
	}
}

void Mugshot::drawBackground(User* user) {
	
	bgFbo.begin();
	{
		ofClear(0,0,0,0);
		ofEnableAlphaBlending();
		
		bgImage.draw(0, 0);
		// draw information text
		
		shared_ptr<ofxSmartFont> font = ofxSmartFont::get("AmericanTypewriter");
		
		ofSetColor(107, 55, 143); // #6B378F
		
		if (user != NULL) {
			
			View& view = user->getView(false);
			ofRectangle faceRec(view.parts[View::HEAD]);
			float face_aspect = faceRec.width / faceRec.height;
			
			// head height
			float headHeight = user->headHeight;
			
			// calc head width
			// head width: get from face bounding-box
			//...
			float headWidth = headHeight * face_aspect;
			
			
			// cubit: ellbow (from json) to wrist+20% (for hand)
			float cubit = user->lowerArm; // + 20%
			
	//		shouldersWidth;

			
			// forehead hight: tkae a portion of the headheight
			float foreheadHeight = 0.0;
			
			// forehead width: take portion
			float foreheadWidth = 0.0;
			
			// chin: calc from head height...
			float chin = 0.0;
			
			
			ofLogNotice() << "user->totalHeight: " << user->totalHeight;
			ofLogNotice() << "user->torsoLength: " << user->torsoLength;
			ofLogNotice() << "user->headHeight: " << headHeight;
			ofLogNotice() << "user->headWidth: " << headWidth;
			ofLogNotice() << "cubit: " << cubit;
			ofLogNotice() << "foreheadHeight: " << foreheadHeight;
			ofLogNotice() << "foreheadWidth: " << foreheadWidth;
			ofLogNotice() << "chin: " << chin;
			
			
			if (user->totalHeight > 0.0)
				font->draw(meterToCMDashMM(user->totalHeight), height_pos.x, height_pos.y);
	//		font->draw(meterToCMDashMM(0.0), eng_height_pos.x, eng_height_pos.y);
	//		font->draw(meterToCMDashMM(0.0), out_a_pos.x, out_a_pos.y);
			
			if (user->torsoLength > 0.0)
				font->draw(meterToCMDashMM(user->torsoLength), trunk_pos.x, trunk_pos.y);
			
			if (headHeight > 0.0)
				font->draw(meterToCMDashMM(user->headHeight), head_length_pos.x, head_length_pos.y);
			
			if (headWidth > 0.0)
				font->draw(meterToCMDashMM(headWidth), head_width_pos.x, head_width_pos.y);
	//		font->draw(meterToCMDashMM(0.0), cheek_width_pos.x, cheek_width_pos.y);
	//		font->draw(meterToCMDashMM(0.0), r_ear_length_pos.x, r_ear_length_pos.y);
			
			if (cubit > 0.0)
				font->draw(meterToCMDashMM(cubit), cubit_pos.x, cubit_pos.y);
			
	//		font->draw(meterToCMDashMM(1.234567), age_pos.x, age_pos.y);
	//		font->draw(meterToCMDashMM(1.234567), apparent_age_pos.x, apparent_age_pos.y);
			
			if (foreheadHeight)
				font->draw(meterToCMDashMM(foreheadHeight), forehead_height_pos.x, forehead_height_pos.y);
			
			if (foreheadWidth > 0.0) {
				font->draw(meterToCMDashMM(foreheadWidth), forehead_width_pos.x, forehead_width_pos.y);
			}
			
			if (chin > 0.0) {
				font->draw(meterToCMDashMM(chin), chin_pos.x, chin_pos.y);
			}
			

			shared_ptr<ofxSmartFont> font_smaller = ofxSmartFont::get("AmericanTypewriter12");
			
			// 2018_11_13_23_28_03_647_0
			string date_string = ofGetTimestampString("%H:%M:%S");
			if (user->id.length() >= 10) {
				date_string = user->id.substr(0, 10);
			}
			font_smaller->draw(date_string, examined_1_pos.x, examined_1_pos.y);
			
			string time_String = ofGetTimestampString("%F");
			if (user->id.length() >= 20) {
				date_string = user->id.substr(11, 8);
			}	
			font_smaller->draw(date_string, examined_2_pos.x, examined_2_pos.y);
			
			font->draw("Berlin-Mitte", examined_at_pos.x, examined_at_pos.y);
		} else {
			ofLogNotice() << "mugshot no user!";
		}
	}
	bgFbo.end();
	
}

void Mugshot::calculateRectangles() {
	
	View& view = user->getView(false);
	
	if (view.isActive()) {
		
		ofRectangle faceRec(view.parts[View::HEAD]);
		// enlarge face
		faceRec.x = MAX(0,  faceRec.x - 0.1 * faceRec.width);
		faceRec.y = MAX(0,  faceRec.y - 0.2 * faceRec.height);
		faceRec.width = MIN (faceRec.width *1.2, face.getWidth() - faceRec.x);
		faceRec.height = MIN (faceRec.height *1.6, face.getHeight() - faceRec.y);
		
		faceBox = ofRectangle(ImageGrid::adjustAspectRatio(faceRec, MG_ASPECT_RATIO));
		partScale = ofPoint((float)MG_WIDTH/faceBox.width, (float)MG_HEIGHT/faceBox.height);
		
		if (currentFeature != View::Features::INVALID) {
			featureRect = ofRectangle(view.parts[currentFeature]);
		}
		
	} else {
		ofLogNotice() << "view not active - did not update rectangles!";
	}
}

void Mugshot::update(View::Features feature) {
	
	currentFeature = feature;
	
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("CrimsonText700Mugshot");
	View& view = user->getView(false);
	
    if (view.isActive()) {
		
		// get face
		face = view.getImage();
		calculateRectangles();
		
		// draw letter overlay
		faceoverlayFbo.begin();
		{
			ofClear(0, 0, 0);
			
			ofScale( partScale);
            ofTranslate(-faceBox.x, -faceBox.y);
			
			drawLettersFront(view.parts, view.getLandmarks(), ofxSmartFont::get("CrimsonText700Mugshot"));
		}
		faceoverlayFbo.end();
		
		drawFbo();
		
		// start fade-in
		featureImageAlpha = 0.0;
		auto alpha_tween = tweenManager.addTween(featureImageAlpha,
										featureImageAlpha,
										(float)1.0,
										1.0,
										0.0,
										TWEEN::Ease::Quadratic::Out);
		alpha_tween->start();
	}
	
	if (currentFeature == View::Features::INVALID) {
		face = ofImage();
	}
}

void Mugshot::fadeOutDarkFrame() {
	
	auto alpha_tween = tweenManager.addTween(featureFrameAlpha,
											 featureFrameAlpha,
											 (float)0.0,
											 1.0,
											 0.0,
											 TWEEN::Ease::Quadratic::Out);
	alpha_tween->start();
}

void Mugshot::drawFbo() {
	
	//
	fbo.begin();
	{
		// draw background
		ofEnableAlphaBlending();
		bgFbo.draw(0, 0);
		
		//--------------------------------
		// draw face (right side) - without markers
		face.bind();
		shader->begin();
		shader->setUniform1f("factor", 0.9); // SET A UNIFORM
		shader->setUniform1f("alpha", 1.0); // SET A UNIFORM
#ifdef MUGSHOT_IS_LEFT
		face.drawSubsection(DROP_SHADOW_W + MG_CARD_INSET_X , 238+DROP_SHADOW_H,
							MG_WIDTH, MG_HEIGHT,
							faceBox.x , faceBox.y, faceBox.width, faceBox.height);
#else
        face.drawSubsection(DROP_SHADOW_W + MG_CARD_INSET_X + MG_WIDTH + MG_SPACE, 238+ DROP_SHADOW_H,
                            MG_WIDTH, MG_HEIGHT,
                            faceBox.x , faceBox.y, faceBox.width, faceBox.height);
#endif
        shader->end();
		face.unbind();
		
		//ofPushMatrix();
		//ofScale( (float)mugSize/box.width, (float)MG_HEIGHT/box.height);
		//ofTranslate(-box.x, -box.y);
		//drawLettersProfile(view.parts, view.getLandmarks(), font);
		//ofPopMatrix();
		
		//  }
		
		// profile = false;
		// view = user->getView(profile);
		// if (view.isActive()) {
		
		//  ofImage& face = view.getImage();
		//  ofRectangle& box = ImageGrid::adjustAspectRatio(view.parts[View::HEAD], aspectRatio);
		
		//--------------------------------
		// draw darker version of image
		ofSetColor(127);
		
		drawFacecutFbo();
	}
	fbo.end();
}

void Mugshot::drawFacecutFbo() {
	
	//--------------------------------
	// draw darker version of image
	facecutFbo.begin();
	{
		ofClear(0, 0, 0);
		
		ofSetColor(255);
		
		// draw face
		face.bind();
		shader->begin();
		shader->setUniform1f("factor", 0.9); // SET A UNIFORM
		shader->setUniform1f("alpha", 0.65); // SET A UNIFORM
		face.drawSubsection(0, 0,
							MG_WIDTH, MG_HEIGHT,
							faceBox.x, faceBox.y, faceBox.width, faceBox.height);
		shader->end();
		face.unbind();
	}
	facecutFbo.end();
#ifdef MUGSHOT_IS_LEFT
    facecutFbo.draw(DROP_SHADOW_W + MG_CARD_INSET_X + MG_WIDTH + MG_SPACE , 298);
#else
	facecutFbo.draw(DROP_SHADOW_W + MG_CARD_INSET_X, 298);
#endif
}

void fadeOutDarkFrame() {
	
}

bool Mugshot::draw() {
	
	if (y >= SCREEN_HEIGHT) {
		return false;
	}
	else {
		ofPushMatrix();
		{
			ofTranslate(x, y);
			ofScale(scale);
			ofRotateDeg(rotation);
			
			// draw card
			fbo.draw(0, 0);
			
			// -> need to draw facepart
			
			// draw featre overlay
			
			ofPushMatrix();
			{
				// move to facecutFbo
#ifdef MUGSHOT_IS_LEFT
				ofTranslate(DROP_SHADOW_W + MG_CARD_INSET_X + MG_WIDTH + MG_SPACE, 256);
#else
                ofTranslate(DROP_SHADOW_W + MG_CARD_INSET_X, 256);
#endif
				
				ofScale(partScale);
				ofTranslate(-faceBox.x, -faceBox.y);
				
				if ((currentFeature != View::Features::INVALID) && featureFrameAlpha > 0.0) {
					// draw black frame
					ofPath path;
					path.setFillColor(ofColor(0,0,0,120*featureFrameAlpha));
					path.setStrokeColor(ofColor(0,0,0,255*featureFrameAlpha));
					path.setStrokeWidth(20);
					path.rectangle(featureRect);
					path.draw();
				}
				
				
				// draw face overlay
				if (featureImageAlpha > 0.0 && face.isAllocated()) {
					face.bind();
					shader->begin();
					shader->setUniform1f("factor", 0.9); // SET A UNIFORM
					shader->setUniform1f("alpha", featureImageAlpha); // SET A UNIFORM
				
					face.drawSubsection(featureRect.x, featureRect.y,
										(int)featureRect.width, (int)featureRect.height,
										featureRect.x, featureRect.y,
										(int)featureRect.width, (int)featureRect.height);
				
					shader->end();
					face.unbind();
				}
				
			}
			ofPopMatrix();
			
		}
		ofPopMatrix();
		
		// draw overlay
		drawOverlay();
	}
	
	return true;
}

void Mugshot::drawOverlay() {
	ofPushMatrix();
	{
		ofTranslate(x, y);
		ofScale(scale);
		ofRotateDeg(rotation);
		
		// draw overlay
#ifdef MUGSHOT_IS_LEFT
        faceoverlayFbo.draw(DROP_SHADOW_W + MG_CARD_INSET_X + MG_WIDTH + MG_SPACE, 256);
#else
		faceoverlayFbo.draw(DROP_SHADOW_W + MG_CARD_INSET_X, 256);
#endif
	}
	ofPopMatrix();
}

#ifdef TARGET_OSX

void Mugshot::drawLettersFront(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font) {
	
#elif defined TARGET_WIN32
	
void Mugshot::drawLettersFront(ofRectangle* parts, vector<glm::vec2>& landmarks, shared_ptr<ofxSmartFont> font) {
	
#endif
    ofSetColor(ofColor::fromHex(0xE9DDC4));
    FontUtil::draw("D", font, FontUtil::CENTER, ofVec2f(landmarks[0].x, landmarks[0].y - (landmarks[3].y - landmarks[0].y)));
    FontUtil::draw("D'", font, FontUtil::CENTER, ofVec2f(landmarks[16].x, landmarks[16].y - (landmarks[13].y - landmarks[16].y)));
    FontUtil::draw("I", font, FontUtil::CENTER, landmarks[1]);
    FontUtil::draw("I'", font, FontUtil::CENTER, landmarks[15]);
    FontUtil::draw("K", font, FontUtil::LEFT, landmarks[3]);
    FontUtil::draw("K'", font, FontUtil::RIGHT, landmarks[13]);
    FontUtil::draw("L", font, FontUtil::LEFT, landmarks[48]);
    FontUtil::draw("L'", font, FontUtil::RIGHT, landmarks[54]);
    FontUtil::draw("G", font, FontUtil::LEFT, landmarks[27]);
    FontUtil::draw("G'", font, FontUtil::RIGHT, landmarks[27]);
    FontUtil::draw("H", font, FontUtil::LEFT, landmarks[36]);
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[37].x + landmarks[38].x) /2, MAX(landmarks[37].y, landmarks[38].y)));
    FontUtil::draw("O", font, FontUtil::ABOVE, ofVec2f((landmarks[43].x + landmarks[44].x) /2, MAX(landmarks[43].y,landmarks[44].y)));
    
    ofSetColor(ofColor::fromHex(0x4F4733));
    ofVec2f a(landmarks[27].x, landmarks[27].y - (landmarks[51].y - landmarks[27].y));
    FontUtil::draw("A", font, FontUtil::ABOVE, a);
    ofVec2f b(landmarks[27].x, landmarks[8].y );
    FontUtil::draw("B", font, FontUtil::BELOW, b);
    FontUtil::draw("C", font, FontUtil::BELOW, ofVec2f(landmarks[17].x, landmarks[17].y - (landmarks[2].y - landmarks[0].y)));
    FontUtil::draw("C'", font, FontUtil::BELOW, ofVec2f(landmarks[26].x, landmarks[26].y -(landmarks[14].y - landmarks[16].y)));
    FontUtil::draw("E", font, FontUtil::RIGHT, landmarks[21]);
    FontUtil::draw("E'", font, FontUtil::LEFT, landmarks[22]);
    FontUtil::draw("F", font, FontUtil::LEFT, landmarks[17]);
    FontUtil::draw("F'", font, FontUtil::RIGHT, landmarks[26]);
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[40].x + landmarks[41].x) /2, MIN(landmarks[40].y, landmarks[41].y)));
    FontUtil::draw("O", font, FontUtil::BELOW, ofVec2f((landmarks[46].x + landmarks[47].x) /2, MIN(landmarks[46].y, landmarks[47].y)));
    FontUtil::draw("J", font, FontUtil::LEFT, landmarks[31]);
    FontUtil::draw("J'", font, FontUtil::RIGHT, landmarks[35]);
    FontUtil::draw("H'", font, FontUtil::RIGHT, landmarks[45]);
    ofDrawLine( a, b);
}

#ifdef TARGET_OSX
	
    void Mugshot::drawLettersProfile(ofRectangle* parts, vector<ofVec2f>& landmarks, shared_ptr<ofxSmartFont> font) {
		
#elif defined TARGET_WIN32
		
        void Mugshot::drawLettersProfile(ofRectangle* parts, vector<glm::vec2>& landmarks, shared_ptr<ofxSmartFont> font) {
			
#endif
    ofSetColor(ofColor::fromHex(0xE9DDC4));
    FontUtil::draw("A", font, FontUtil::LEFT, ofVec2f(landmarks[27].x, landmarks[27].y - (landmarks[51].y - landmarks[27].y)));
    FontUtil::draw("D", font, FontUtil::LEFT, landmarks[31]);
    FontUtil::draw("G'", font, FontUtil::LEFT, landmarks[42]);
    FontUtil::draw("B", font, FontUtil::ABOVE, landmarks[22]);
    FontUtil::draw("H", font, FontUtil::RIGHT, landmarks[45]);
    
    ofSetColor(ofColor::fromHex(0x4F4733));
    FontUtil::draw("F", font, FontUtil::ABOVE, landmarks[8]);
    FontUtil::draw("E", font, FontUtil::RIGHT, landmarks[35]);
    FontUtil::draw("L", font, FontUtil::ABOVE, ofVec2f((landmarks[43].x + landmarks[44].x) /2, MAX(landmarks[43].y, landmarks[44].y)));
    FontUtil::draw("I'", font, FontUtil::ABOVE, ofVec2f(landmarks[45].x, landmarks[29].y));
    
   // ofSetLineWidth(1);
    ofDrawLine(landmarks[15], landmarks[45]);
    ofDrawLine(landmarks[15], ofVec2f(landmarks[45].x, landmarks[29].y));
    ofSetColor(ofColor::fromHex(0xE9DDC4));
}

void Mugshot::drawDottedLine(ofVec2f start, ofVec2f end) {
    /* select black for all lines  */
    ofSetLineWidth(4);
    glColor3f (0.0, 0.0, 0.0);
    
    glEnable (GL_LINE_STIPPLE);
    
    glLineStipple (4, 0xAAAA);  /*  first parameter set the dash length in px  */
    glBegin(GL_LINES);
    glVertex2f (start.x, start.y); /* starting points */
    glVertex2f (end.x, end.y); /* end points */
    glEnd();
    glDisable (GL_LINE_STIPPLE);
    glFlush ();
    
    //ofPopMatrix();
}
		
void Mugshot::introAnimationDone() {
	appcontroller->mugshotIntroAnimationDone();
	m_introAnimationDone = true;
}

void Mugshot::animate(float delay) {
	animateCounter++;
	
    if (firstTime) {
		
        firstTime = false;
        int yTo = START_Y;
		auto t0 = tweenManager.addTween(y, y, yTo, ANIMATION_TIME, 0 , TWEEN::Ease::Quadratic::Out);
		
		t0->onComplete(myCompleteFunc, this);
		
        t0->start();
    }
	
    else {
		int y_dist = MG_Y_MOVE / animateCounter;
		int xTo = x - ofRandom( -80., 80);//+ ofRandom(y_dist - 10, y_dist + 10);
		
        int yTo = y  + ofRandom(y_dist - 0.3*y_dist, y_dist);//- ofRandom( -10., 10.);
        float rotTo = ofRandom(-4., 4.);
		float scaleTo = scale - (0.01 * animateCounter);
        auto t0 = tweenManager.addTween(x, x, xTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);
        auto t1 = tweenManager.addTween(y, y, yTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);
        auto t2 = tweenManager.addTween(rotation, rotation, rotTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);
		auto t3 = tweenManager.addTween(scale, scale, scaleTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);
        
        t0->start();
        t1->start();
        t2->start();
		t3->start();
    }
}
