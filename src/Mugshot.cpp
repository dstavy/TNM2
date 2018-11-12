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



Mugshot::Mugshot(ofShader* shader, User* user) {
	
	// load background image (card)
	bgImage.load("Assets/sample_sheet.png");
	bgFbo.allocate(bgImage.getWidth(), bgImage.getHeight(), GL_RGBA);
	drawBackground(user);
	
	facecutFbo.allocate(MG_WIDTH, MG_HEIGHT, GL_RGB);
	facecutFbo.getTextureReference().getTextureData().bFlipTexture = true;
	
	fbo.allocate(bgImage.getWidth(), bgImage.getHeight(), GL_RGBA);
	
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    this->shader = shader;
    this->user = user;
    firstTime = true;
    x = START_X;
    y = ofGetHeight();
   // pos = glm::vec3(x, y, rotation);
}

void Mugshot::drawBackground(User* user) {
	
	bgFbo.begin();
	{
		ofClear(0,0,0,255);
		ofClearAlpha();
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
		
		ofLogNotice() << ofGetTimestampString("%H:%M:%S");
		font->draw(ofGetTimestampString("%H:%M:%S"), examined_1_pos.x, examined_1_pos.y);
		font->draw(ofGetTimestampString("%F"), examined_2_pos.x, examined_2_pos.y);
	}
	bgFbo.end();
	
}

void Mugshot::update(View::Features feature) {
	
    float aspectRatio = (float)MG_WIDTH/(float)MG_HEIGHT;
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("CrimsonText700Mugshot");
    bool profile = false;
	
	View& view = user->getView(profile);
    if (view.isActive()) {
		
		// get face
		ofImage& face = view.getImage();
		
		ofRectangle faceRec(view.parts[View::HEAD]);
		
        //faceRec.x += faceRec.x/4;
        //faceRec.width -= faceRec.x/4;
      //   faceRec.y -= faceRec.x/8;
       // faceRec.width += faceRec.x/2;
		ofRectangle box(ImageGrid::adjustAspectRatio(faceRec, aspectRatio));
		
		//
		fbo.begin();
		
		// draw background
		ofEnableAlphaBlending();
		bgFbo.draw(0, 0);
		
		// draw face
        face.bind();
        shader->begin();
        shader->setUniform1f("factor", 0.9); // SET A UNIFORM
        face.drawSubsection(dropshadow_w + MG_CARD_INSET_X, 238+dropshadow_h,
							MG_WIDTH, MG_HEIGHT,
							box.x , box.y, box.width, box.height);
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
		
		// draw darker version of image
		facecutFbo.begin();
		ofClear(0, 0, 0);
		
		ofSetColor(127);
		face.bind();
		shader->begin();
		shader->setUniform1f("factor", 0.9); // SET A UNIFORM
		face.drawSubsection(0, 0,
							MG_WIDTH, MG_HEIGHT,
							box.x, box.y, box.width, box.height);
		shader->end();
		face.unbind();
		
		ofSetColor(255);
		
        ofPushMatrix();
		ofRectangle& part = view.parts[feature];
		ofColor dark(0,0,0,125);
		ofPath path;
        //path.rectangle(box);
        //path.rectangle(part);
        ofTranslate(0, 0);
        ofScale( (float)MG_WIDTH/box.width, (float)MG_HEIGHT/box.height);
        ofTranslate(-box.x, -box.y);
        path.setFillColor(dark);
        path.setStrokeColor(ofColor::black);
        path.setStrokeWidth(10);
        path.rectangle(box);
        path.rectangle(part);
        ofEnableAlphaBlending();
        path.draw();
        ofDisableAlphaBlending();
        //ofTranslate(mugSize + MG_SPACE, 0);
        drawLettersFront(view.parts, view.getLandmarks(), font);
        ofPopMatrix();
		
		facecutFbo.end();
		
		facecutFbo.draw(dropshadow_w + MG_CARD_INSET_X + MG_WIDTH + MG_SPACE, 298);
		
        fbo.end();
     }
// ofPopMatrix();
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

bool Mugshot::draw() {
    if (x >= ofGetWindowWidth()) {
        return false;
    }
    else {
        ofPushMatrix();
        ofRotate(rotation);
        fbo.draw(x, y);
        ofPopMatrix();
    }
    return true;
}
        
void Mugshot::animate(float delay) {
	animateCounter++;
	
    if (firstTime) {
		
        firstTime = false;
        int yTo = START_Y;
		auto t0 = tweenManager.addTween(y, y, yTo, ANIMATION_TIME, 0 , TWEEN::Ease::Quadratic::Out);
        t0->start();
    }
	
    else {
		int x_dist = MG_X_MOVE - (50 * animateCounter);
		int xTo = x + ofRandom(x_dist -10, x_dist + 10);
        int yTo = y - ofRandom( -10,  + 10);
        float rotTo = ofRandom(-4., 4.);
        auto t0 = tweenManager.addTween(x, x, xTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);
        auto t1 = tweenManager.addTween(y, y, yTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);
        auto t2 = tweenManager.addTween(rotation, rotation, rotTo, ANIMATION_TIME, delay , TWEEN::Ease::Quadratic::Out);

        t0->start();
        t1->start();
        t2->start();
    }
}
