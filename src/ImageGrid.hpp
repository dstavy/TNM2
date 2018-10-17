//
//  ImageGrid.hpp
//  TNM2
//
//  Created by Dan Stavy on 08/10/2018.
//

#ifndef ImageGrid_hpp
#define ImageGrid_hpp

#include "ofMain.h"

class ImageGrid  {
public:
void setup();
void update();
void draw();
private:
int aize = 600;
ofFbo fbo;
};
#endif /* ImageGrid_hpp */


