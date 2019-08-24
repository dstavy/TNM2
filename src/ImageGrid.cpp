//
//  ImageGrid.cpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#include "ImageGrid.hpp"
#include "ofxSmartFont.h"
#include "Mugshot.hpp"
#include "Globals.h"
#include "ofApp.h"
#include "Mugshot.hpp"

#define FIXED_FLOAT(x) std::fixed <<std::setprecision(2)<<(x)

static void myCB_FadeoutDone(void* ptr) {
    
    if (ptr) {
        ImageGrid* grid = (ImageGrid*)ptr;
        grid->update();
    }
}

static void myCB_DummyDone(void* ptr) {
    
    if (ptr) {
        ImageGrid* grid = (ImageGrid*)ptr;
        grid->startScaleUpFlyingImage();
    }
}

static void myCB_ScalingDone(void* ptr) {
    
    if (ptr) {
        ImageGrid* grid = (ImageGrid*)ptr;
        grid->startFlyingAnimation();
    }
}

static void myCB_FlyDone(void* ptr) {
    
    if (ptr) {
        ImageGrid* grid = (ImageGrid*)ptr;
        grid->resetLoading();
    }
}

void ImageGrid::setup(ofApp* app,
                      ofShader* shader,
                      Group* group,
                      int wElement,
                      int hElement,
                      int userPerLevel,
                      ofPoint flyInStartPos,
                      float gridX,
                      float gridY,
                      float scale,
                      int delayLoading,
                      string title,
                      ofColor bg)
{
    this->appcontroller = app;
    this->shader = shader;
    this->group = group;
    this->scale = scale;
    this->w = wElement*scale;
    this->h = hElement*scale;
    this->userPerLevel = userPerLevel;
    this->title = title;
    this->bg = bg;
    this->delayLoading = delayLoading;
    this->flyInStartPosition = flyInStartPos;
    this->gridLocation.x = gridX*scale;
    this->gridLocation.y = gridY*scale;
    
    //this->leftPanel = leftPanel;
    aspectRatio = (float)w/h;
    
    calculateSizes();
    int fboW = wholeSize.x;
    int fboH = wholeSize.y;
    
    fbo.allocate(fboW, fboH, GL_RGBA);
    //maskFbo.allocate(fboW, fboH, GL_RGB);
    outputFbo.allocate(fboW, fboH, GL_RGBA);
    //	outputFbo.getTexture().getTextureData().bFlipTexture = true;
    
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
    /*
     maskFbo.begin();
     ofClear(0,0,0,0);
     ofSetColor(255);
     ofDrawCircle(fboW/2, fboH/2, fboW/2);
     maskFbo.end();
     */
    outputFbo.begin();
    ofClear(0,0,0,0);
    outputFbo.end();
    
    // log and get size
    getSize();
    
    
    //	if (!maskShader.load("shaders/mask")) {
    //		ofLogNotice("cant load shaders/mask");
    //	}
}

// call this to calculate where to draw next
ofPoint ImageGrid::getSize() {
    ofLogNotice("Grid:" + getTitle() + " size: " + ofToString(wholeSize.x/scale) + " " + ofToString(wholeSize.y/scale));
    
    return ofPoint(wholeSize.x/scale, wholeSize.y/scale);
}

void ImageGrid::reset() {
    
    //	float toA = 0.0;
    //	auto tween = tweenManager.addTween(fboAlpha, fboAlpha, toA, 1.0, 0.0 , TWEEN::Ease::Quadratic::Out);
    //	tween->onComplete(myCB_FadeoutDone, this);
    //	tween->start();
    
    loadingTime = ofGetElapsedTimeMillis();
    animStage = FADE_OUT;
}

void ImageGrid::resetLoading() {
    if (animStage == FLY_IN) {
        loading = true;
        loadingTime = ofGetElapsedTimeMillis();
        fboAlpha = 1.0;
        animStage = FADE_IN;
    }
}

void ImageGrid::update() {
    if (doUpdateNextUpdate) {
        doUpdateNextUpdate = false;
        doUpdate();
    }
}

void ImageGrid::doUpdate() {
    
    vector<User*> users(0);
    int currentUserIndex;
    currentUser = group->getGridUsers(userPerLevel, users, currentUserIndex);
    
    if (users.size() > 0) {
        currentUserPosition.x = -1;
        currentUserPosition.y = -1;
        
        ofPushStyle();
        {
            fbo.begin();
            {
                ofColor bgcolor;
                bgcolor.fromHex(0x544e41);
                ofClear(60, 50, 41, 255);
                ofClearAlpha();
                ofSetColor(bg);
                ofFill();
                
                //    drawHeader();
                //    y += HEADER_HEIGHT;
                int y = (group->numLevels - 1) * (rawSize.y + Y_SPACING);
                for(int i = 0; i < group->numLevels; i++) {
                    drawRow(y, users.begin() + (i * userPerLevel), userPerLevel);
                    y -= rawSize.y + Y_SPACING;
                }
                
                //			int rawMax = (group->numLevels * userPerLevel) % currElement;
                //			int remain = currElement - (rawMax * userPerLevel);
                //			if (remain > 0) {
                //				drawRow(y, users.begin() + (remain * userPerLevel), remain);
                //			}
            }
            fbo.end();
        }
        ofPopStyle();
        
        
        // set alpha, but also FLY_IN so we dont see the fbo yet
        fboAlpha = 1.0;
        imageAlpha = 0.0;
        flyInImage = ofImage();
        
        
        
        // add a delay before starting fly-ing animation
        auto tween_dummy = tweenManager.addTween(dummy,
                                                 (float)1.0,
                                                 (float)0.0,
                                                 TIME_WAIT_BEFORE_FLY,
                                                 0.0,
                                                 TWEEN::Ease::Linear);
        tween_dummy->onComplete(myCB_DummyDone, this);
        tween_dummy->start();
        
        animStage = DELAY;
    }
}

void ImageGrid::setupAnimation() {
    
    // get data from mugshot
    Mugshot* mugshot = appcontroller->getCurrentMugshot();
    flyInImage = mugshot->face;
    featureRect = mugshot->featureRect;
    faceBox = mugshot->faceBox;
    partScale = mugshot->partScale;
    currentMugshot = mugshot;
    
    // setup inital values
    flyInImagePosition = ofPoint(flyInStartPosition.x, flyInStartPosition.y);
    flyingImageImageOffset = ofPoint(featureRect.x, featureRect.y);
    flyingImageSize = ofPoint(featureRect.width , featureRect.height);
    flyingImageRectSize = ofPoint(featureRect.width , featureRect.height );
    flyingImageRectPos = ofPoint(featureRect.x, featureRect.y);
}

void ImageGrid::startScaleUpFlyingImage() {
    
    setupAnimation();
    
    float anim_time = TIME_FEATURE_SCALE;
    
    //----------------------------------------
    //	ofPoint target_scale(flyingImageSize.x * FEATURE_SCALE, flyingImageSize.y * FEATURE_SCALE);
    //	float x_d = (target_scale.x - flyingImageSize.x) / 2.0;
    //	float y_d = (target_scale.y - flyingImageSize.y) / 2.0;
    //	ofPoint target_position(flyInImagePosition.x - x_d, flyInImagePosition.y - y_d);
    
    float diff = 10 / partScale.x;
    
    ofPoint target_scale(flyingImageSize.x + diff, flyingImageSize.y + diff);
    ofPoint target_position(flyInImagePosition.x - diff/2.0, flyInImagePosition.y - diff/2.0);
    
    auto pos_scale = tweenManager.addTween(flyInImagePosition,
                                           flyInImagePosition,
                                           target_position,
                                           anim_time,
                                           0.0,
                                           TWEEN::Ease::Quadratic::InOut);
    
    
    auto tween_scale = tweenManager.addTween(flyingImageSize,
                                             flyingImageSize,
                                             target_scale,
                                             anim_time,
                                             0.0,
                                             TWEEN::Ease::Quadratic::InOut);
    
    
    auto alpha_scale = tweenManager.addTween(imageAlpha,
                                             imageAlpha,
                                             (float)1.0,
                                             0.2,
                                             0.0,
                                             TWEEN::Ease::Quadratic::InOut);
    
    tween_scale->onComplete(myCB_ScalingDone, this);
    tween_scale->start();
    pos_scale->start();
    alpha_scale->start();
    
    //----------------------------------------
    animStage = SCALE;
}

void ImageGrid::startFlyingAnimation() {
    
    currentMugshot->showDarkFrame();
    
    //----------------------------------------
    // tween position
    auto tween = tweenManager.addTween(flyInImagePosition,
                                       flyInImagePosition,
                                       currentUserPosition + gridLocation,
                                       TIME_FEATURE_FLY,
                                       0.0,
                                       TWEEN::Ease::Quadratic::Out);
    
    //----------------------------------------
    // image offset
    ofPoint target_offset(faceBox.x, faceBox.y);
    auto tween_image_offset = tweenManager.addTween(flyingImageImageOffset,
                                                    flyingImageImageOffset,
                                                    target_offset,
                                                    TIME_FEATURE_FLY,
                                                    0.0,
                                                    TWEEN::Ease::Quadratic::Out);
    
    //----------------------------------------
    // tween for scale
    ofPoint target_scale(w/partScale.x, h/partScale.y);
    auto tween_scale = tweenManager.addTween(flyingImageSize,
                                             flyingImageSize,
                                             target_scale,
                                             TIME_FEATURE_FLY,
                                             0.0,
                                             TWEEN::Ease::Quadratic::Out);
    
    //----------------------------------------
    // tween for featureRect
    ofRectangle fr_copy(featureRect);
    ofRectangle target_rect = ofRectangle(ImageGrid::adjustAspectRatio(fr_copy, (float)w/(float)h));
    
    float width_dist = (target_rect.width - featureRect.width);
    float height_dist = (target_rect.height - featureRect.height);
    
    ofPoint target_rect_size(featureRect.width+width_dist, featureRect.height+height_dist);
    
    auto tween_rect = tweenManager.addTween(flyingImageRectSize,
                                            flyingImageRectSize,
                                            target_rect_size,
                                            TIME_FEATURE_FLY,
                                            0.0,
                                            TWEEN::Ease::Quadratic::Out);
    
    ofPoint target_rect_pos(featureRect.x-(width_dist/2.0), featureRect.y-(height_dist/2.0));
    auto tween_rect_pos = tweenManager.addTween(flyingImageRectPos,
                                                flyingImageRectPos,
                                                target_rect_pos,
                                                TIME_FEATURE_FLY,
                                                0.0,
                                                TWEEN::Ease::Quadratic::Out);
    
    //	ofLogNotice() << "imagegrid featureRect: " << ofToString(featureRect);
    //	ofLogNotice() << "imagegrid target_Rect: " << ofToString(target_rect_pos) << " : " << ofToString(target_rect_size);
    
    
    // start all the tweens
    tween->onComplete(myCB_FlyDone, this);
    tween->start();
    tween_image_offset->start();
    tween_scale->start();
    tween_rect->start();
    tween_rect_pos->start();
    
    signalOnNextRender = 0;
    animStage = FLY_IN;
}


void ImageGrid::calculateSizes() {
    elementSize.x = w + ELEMENT_SIDE_PADDING * 2;
    elementSize.y = h + SCORE_AREA_HEIGHT;
    rawSize.x = PADDING_ROW * 2 + userPerLevel * elementSize.x;
    rawSize.y = PADDING_ROW * 2 + elementSize.y;
    wholeSize.x = rawSize.x;
    wholeSize.y = rawSize.y * group->numLevels + Y_SPACING * (group->numLevels- 1);
    //    wholeSize.y = HEADER_HEIGHT + lineSize.y * group->numLevels + Y_SPACING * (group->numLevels- 1);
}

void ImageGrid::draw() {
    
    //	outputFbo.begin();
    //	ofClear(0, 0, 0, 0);
    //	fbo.draw(0,0);
    //	outputFbo.end();
    
    
    ofPushMatrix();
    {
        ofTranslate(gridLocation);
#ifdef DRAW_DEBUG
        ofSetColor(200, 200, 0, 100);
        ofDrawRectangle(0, 0, 20, 20);
#endif
        
        //		outputFbo.begin();
        //		ofClear(0, 0, 0, 0);
        //		maskShader.begin();
        //		maskShader.setUniformTexture("maskTex", maskFbo.getTexture(), 1 );
        
        //ofScale(scale);
        //ofScale(GRID_SCALE);
        // draw grid
        if (animStage == FADE_IN) {
            
            int section = floor((float)(ofGetElapsedTimeMillis() - loadingTime) / delayLoading) * (rawSize.y+Y_SPACING);
            
            fbo.getTexture().drawSubsection(0, wholeSize.y - section,
                                            wholeSize.x, section,
                                            0, wholeSize.y - section,
                                            wholeSize.x, section);
            
            if ((wholeSize.y - section) < rawSize.y) {
                loading = false;
                animStage = DONE;
                currentMugshot->fadeOutDarkFrame();
            }
        }
        else if (animStage == FADE_OUT) {
            
            int section = floor((float)(ofGetElapsedTimeMillis() - loadingTime) / delayLoading) * (rawSize.y+Y_SPACING) ;
            
            if (section < currentUserPosition.y) {
                flyInImage = ofImage();
                imageAlpha = 0.0;
            }
            
            fbo.getTexture().drawSubsection(0, 0,
                                            wholeSize.x, wholeSize.y - section,
                                            0, 0,
                                            wholeSize.x, wholeSize.y - section);
            
            if ((wholeSize.y - section) <= rawSize.y) {
                // fadeout done
                loading = false;
                fboAlpha = 0.0;
                
                doUpdateNextUpdate = true;
                //				update();
            }
            
        }
        else if (animStage > FLY_IN) {
            
            ofSetColor(255, 255, 255, fboAlpha*255);
            fbo.draw(0,0);
        }
    }
    ofPopMatrix();
    //		maskShader.end();
    //		outputFbo.end();
    //		outputFbo.draw(0, 0);
    
    // draw flying image
    if (flyInImage.isAllocated()) {
        
        ofPushMatrix();
        ofPushStyle();
        {
            // scale "out" of gridscale
            //ofScale(1.0/GRID_SCALE);
            ofTranslate(flyInImagePosition.x, flyInImagePosition.y);
#ifdef DRAW_DEBUG
            ofSetColor(200, 0, 0, 100);
            ofDrawRectangle(0, 0, 20, 20);
#endif
            
            ofScale(partScale);
            ofTranslate(-faceBox.getX(), -faceBox.getY());
#ifdef DRAW_DEBUG
            ofSetColor(0, 200, 0, 100);
            ofDrawRectangle(0, 0, 20, 20);
#endif
            
            flyInImage.bind();
            shader->begin();
            shader->setUniform1f("factor", 0.9); // SET A UNIFORM
            shader->setUniform1f("alpha", fboAlpha * imageAlpha); // SET A UNIFORM
            
            flyInImage.drawSubsection(flyingImageImageOffset.x, flyingImageImageOffset.y,
                                      flyingImageSize.x, flyingImageSize.y,
                                      flyingImageRectPos.x, flyingImageRectPos.y,
                                      flyingImageRectSize.x, flyingImageRectSize.y);
            
            shader->end();
            flyInImage.unbind();
#ifdef DRAW_DEBUG
            ofSetColor(0, 0, 200, 100);
            ofDrawRectangle(featureRect);
#endif
        }
        ofPopStyle();
        ofPopMatrix();
        
        //---
        if (signalOnNextRender > -1 && ++signalOnNextRender == 1) {
            signalOnNextRender = -1;
            
            // signal mugshot to hide image!
            if (appcontroller) {
                appcontroller->signalCurrentMugshotImageOff();
            }
        }
    }
    
#ifdef DRAW_DEBUG
    ofSetColor(200, 0, 200, 100);
    ofDrawRectangle(0, -20, dummy*500.0, 20);
#endif
    //}
    //ofPopMatrix();
}

void ImageGrid::drawFlyingImage() {
}

string ImageGrid::getTitle() {
    return View::featureToString(group->feature) + " / " + View::proflieToString(group->profile);
}

void ImageGrid::drawHeader() {
    //ofSetColor(bg);
    ofDrawRectangle(0,0, wholeSize.x, HEADER_HEIGHT);
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("BodonPoster");
    ofSetColor(0,0,0); //(ofColor::black);
    string title = getTitle();
    int hs = font->height(title);
    int ws = font->width(title);
    font->draw(title, wholeSize.x / 2 - ws /2, HEADER_HEIGHT/2 + hs /2);
    ofSetColor(bg);
}

void ImageGrid::drawRow(int y, vector<User*>::iterator it, int num) {
    
    ofLogNotice() << "drawRow: " << y;
    
    int x = 0;
    ofDrawRectangle(x, y, rawSize.x, rawSize.y);
    y += PADDING_ROW;
    x += PADDING_ROW;
    for (int j = 0; j < num; j++) {
        drawElement(*it, x, y);
        it++;
        x += elementSize.x;
    }
}

void ImageGrid::drawElement(User* user, int x, int y) {
    if (user != NULL) {
        View& view = user->getView(group->profile);
        
        if (user->isCurrent && currentUserPosition.x < 0) {
            ofLogNotice() << "current user position: " << x << " " << y;
            currentUserPosition.set(x, y);
        }
        
        if (view.isActive()) {
            ofImage& face = view.getImage();
            ofRectangle box(view.getBounderyBox(group->feature));
            if (box.width > 0) {
                box = adjustAspectRatio(box, aspectRatio);
                
                face.bind();
                shader->begin();
                shader->setUniform1f("factor", 1.0);// color
                shader->setUniform1f("alpha", 1.0);// color
                face.drawSubsection(x + ELEMENT_SIDE_PADDING, y,
                                    w, h,
                                    box.x, box.y,
                                    box.width, box.height);
                shader->end();
                face.unbind();
                
                drawScoreArea(user->getFactrorScore(), user->isCurrent, x , y + h);
            }
        } else {
            ofLogNotice() << "view not active!";
        }
    }
}

void ImageGrid::drawScoreArea(float score, bool isCurrent, int x, int y) {
    
    shared_ptr<ofxSmartFont> font = ofxSmartFont::get("CrimsonRegular");
    //    ofSetColor(ofColor::lightGrey);
    //    ofDrawRectangle(x, y, w, SCORE_AREA_HEIGHT);
    
    ofSetColor(ofColor::fromHex(0x463f37));
    std::stringstream buffer;
    buffer << FIXED_FLOAT(score);
    string sScore = buffer.str();
    int hs = font->height(sScore);
    int ws = font->width(sScore);
    font->draw(sScore, x + (w - ws)/2, y + SCORE_AREA_HEIGHT/2 + hs/2);
    ofSetColor(bg);
}

ofRectangle& ImageGrid::adjustAspectRatio(ofRectangle& box, float aspectRatio) { // x/y
    
    float imageAspectRatio = (float)box.width/box.height;
    int w = box.width;
    int h = box.height;
    if (imageAspectRatio > aspectRatio) {
        h *=  imageAspectRatio / aspectRatio;
    } else if (imageAspectRatio < aspectRatio) {
        w *=  aspectRatio / imageAspectRatio;
    }
    
    box.y -= (h - box.height) /2;
    box.x -= (w - box.width) /2;
    box.height = h;
    box.width = w;
    
    return box;
}
