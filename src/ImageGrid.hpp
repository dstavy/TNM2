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
    void setup(ofShader& shader, Group* group, bool profile, int x, int y, int w, int h, int xSpacing, int ySpacing, int userPerLevel) {
        this->shader = shader;
        this->group = group;
        this->profile = profile;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->xSpacing = xSpacing;
        this->ySpacing = ySpacing;
        
        // Clear the FBO's
        // otherwise it will bring some junk with it from the memory
        fbo.begin();
        ofClear(0,0,0,255);
        fbo.end();
    };
    
void update();
void draw();
private:
    ofFbo fbo;
    Group *group;
    bool profile;
    int x;
    int y;
    int w;
    int h;
    int xSpacing;
    int ySpacing;
    int userPerLevel;
    ofShader& shader;
};
#endif /* ImageGrid_hpp */


