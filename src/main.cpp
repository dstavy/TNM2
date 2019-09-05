#include "ofMain.h"
#include "ofApp.h"
//#include "ofxWatchdog.h"

//========================================================================
int main( ){
    // ofxWatchdog::boot(msec, reboot, override, verbose)
    //
    //     msec : how long does the watchdog wait, when the process hangs-up in milli seconds
    //   reboot : automatically restart the process
    // override : use internal signal handler (optional)
    //  verbose : print more log information (optional)
    //ofxWatchdog::boot(10000, true, true, true);
    /**/
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); //we define the OpenGL version we want to use
	
	settings.setSize(2160, 1920);
	//settings.setSize(1920, 2160);
	//settings.setSize(1080, 1920);
	//settings.setSize(1920, 1080 * 2);
	
#//ifdef TARGET_WIN32
	settings.decorated = false;
	// settings.windowMode = OF_FULLSCREEN;
	//settings.multiMonitorFullScreen = true;
	//settings.windowMode = OF_FULLSCREEN;
	//settings.setPosition(glm::vec2(1280.0f, 0.0f));
	// settings.setPosition(glm::vec2(0, 0));
	//window.setMultiDisplayFullscreen(true);
	settings.setSize(2160, 1920);
	settings.setPosition(ofVec2f(1280, 0));

	//settings.monitor = 1;
	//settings.resizable = false;
	//settings.resizable = true;
//#endif
    
#ifdef DEBUG
     settings.multiMonitorFullScreen = false;
#else
   // settings.multiMonitorFullScreen = true;
	//settings.windowMode = OF_WINDOW;
   //setMultiDisplayFullscreen (true);
#endif
    
    ofCreateWindow(settings);
  //  ofSetFullscreen(true);
#ifdef DEBUG
    //ofSetFullscreen(false);
#else
#endif
    // ofxWatchdog::trap(void)
    // ofxWatchdog::trap();
    
    // this kicks off the running of my app
    ofRunApp(new ofApp());
    
}
