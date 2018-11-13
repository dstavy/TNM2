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
    
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); //we define the OpenGL version we want to use
	
    settings.setSize(3840, 1080);
    
#ifdef DEBUG
    // settings.multiMonitorFullScreen = false;
#else
    // settings.multiMonitorFullScreen = true;
    //  setMultiDisplayFullscreen (true);
#endif
    
    ofCreateWindow(settings);
#ifdef DEBUG
    //ofSetFullscreen(false);
#else
    //ofSetFullscreen(true);
#endif
    // ofxWatchdog::trap(void)
    // ofxWatchdog::trap();
    
    // this kicks off the running of my app
    ofRunApp(new ofApp());
    
}
