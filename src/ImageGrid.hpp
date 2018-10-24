//
//  ImageGrid.hpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#ifndef ImageGrid_hpp
#define ImageGrid_hpp

#include "ofMain.h"
#include "Group.hpp"

class ImageGrid  {
public:
    void setup(ofShader* shader, Group* group, int wElement, int hElement, int userPerLevel, string title, float scale, ofColor bg = ofColor::antiqueWhite);
    void update();
    void draw(int x, int y);
    static const int PADDING_ROW = 4;
    static const int SCORE_AREA_HEIGHT = 33;
    static const int ELEMENT_SIDE_PADDING = 1;
    static const int HEADER_HEIGHT = 60;
    static const int Y_SPACING = 4;
    ofPoint getSize();
    ofColor getBgColor() {
        return bg;
    };
    
    ~ImageGrid() {
        fbo.clear();
    };
    
    ofFbo fbo;
    Group *group;
    int w;
    int h;
    int userPerLevel;
    float aspectRatio;
    ofShader* shader;
    string title;
    float scale;
    ofColor bg;
    //bool leftPanel;
    void calculateSizes();
    string getTitle();
    void drawHeader();
    void drawRow(int y, vector<User*>::iterator it);
    void drawLeftPanel();
    void drawElement(User* user, int x, int y);
    void drawScoreArea(float score, bool isCurrent, int x, int y);
    ofPoint lineSize;
    ofPoint elementSize;
    ofPoint wholeSize;
    ofRectangle& adjustAspectRatio(ofRectangle& box, float aspectRatio);
};
#endif /* ImageGrid_hpp */


