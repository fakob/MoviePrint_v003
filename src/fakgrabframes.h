#ifndef FAKGRABFRAMES_H
#define FAKGRABFRAMES_H

#include <string>
#include <iostream>
#include "ofMain.h"
#include "fakgrabbedmoviestill.h"

#include "ofxCv.h"

#define TimeToWaitForMovie 500

#define FAK_ORANGECOLOR ofColor(238, 71, 0, 255)
#define FAK_DARKORANGECOLOR ofColor(99, 30, 0, 255)
#define FAK_DARKDARKORANGECOLOR ofColor(75, 21, 0, 255)
#define FAK_MIDDLEDARKORANGECOLOR ofColor(170, 50, 0, 255)
#define FAK_LIGHTERMIDDLEDARKORANGECOLOR ofColor(185, 55, 0, 255)

#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)
#define FAK_LIGHTGRAY ofColor(205, 205, 205, 255)
#define FAK_MIDDLEGRAY ofColor(195, 195, 195, 255)
#define FAK_TRANSPARENT ofColor(0,0,0,0)

#define FAK_ORANGE1 ofColor(255, 80, 6, 255)
#define FAK_ORANGE2 ofColor(255, 183, 153, 255)
#define FAK_ORANGE3 ofColor(255, 147, 101, 255)
#define FAK_ORANGE4 ofColor(255, 168, 131, 255)
#define FAK_ORANGE5 ofColor(255, 211, 193, 255)

#define FAK_WHITE ofColor(255, 255, 255, 255)
#define FAK_BLACK ofColor(0, 0, 0, 255)
#define FAK_SHADOW ofColor(0, 0, 0, 130)
#define FAK_GRAY ofColor(59, 59, 59, 255)

class fakGrabFrames : public ofThread
{
public:
    fakGrabFrames(){};

//    struct compareOnlyYValueStruct
//    {
//        bool operator()(const ofVec2f &a, const ofVec2f &b)
//        {
//            return a.y < b.y;
//        }
//    } compareYOperator;

    //--------------------------------------------------------------
//    struct grabbedFrameStruct {
//        ofImage gsImage; //image into which the videoframe is grabbed
//        ofTexture gsTexture; //texture into which the grabbedFrame is copied - this is drawn to screen - this is also resized if necessary
//        int gsID; // ID in Array
//        int gsFrameNumber; // current Framenumber
//        int gsUpdateOrderNumber; // order which is used to update frames, reordering dependend on framenumber, so the playhead doesnt jump back and forth
//        bool gsManipulated = FALSE; // indicating if the framenumber is automatically assigned or if it was manually manipulated
//        bool gsToBeUpdated = TRUE; // This frame/image has changed, the texture needs to be updated
//        bool gsToBeGrabbed = TRUE; // The framenumber has changed so the frame has to be grabbed again (threadable)
//        int gsWidth; //original width of grabbedFrame
//        int gsHeight; //original height of grabbedFrame
//    };

    void setup(string vfMovieName, int _numberOfFrames){
        gmSetupFinished = false;
        loadNewMovieToBeGrabbed(vfMovieName);
        allocateNewNumberOfFrames(_numberOfFrames);
        gmSetupFinished = true;
    }

    bool loadNewMovieToBeGrabbed(string vfMovieName){
        stop(FALSE);

        ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function" + ofToString(vfMovieName));
        ofFile fileToRead(vfMovieName);
        gmMoviePath = fileToRead.getAbsolutePath();
        movieFile.open(gmMoviePath);
        // ofLog(OF_LOG_VERBOSE, "fileToRead.getAbsolutePath(): " + ofToString(fileToRead.getAbsolutePath()));
        // movieFile.read(matOrig);

        if(movieFile.isOpened()){
            // get some meta data about frame.
            gmFrameRate = int(movieFile.get(CV_CAP_PROP_FPS));
            gmTotalFrames = int(movieFile.get(CV_CAP_PROP_FRAME_COUNT));
            gmFrameHeight = int(movieFile.get(CV_CAP_PROP_FRAME_HEIGHT));
            gmFrameWidth = int(movieFile.get(CV_CAP_PROP_FRAME_WIDTH));
            //                double posFrames = movieFile.get(CV_CAP_PROP_POS_FRAMES);
            //                double posMsec = movieFile.get(CV_CAP_PROP_POS_MSEC);
            gmImageRatio = gmFrameWidth/(float)gmFrameHeight;
            gmPixelRatio = 1.0;
        }
        while (!isMovieLoaded()) {
            ofLog(OF_LOG_VERBOSE, "_____________________________________ waiting for movie to load - " + ofToString(gmMoviePath));
        }

        ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function" + ofToString(vfMovieName));

        if (isMovieLoaded()) {
            ofLog(OF_LOG_VERBOSE, "Width: " + ofToString(gmFrameWidth) + " Height: " + ofToString(gmFrameHeight) + " ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) + " totalFrames:" + ofToString(gmTotalFrames) + " getDuration:" + ofToString(gmMovie.getDuration()));
            ofLog(OF_LOG_VERBOSE, "Movie loaded");
        } else {
            ofLog(OF_LOG_VERBOSE, "Movie not loaded");
        }
        return isMovieLoaded();
    }

    void allocateNewNumberOfFrames(int _numberOfFrames){

        gmNumberOfFrames = _numberOfFrames;

        if (isMovieLoaded()) {
            gmCurrAllocating = true;

            grabbedFrame.clear();
            grabbedFrame.resize(gmNumberOfFrames);

            for(int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames(); i++)
            {
                grabbedFrame[i].gsID = i;
                grabbedFrame[i].gsWidth = gmFrameWidth;
                grabbedFrame[i].gsHeight = gmFrameHeight;
                grabbedFrame[i].gsImage.setUseTexture(false); // can also not use texture yet - later: grabbedFrame[i].gsTexture.loadData(grabbedFrame[i].gsImage);
                grabbedFrame[i].gsImage.allocate(grabbedFrame[i].gsWidth, grabbedFrame[i].gsHeight, OF_IMAGE_COLOR);
                grabbedFrame[i].gsTexture.allocate(grabbedFrame[i].gsWidth, grabbedFrame[i].gsHeight,GL_RGB);
//                grabbedFrame[i].gsToBeUpdated = TRUE;
                grabbedFrame[i].gsToBeGrabbed = TRUE;
                grabbedFrame[i].gsFrameNumber = ofMap(i, 0, (returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames() - 1), 0, (gmTotalFrames - 1), TRUE);
                ofLog(OF_LOG_VERBOSE, "grabbedFrame[i].gsFrameNumber:" + ofToString(grabbedFrame[i].gsFrameNumber) + "gmTotalFrames:" + ofToString(gmTotalFrames));
            }
            ofLog(OF_LOG_VERBOSE, "Allocations worked");
        } else{
            ofLog(OF_LOG_VERBOSE, "Movie not loaded");
        }
        gmCurrAllocating = false;
    }

    string getMoviePath(){
        return gmMoviePath;
    }

    void update(){
        if (isMovieLoaded()) {
//            ofLog(OF_LOG_VERBOSE, "____________gmMovie.update() ");
//            for (int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames(); i++) {
//                grabbedFrame[i].gsTexture.loadScreenData(0,0,200,200);
//                grabbedFrame[i].gsImage.update();
//            }
//            for (int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames(); i++) {
//                ofLog(OF_LOG_VERBOSE, "grabbedFrame[i].gsImage.isUsingTexture():" + ofToString(grabbedFrame[i].gsImage.isUsingTexture()));
//                if (grabbedFrame[i].gsToBeUpdated && !grabbedFrame[i].gsImage.isUsingTexture()) { // load textures in proper size
//                    if (!grabbedFrame[i].gsToBeGrabbed ) {
//                        //                    if (gmCalcResizeSwitch) {
//                        //                        grabbedStill[i].gsImage.resize(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
//                        //                    }
//                        grabbedFrame[i].gsTexture.loadData(grabbedFrame[i].gsImage);
////                        grabbedFrame[i].gsTexture.loadScreenData(0,0,400,400);
//                        grabbedFrame[i].gsImage.update();
//                        grabbedFrame[i].gsToBeUpdated = FALSE;
//                        ofLog(OF_LOG_VERBOSE, "Texture updated:" + ofToString(i));
//                    }
//                }
//            }
        }
    }


    int returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames(){
        int tempNumberOfFrames = gmNumberOfFrames;
//        ofLog(OF_LOG_VERBOSE, "returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames() gmNumberOfFrames:" + ofToString(gmNumberOfFrames));
        if (gmNumberOfFrames != grabbedFrame.size()) {
            ofLog(OF_LOG_VERBOSE, "XXX___ Size of gmNumberOfFrames and grabbedFrame.size() differs -> grabbedFrame.size() is used");
            tempNumberOfFrames = grabbedFrame.size();
        }
        return tempNumberOfFrames;
    }

    void setAllToBeGrabbedAndToBeUpdated(){
        ofLog(OF_LOG_VERBOSE, "____________setAllToBeGrabbedAndToBeUpdated()");
        for (int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames(); i++) {
            setOneToBeGrabbedAndToBeUpdated(i, FALSE);
        }
    }

    void setOneToBeGrabbedAndToBeUpdated(int _i, bool _manuallyManipulated){
        ofLog(OF_LOG_VERBOSE, "____________setOneToBeGrabbedAndToBeUpdated() "+ ofToString(isMovieLoaded()));
        if (isMovieLoaded()) {
            grabbedFrame[_i].gsToBeUpdated = TRUE;
            grabbedFrame[_i].gsToBeGrabbed = TRUE;
            grabbedFrame[_i].gsManipulated = _manuallyManipulated;
        }
    }

    bool isMovieLoaded(){
//        ofLog(OF_LOG_VERBOSE, "____________isMovieLoaded() "+ ofToString(isMovieLoaded()));
        return movieFile.isOpened();
    }

    void play(){
        if (isMovieLoaded()) {
            ofLog(OF_LOG_VERBOSE, "____________gmMovie.play() ");
            gmMovie.play();
        }
    }

    int getWidth(){
        return gmFrameWidth;
    }

    int getHeight(){
        return gmFrameHeight;
    }

    void grabNextFrame(int i, bool _useThreads){
        setOneToBeGrabbedAndToBeUpdated(i, TRUE);
        grabbedFrame[i].gsFrameNumber = grabbedFrame[i].gsFrameNumber + 1;
        if (_useThreads) {
            if (!isThreadRunning()) {
                start();
            }
        } else {
            grabToImageAll();
        }
    }

    void grabPreviousFrame(int i, bool _useThreads){
        setOneToBeGrabbedAndToBeUpdated(i, TRUE);
        grabbedFrame[i].gsFrameNumber = grabbedFrame[i].gsFrameNumber - 1;
        if (_useThreads) {
            if (!isThreadRunning()) {
                start();
            }
        } else {
            grabToImageAll();
        }
    }

    void grabOneFrame(int i, int _frame, bool _useThreads){
        setOneToBeGrabbedAndToBeUpdated(i, TRUE);
        grabbedFrame[i].gsFrameNumber = _frame;
        if (_useThreads) {
            if (!isThreadRunning()) {
                start();
            }
        } else {
            grabToImageAll();
        }
    }

    void grabAllFrames(bool _useThreads){
        setAllToBeGrabbedAndToBeUpdated();
        if (_useThreads) {
            if (!isThreadRunning()) {
                start();
            }
        } else {
            grabToImageAll();
        }
    }

    void grabToImage(int i, int _frame, bool _inThread){

        string str = "";
        if (_inThread) {
            str = "In Thread function - ";
        }
        ofLog(OF_LOG_VERBOSE, str + "1 _frame: " + ofToString(_frame) + " getPosition: " + ofToString(gmMovie.getPosition()) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

        if (isMovieLoaded()) {

            _frame = min((gmTotalFrames-1), max(_frame, 0));
            grabbedFrame[i].gsFrameNumber = _frame;

            movieFile.set(CV_CAP_PROP_POS_FRAMES, (double)_frame);
            movieFile.read(matOrig);

            if (grabbedFrame[i].gsImage.isAllocated() && !gmCurrAllocating) {
                ofxCv::copy(matOrig, grabbedFrame[i].gsImage);
                grabbedFrame[i].gsImage.update();
                grabbedFrame[i].gsToBeGrabbed = FALSE;
                ofLog(OF_LOG_VERBOSE, str + "Frame " + ofToString(i) + " saved");
            } else {
                ofLog(OF_LOG_VERBOSE, str + "CRASH AVOIDED grabbedFrame[i].gsImage.isAllocated() FALSE _______________________________");
            }
        }
        ofLog(OF_LOG_VERBOSE, str + "2 _frame: " + ofToString(_frame) + " getPosition: " + ofToString(gmMovie.getPosition()) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));
    }

    bool allGrabbed(){
        int allGrabbed = 0;
        if (isMovieLoaded()) {

            for(int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfFrames(); i++)
            {
                if(!grabbedFrame[i].gsToBeGrabbed){
                    allGrabbed++;
                }
            }
        }
        if (allGrabbed == gmNumberOfFrames){
            return TRUE;
        } else {
            return FALSE;
        }

    }

    void grabToImageAll(){
        if (gmSetupFinished && isMovieLoaded()) { // only start when setup is finished and movie is loaded
            do {
                for (int i = 0; i<grabbedFrame.size(); i++) { // frames are being updated in the order of their framenumber
                    if (grabbedFrame[i].gsToBeGrabbed) {
                        ofLog(OF_LOG_VERBOSE, "In NoneThread Function - gsUpdateOrderNumber:" + ofToString(grabbedFrame[i].gsUpdateOrderNumber) + " Frame:" + ofToString(grabbedFrame[i].gsFrameNumber));
                        grabToImage(i, grabbedFrame[i].gsFrameNumber, false);
                    }
                }
            } while (!allGrabbed());
        }
    }

    // Thread funcions

    void threadedFunction(){

        if (gmSetupFinished && isMovieLoaded()) { // only start when setup is finished and movie is loaded
            lock();
            do {
                for (int i = 0; i<grabbedFrame.size(); i++) {
                    if (grabbedFrame[i].gsToBeGrabbed) {
                        gmThreadCounter++;
                        ofLog(OF_LOG_VERBOSE, "In Thread Function - i:" + ofToString(i) + " Frame:" + ofToString(grabbedFrame[i].gsFrameNumber));
                        grabToImage(i, grabbedFrame[i].gsFrameNumber, true);                    }
                }
            } while (!allGrabbed());
            unlock();
        }
        stop(TRUE);
        ofLog(OF_LOG_VERBOSE, "Closing Thread Function-----------------------------------------------" );

    }

    void start(){

        startThread(true);   // blocking, verbose
        gmThreadCounter = 0;

    }

    void stop(bool rightNow){

        if (!rightNow) {
            waitForThread();
        }
        stopThread();

    }

    //properties

    ofVideoPlayer gmMovie;

//    vector<grabbedFrameStruct> grabbedFrame;
    vector<fakGrabbedMovieStill> grabbedFrame;
    vector<ofVec2f> gmOrderNumberVector;

    int gmFrameWidth;
    int gmFrameHeight;
    bool gmCurrAllocating;
    bool gmHasNoFrames;
    float gmFrameRate;
    int gmTotalFrames;
    float gmImageRatio;
    float gmPixelRatio;
    int gmNumberOfFrames;
    bool gmSetupFinished;
    int gmThreadCounter;
    string gmMoviePath;

    cv::VideoCapture movieFile;
    cv::Mat matOrig;
};


#endif // FAKGRABFRAMES_H
