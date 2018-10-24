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
    void setImageGrid(ImageGrid* imageGrid) {
        grid = imageGrid;
    };
    
    void update();
    void draw(int x, int y);
    static const int LEFT_PANEL_WIDTH = 70;
    static const int LINE_WIDTH = 6;
    static const int FBO_W = 1920;
    static const int FBO_H = 1080;
    
    ofPoint getSize();
    void setup();
    ~LeftPanelImageGrid() {
        fbo.clear();
    };
private:
    ofFbo fbo;
    ImageGrid* grid;
    void drawPanel(int y, string text);
};
#endif /* LeftPanelImageGrid_hpp */
