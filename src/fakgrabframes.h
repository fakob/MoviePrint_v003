#ifndef FAKGRABFRAMES_H
#define FAKGRABFRAMES_H

#include <string>
#include <iostream>
#include "ofMain.h"

#include "ofxAvAudioPlayer.h"
#include "ofxAvVideoPlayer.h"

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
    struct grabbedFrameStruct {
        ofImage gsImage; //image into which the videoframe is grabbed
        ofTexture gsTexture; //texture into which the grabbedFrame is copied - this is drawn to screen - this is also resized if necessary
        int gsID; // ID in Array
        int gsFrameNumber; // current Framenumber
        int gsUpdateOrderNumber; // order which is used to update frames, reordering dependend on framenumber, so the playhead doesnt jump back and forth
        bool gsManipulated = FALSE; // indicating if the framenumber is automatically assigned or if it was manually manipulated
        bool gsToBeUpdated = TRUE; // This frame has changed somehow, it needs to be redrawn
        bool gsToBeGrabbed = TRUE; // The framenumber has changed so the frame has to be grabbed again (threadable)
        int gsWidth; //original width of grabbedStill
        int gsHeight; //original height of grabbedStill
    };

    void setup(string vfMovieName, int _numberOfStills){

        gmSetupFinished = false;
        loadNewMovieToBeGrabbed(vfMovieName, _numberOfStills);
        gmSetupFinished = true;
    }

    bool loadNewMovieToBeGrabbed(string vfMovieName, int _numberOfStills){

        gmNumberOfStills = _numberOfStills;
        stop(FALSE);

        ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function");
        gmMovie.load(vfMovieName);
        ofLog(OF_LOG_VERBOSE, "_____________________________________ " + ofToString(vfMovieName));
        ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function");

        if (gmMovie.isLoaded()) {
            if (gmMovie.getTotalNumFrames() < 2) { //check if movie has only one frame, if so than calculate totalframes and later use setPosition instead of setFrame
                gmHasNoFrames = TRUE;
                gmFrameRate = 25;
                gmTotalFrames = gmMovie.getDuration() * gmFrameRate;
            } else {
                gmHasNoFrames = FALSE;
                gmTotalFrames = gmMovie.getTotalNumFrames();
                gmFrameRate = ceil(gmTotalFrames/gmMovie.getDuration());
            }
            gmFrameWidth = gmMovie.getWidth();
            gmFrameHeight = gmMovie.getHeight();
            gmImageRatio = gmFrameWidth/gmFrameHeight;
            gmPixelRatio = gmMovie.getPixelFormat();
            ofLog(OF_LOG_VERBOSE, "ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) + " totalFrames:" + ofToString(gmTotalFrames) + " getDuration:" + ofToString(gmMovie.getDuration()));

        } else {
            gmTotalFrames = gmNumberOfStills;
            ofLog(OF_LOG_VERBOSE, "Movie not loaded - gmTotalFrames set manually");
        }

        allocateNewNumberOfStills(gmNumberOfStills);

        return isMovieLoaded();
    }

    void allocateNewNumberOfStills(int _numberOfStills){
        if (isMovieLoaded()) {
            gmCurrAllocating = true;

            grabbedFrame.clear();
            grabbedFrame.resize(gmNumberOfStills);

            for(int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
                grabbedFrame[i].gsID = i;
                grabbedFrame[i].gsWidth = gmMovie.getWidth();
                grabbedFrame[i].gsHeight = gmMovie.getHeight();
                grabbedFrame[i].gsImage.setUseTexture(TRUE); // can also not use texture yet - later: grabbedFrame[i].gsTexture.loadData(grabbedFrame[i].gsImage);
                grabbedFrame[i].gsImage.allocate(gmMovie.getWidth(), gmMovie.getHeight(), OF_IMAGE_COLOR);
                grabbedFrame[i].gsTexture.allocate(grabbedFrame[i].gsWidth, grabbedFrame[i].gsHeight,GL_RGB);
                grabbedFrame[i].gsToBeUpdated = TRUE;
                grabbedFrame[i].gsFrameNumber = ofMap(i, 0, returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfStills(), 0, gmTotalFrames, TRUE);
            }

            ofLog(OF_LOG_VERBOSE, "Allocations worked");
        } else{
            ofLog(OF_LOG_VERBOSE, "Movie not loaded");
        }
        gmCurrAllocating = false;
    }

    void update(){
        if (isMovieLoaded()) {
//            ofLog(OF_LOG_VERBOSE, "____________gmMovie.update() ");
            gmMovie.update();
        }
    }


    int returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfStills(){
        int tempNumberOfStills = gmNumberOfStills;
        if (gmNumberOfStills != grabbedFrame.size()) {
            ofLog(OF_LOG_VERBOSE, "XXX___ Size of gmNumberOfStills and grabbedFrame.size() differs -> grabbedFrame.size() is used");
            tempNumberOfStills = grabbedFrame.size();
        }
        return tempNumberOfStills;
    }

    bool isMovieLoaded(){
//        ofLog(OF_LOG_VERBOSE, "____________isMovieLoaded() "+ ofToString(gmMovie.isLoaded()));
        return gmMovie.isLoaded();
    }

    void grabToImage(int i, int _frame){

        ofLog(OF_LOG_VERBOSE, "1 _frame: " + ofToString(_frame) + " getPosition: " + ofToString(gmMovie.getPosition()) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

        if (isMovieLoaded()) {

            _frame = min((gmTotalFrames-1), max(_frame, 0));

//            if (gmHasNoFrames) { // movies die "keine frames haben" benoetigen setPosition, deshalb sind auch meist die ersten paar frames "kaputt"
//                if (_frame < 5) {
//                    _frame = 5;
//                }
//                gmMovie.setPosition((float)(_frame-2)/(float)(gmTotalFrames-1)); //setPosition Movies brauchen das setzen des frames davor und dann nextFrame
////                gmMovie.nextFrame();
//                ofSleepMillis(TimeToWaitForMovie);
//                ofLog(OF_LOG_VERBOSE, "setPosition1: " + ofToString(gmMovie.getPosition()) + " _frame: " + ofToString(_frame) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

//            } else {
//                if (_frame==0) {
//                    gmMovie.setFrame(0);
//                    gmMovie.update();
//                    ofSleepMillis(TimeToWaitForMovie);
//                } else {
//                    gmMovie.setFrame(_frame);
//                    gmMovie.update();
//                    ofSleepMillis(TimeToWaitForMovie);
//                }
//                gmMovie.play();
//                gmMovie.setPosition(_frame/(float)gmTotalFrames);
//                gmMovie.update();
//                gmMovie.stop();
//                ofSleepMillis(TimeToWaitForMovie);
//                ofLog(OF_LOG_VERBOSE, "setPosition2: " + ofToString(gmMovie.getPosition()) + " _frame: " + ofToString(_frame) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

//            }

            gmMovie.play();
            ofSleepMillis(TimeToWaitForMovie);
//            gmMovie.setPosition(_frame/(float)gmTotalFrames);
            gmMovie.update();
            gmMovie.setFrame(_frame);
            gmMovie.update();
            ofSleepMillis(TimeToWaitForMovie);
//            gmMovie.stop();

            if (grabbedFrame[i].gsImage.isAllocated() && !gmCurrAllocating) {
                grabbedFrame[i].gsImage.setFromPixels(gmMovie.getPixels());
                grabbedFrame[i].gsImage.update();
                grabbedFrame[i].gsToBeGrabbed = FALSE;
                ofLog(OF_LOG_VERBOSE, "Frame " + ofToString(i) + " saved");
            } else {
                ofLog(OF_LOG_VERBOSE, "CRASH AVOIDED grabbedStill[i].gsImage.isAllocated() FALSE _______________________________");
            }
        }
        ofLog(OF_LOG_VERBOSE, "2 _frame: " + ofToString(_frame) + " getPosition: " + ofToString(gmMovie.getPosition()) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));
    }

    ofImage getImage(int i){
        return grabbedFrame[i].gsImage;
    }

    void grabToImageFunction(){
        if (gmSetupFinished && isMovieLoaded()) { // only start when setup is finished and movie is loaded
            do {
                for (int i = 0; i<grabbedFrame.size(); i++) { // frames are being updated in the order of their framenumber
                    if (grabbedFrame[i].gsToBeGrabbed) {
//                        gmThreadCounter++;
//                        ofLog(OF_LOG_VERBOSE, "In NoneThread Function - gsUpdateOrderNumber:" + ofToString(grabbedFrame[i].gsUpdateOrderNumber) + " Frame:" + ofToString(grabbedFrame[i].gsFrameNumber));
                        grabToImage(i, grabbedFrame[i].gsFrameNumber);
                    }
                }

            } while (!allGrabbed());
        }
    }

    bool allGrabbed(){
        int allGrabbed = 0;
        if (isMovieLoaded()) {

            for(int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
                if(!grabbedFrame[i].gsToBeGrabbed){
                    allGrabbed++;
                }
            }
        }
        if (allGrabbed == gmNumberOfStills){
            return TRUE;
        } else {
            return FALSE;
        }

    }

    // Thread funcions

    void threadedFunction(){

    }

    void start(){

        startThread(true);   // blocking, verbose
//        gmThreadCounter = 0;

    }

    void stop(bool rightNow){

        if (!rightNow) {
            waitForThread();
        }
        stopThread();

    }

    //properties

    // used for ofxAvCodec addon
//    ofxAvVideoPlayer gmMovie;

    ofVideoPlayer gmMovie;

    vector<grabbedFrameStruct> grabbedFrame;
    vector<ofVec2f> gmOrderNumberVector;

    int gmFrameWidth;
    int gmFrameHeight;
    bool gmCurrAllocating;
    bool gmHasNoFrames;
    float gmFrameRate;
    int gmTotalFrames;
    float gmImageRatio;
    float gmPixelRatio;
    int gmNumberOfStills;
    bool gmSetupFinished;

};


#endif // FAKGRABFRAMES_H
