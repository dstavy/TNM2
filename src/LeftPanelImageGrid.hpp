//
//  LeftPanelImageGrid.hpp
//  TNM2
//
//  Created by Dan Stavy on 24/10/2018.
//

#ifndef LeftPanelImageGrid_hpp
#define LeftPanelImageGrid_hpp

#include "ofMain.h"
#include "ImageGrid.hpp"

class LeftPanelImageGrid  {
public:
    void setImageGrid(ImageGrid* imageGrid);
    void update();
    void draw(int x, int y);
    static const int LEFT_PANEL_WIDTH = 70;
    static const int FBO_W = 1920;
    static const int FBO_H = 1080;
    
    ofPoint getSize();
    LeftPanelImageGrid() {
        fbo.allocate(FBO_W, FBO_H);
        // Clear the FBO's
        // otherwise it will bring some junk with it from the memory
        fbo.begin();
        ofClear(0,0,0,255);
        fbo.end();
        //getSize();
    };
    
    ~LeftPanelImageGrid() {
        fbo.clear();
    };
private:
    ofFbo fbo;
};
#endif /* LeftPanelImageGrid_hpp */
