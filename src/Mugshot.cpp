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

const static int dropshadow_w = 20;
const static int dropshadow_h = 18;

const static ofPoint height_pos = {140, 85, 0};
const static ofPoint eng_height_pos = {124, 115, 0};
const static ofPoint out_a_pos = {140, 145, 0};
const static ofPoint trunk_pos = {124, 175, 0};

const static ofPoint head_length_pos = {280, 85, 0};
const static ofPoint head_width_pos = {280, 115, 0};
const static ofPoint cheek_width_pos = {280, 145, 0};
const static ofPoint r_ear_length_pos = {280, 175, 0};

const static ofPoint age_pos = {666, 85, 0};
const static ofPoint apparent_age_pos = {720, 115, 0};


const static ofPoint examined_1_pos = {120, 796, 0};
const static ofPoint examined_2_pos = {236, 792, 0};

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
	y = ofGetHeight();

	
	bgFbo.allocate(bgImage.getWidth(), bgImage.getHeight(), GL_RGBA);
	drawBackground(user);
	
	// darkened face
	facecutFbo.allocate(MG_WIDTH, MG_HEIGHT, GL_RGB);
	facecutFbo.getTexture().getTextureData().bFlipTexture = true;
	
	// letter overlay
	faceoverlayFbo.allocate(MG_WIDTH, MG_HEIGHT, GL_RGBA);
	
	// render overlay fbo
	View& view = this->user->getView(false);
	// get face
	ofImage& face = view.getImage();
	ofRectangle faceRec(view.parts[View::HEAD]);
	ofRectangle box(ImageGrid::adjustAspectRatio(faceRec, MG_ASPECT_RATIO));
	
	faceoverlayFbo.begin();
	{
		ofClear(0, 0, 0);
		
		ofScale( (float)MG_WIDTH/box.width, (float)MG_HEIGHT/box.height);
		ofTranslate(-box.x, -box.y);
		drawLettersFront(view.parts, view.getLandmarks(), ofxSmartFont::get("CrimsonText700Mugshot"));
	}
	faceoverlayFbo.end();
	
	
	fbo.allocate(bgImage.getWidth(), bgImage.getHeight(), GL_RGBA);
	
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
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
		return View::Features::INVALID;
	}
	View::Features new_feature = (View::Features)partsToVisit[0];
	partsToVisit.erase (partsToVisit.begin());
	return new_feature;
	
//	int id = (int)(floor(ofRandom(partsToVisit.size())));
//	int f = partsToVisit[id];
//	partsToVisit.erase (partsToVisit.begin()+id);
//
//	return (View::Features)f;
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
		
		font->draw(meterToCMDashMM(1.234567), height_pos.x, height_pos.y);
		font->draw(meterToCMDashMM(1.234567), eng_height_pos.x, eng_height_pos.y);
		font->draw(meterToCMDashMM(1.234567), out_a_pos.x, out_a_pos.y);
		font->draw(meterToCMDashMM(1.234567), trunk_pos.x, trunk_pos.y);
		
		font->draw(meterToCMDashMM(1.234567), head_length_pos.x, head_length_pos.y);
		font->draw(meterToCMDashMM(1.234567), head_width_pos.x, head_width_pos.y);
		font->draw(meterToCMDashMM(1.234567), cheek_width_pos.x, cheek_width_pos.y);
		font->draw(meterToCMDashMM(1.234567), r_ear_length_pos.x, r_ear_length_pos.y);
		
		font->draw(meterToCMDashMM(1.234567), age_pos.x, age_pos.y);
		font->draw(meterToCMDashMM(1.234567), apparent_age_pos.x, apparent_age_pos.y);
		
		font->draw(ofGetTimestampString("%H:%M:%S"), examined_1_pos.x, examined_1_pos.y);
		font->draw(ofGetTimestampString("%F"), examined_2_pos.x, examined_2_pos.y);
	}
	bgFbo.end();
	
}

void Mugshot::update(View::Features feature) {
	
	currentFeature = feature;
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("CrimsonText700Mugshot");
	View& view = user->getView(false);
	
    if (view.isActive()) {
		
		// get face
		face = view.getImage();
		ofRectangle faceRec(view.parts[View::HEAD]);
		faceBox = ofRectangle(ImageGrid::adjustAspectRatio(faceRec, MG_ASPECT_RATIO));
		partScale = ofPoint((float)MG_WIDTH/faceBox.width, (float)MG_HEIGHT/faceBox.height);
		
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
			face.drawSubsection(dropshadow_w + MG_CARD_INSET_X + MG_WIDTH + MG_SPACE, 238+dropshadow_h,
								MG_WIDTH, MG_HEIGHT,
								faceBox.x , faceBox.y, faceBox.width, faceBox.height);
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
			facecutFbo.begin();
			{
				ofClear(0, 0, 0);
				ofSetColor(127);
				
				// draw face
				face.bind();
				shader->begin();
				shader->setUniform1f("factor", 0.9); // SET A UNIFORM
				shader->setUniform1f("alpha", 0.5); // SET A UNIFORM
				face.drawSubsection(0, 0,
									MG_WIDTH, MG_HEIGHT,
									faceBox.x, faceBox.y, faceBox.width, faceBox.height);
				shader->end();
				face.unbind();
				
				ofSetColor(255);
				
				if (currentFeature != View::Features::INVALID) {
					
					featureRect = ofRectangle(view.parts[currentFeature]);
					
					ofLogNotice() << "mugshot featureRect: " << ofToString(featureRect);
					
					// overlay selected feature with dark frame
					// undarkened part has to be drawn from outside this!
					ofPushMatrix();
					{
						ofEnableAlphaBlending();
						
						ofScale(partScale);
						ofTranslate(-faceBox.x, -faceBox.y);
						
						ofPath path;
						path.setFillColor(ofColor(0,0,0,90));
						path.setStrokeColor(ofColor::black);
						path.setStrokeWidth(20);
						path.rectangle(featureRect);
						path.draw();
					}
					ofPopMatrix();
				}
			}
			facecutFbo.end();
			
			facecutFbo.draw(dropshadow_w + MG_CARD_INSET_X, 298);
		}
        fbo.end();
     }
// ofPopMatrix();
}

bool Mugshot::draw() {
	
	if (x >= SCREEN_WIDTH) {
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
			
			ofPushMatrix();
			{
				// move to facecutFbo
				ofTranslate(dropshadow_w + MG_CARD_INSET_X, 256);
				
				ofScale(partScale);
				ofTranslate(-faceBox.x, -faceBox.y);
				
				// draw face overlay
				if (face.isAllocated()) {
					
					face.bind();
					shader->begin();
					shader->setUniform1f("factor", 0.9); // SET A UNIFORM
					shader->setUniform1f("alpha", 1.0); // SET A UNIFORM
					
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
		faceoverlayFbo.draw(dropshadow_w + MG_CARD_INSET_X, 298);
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
		int x_dist = 1.7 * MG_X_MOVE / animateCounter;
		int xTo = x + ofRandom(x_dist - 10, x_dist + 10);
		
        int yTo = y - ofRandom( -10., 10.);
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
