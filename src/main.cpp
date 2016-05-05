#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

//    ofAppGLFWWindow window;

    ofSetupOpenGL(1320,660,OF_WINDOW);			// <-------- setup the GL context

//    window.setWindowTitle("MoviePrint");

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());

}
