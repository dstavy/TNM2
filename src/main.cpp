#include "ofMain.h"
#include "ofApp.h"
<<<<<<< HEAD
//#include "ofxWatchdog.h"

//========================================================================
int main( ){
    // ofxWatchdog::boot(msec, reboot, override, verbose)
    //
    //     msec : how long does the watchdog wait, when the process hangs-up in milli seconds
    //   reboot : automatically restart the process
    // override : use internal signal handler (optional)
    //  verbose : print more log information (optional)
//    ofxWatchdog::boot(10000, true, true, true);
    
=======

//========================================================================
int main( ){
>>>>>>> 1cd2ba3b742e41f6f46d4409526405b24f2a1084
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); //we define the OpenGL version we want to use
    settings.setSize(1920, 1080);
    ofCreateWindow(settings);
<<<<<<< HEAD
#ifdef DEBUG
    ofSetFullscreen(false);
#else
    ofSetFullscreen(true);
#endif
    // ofxWatchdog::trap(void)
//    ofxWatchdog::trap();
    
=======
>>>>>>> 1cd2ba3b742e41f6f46d4409526405b24f2a1084
    // this kicks off the running of my app
    ofRunApp(new ofApp());
    
}
