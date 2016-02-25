#ifndef FAKGRABBEDMOVIE_H
#define FAKGRABBEDMOVIE_H

#include <string>
#include <iostream>
#include "ofMain.h"
#include "fakGrabbedMovieStill.h"

#define REsizeFactor 10.0
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

class fakGrabbedMovie : public ofThread
{
public:
    fakGrabbedMovie(){};

    struct compareOnlyYValueStruct
    {
        bool operator()(const ofVec2f &a, const ofVec2f &b)
        {
            return a.y < b.y;
        }
    } compareYOperator;

    void setup(string vfMovieName, int _numberOfStills, int _gmThumbWidth, int _gmThumbHeight, bool _showPlaceHolder){

        gmOrderNumberVector.clear();

        gmThumbWidth = _gmThumbWidth;
        gmThumbHeight = _gmThumbHeight;
        gmMouseEventsEnabled = false;

        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        shader.linkProgram();

        setInPointImage.load("images/MoviePrint_SetInPoint_v001_00000.png");
        setOutPointImage.load("images/MoviePrint_SetOutPoint_v001_00000.png");
        frameForwardImage.load("images/MoviePrint_FrameForward_v001_00000.png");
        frameForward2Image.load("images/MoviePrint_FrameForward_v001_00001.png");
        frameForward3Image.load("images/MoviePrint_FrameForward_v001_00002.png");
        frameBackwardImage.load("images/MoviePrint_FrameBackward_v001_00000.png");
        frameBackward2Image.load("images/MoviePrint_FrameBackward_v001_00001.png");
        frameBackward3Image.load("images/MoviePrint_FrameBackward_v001_00002.png");
        scrubImage.load("images/MoviePrint_Scrubb_v002_00000.png");
        corruptImage.load("images/MoviePrint_Corrupt_v001_00000.jpg");
        emptyImage.load("images/MoviePrint_Corrupt_00000.jpg");
        updatingStill.load("images/MoviePrint_StillUpdating_v002_00000.png");
        headerImage.load("images/MoviePrint_Layout_Header_v001_00000.png");

        setNumberOfStills(_numberOfStills);

//        gmFontStashUbuntu.setup("Ubuntu-Light.ttf", 1.03);
//        gmFontStashFranchise.setup("Franchise-Bold.ttf", 0.9);
//        gmFontStashHelveticaLight.setup("HelveticaNeueLTCom-Lt.ttf");
//        gmFontStashHelveticaMedium.setup("HelveticaNeueLTCom-Md.ttf");


//        franchiseFontRightSize = 0;
//        stringMargin = 2;

//        gmSetTitleInfo = TRUE; //create new title size und umbruch

        loadNewMovieToBeGrabbed(vfMovieName, gmNumberOfStills, _showPlaceHolder, false);
//        loadNewMovieToBeGrabbed(vfMovieName);

        gmSetupFinished = TRUE;
        gmShowFramesUI = TRUE;

    }

    bool loadNewMovieToBeGrabbed(string vfMovieName, int _numberOfStills, bool _showPlaceHolder, bool _addListener){

        setNumberOfStills(_numberOfStills);
        stop(FALSE);

        isMovieLoaded = FALSE;

        if (!_showPlaceHolder) {
            ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function");
            gmMovie.load(vfMovieName);
            ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function");
            ofLog(OF_LOG_VERBOSE, "_____________________________________ " + ofToString(vfMovieName));

            if (gmMovie.isLoaded()) {
                isMovieLoaded = TRUE;
                if (gmMovie.getTotalNumFrames() < 2) { //check if movie has only one frame, if so than calculate totalframes and later use setPosition instead of setFrame
                    gmHasNoFrames = TRUE;
                    gmFrameRate = 25;
                    gmTotalFrames = gmMovie.getDuration() * gmFrameRate;
                } else {
                    gmHasNoFrames = FALSE;
                    gmTotalFrames = gmMovie.getTotalNumFrames();
                    gmFrameRate = ceil(gmTotalFrames/gmMovie.getDuration());
                }
                gmImageRatio = gmMovie.getWidth()/gmMovie.getHeight();
                gmPixelRatio = gmMovie.getPixelFormat();
                ofLog(OF_LOG_VERBOSE, "ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) + " totalFrames:" + ofToString(gmTotalFrames) + " getDuration:" + ofToString(gmMovie.getDuration()));

            } else {
                gmTotalFrames = gmNumberOfStills;
                ofLog(OF_LOG_VERBOSE, "gmTotalFrames set manually");

            }

        }
        allocateNewNumberOfStills(gmNumberOfStills, gmThumbWidth, gmThumbHeight, _showPlaceHolder, _addListener);

//        getMovieInformation(vfMovieName);

        gmSetTitleInfo = TRUE;

        return isMovieLoaded;
    }

    void update(){
        if (isMovieLoaded) {
            gmMovie.update();
        }
    }

    void play(){
        if (isMovieLoaded) {
            gmMovie.play();
        }
    }

    void setNumberOfStills(int _numberOfStills){
        gmNumberOfStills = _numberOfStills;
    }

    int returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(){
        int tempNumberOfStills = gmNumberOfStills;
        if (gmNumberOfStills != grabbedStill.size()) {
            ofLog(OF_LOG_VERBOSE, "XXX___ Size of gmNumberOfStills and grabbedStill.size() differs -> grabbedStill.size() is used");
            tempNumberOfStills = grabbedStill.size();
        }
        return tempNumberOfStills;
    }

    void updateOrderNumber(){

        gmOrderNumberVector.clear();

        for (int i = 0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++) {
            gmOrderNumberVector.push_back(ofVec2f(i,grabbedStill[i].gsFrameNumber));
        }
        sort(gmOrderNumberVector.begin(), gmOrderNumberVector.end(), compareYOperator);
        for (int i = 0; i<gmNumberOfStills; i++) {
            grabbedStill[i].gsUpdateOrderNumber = gmOrderNumberVector.at(i).x;
        }
    }

    void updateAllFrameNumbers(vector<int>* _gridTimeArray){
        if (isMovieLoaded) {
            for (int i = 0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++) {
                grabbedStill[i].gsFrameNumber = _gridTimeArray->at(i);
                grabbedStill[i].gsUpdateOrderNumber = i;
                grabbedStill[i].gsToBeUpdated = TRUE;
                grabbedStill[i].gsToBeGrabbed = TRUE;
                grabbedStill[i].gsManipulated = FALSE;
            }
            updateOrderNumber();
        }
    }

    void allocateNewNumberOfStills(int _numberOfStills, int _gmThumbWidth, int _gmThumbHeight, bool _drawPlaceHolder, bool _addListener){
        if (isMovieLoaded) {
            gmCurrAllocating = true;
            gmThumbWidth = _gmThumbWidth;
            gmThumbHeight = _gmThumbHeight;

            stop(TRUE);
            while (isThreadRunning()) {
                ofLog(OF_LOG_VERBOSE, "allocateNewNumberOfStills is waiting for thread to stop");
            }

////          unregister All Mouse Events of the Stills (old gmNumberOfStills)
//            disableMouseEvents();

            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);

//            if (_addListener) {
//                enableMouseEvents();
//            }
            for(int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
//                if (_addListener) {
//                    ofAddListener(grabbedStill[i].gsClickedInside, this, &fakGrabbedMovie::scrubMovie);
//                    ofAddListener(grabbedStill[i].gsMovedInside, this, &fakGrabbedMovie::rollOverMovie);
//                }
                grabbedStill[i].gsID = i;
                grabbedStill[i].gsX = 0;
                grabbedStill[i].gsY = 0;
                grabbedStill[i].gsDrawWidth = 160;
                grabbedStill[i].gsDrawHeight = 90;
                grabbedStill[i].gsUpperLimitY = gmUpperLimitY;
                grabbedStill[i].gsLowerLimitY = gmLowerLimitY;
                grabbedStill[i].gsLeftLimitX = gmLeftLimitX;
                grabbedStill[i].gsRightLimitX = gmRightLimitX;
                if (gmCalcResizeSwitch) {
                    grabbedStill[i].gsResizeFactor = REsizeFactor;
                } else {
                    grabbedStill[i].gsResizeFactor = 1.0;
                }
                grabbedStill[i].gsWidth = gmMovie.getWidth()/grabbedStill[i].gsResizeFactor;
                grabbedStill[i].gsHeight = gmMovie.getHeight()/grabbedStill[i].gsResizeFactor;
                grabbedStill[i].gsImage.setUseTexture(FALSE);
                grabbedStill[i].gsImage.allocate(gmMovie.getWidth(), gmMovie.getHeight(), OF_IMAGE_COLOR);
                grabbedStill[i].gsTexture.allocate(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight,GL_RGB);
                grabbedStill[i].gsToBeUpdated = TRUE;
            }

            // creating maskFbo - for rounding stills
            maskFbo.allocate(grabbedStill[0].gsWidth,grabbedStill[0].gsHeight);
            ofPushStyle();
            maskFbo.begin();
            ofEnableAlphaBlending();
            ofClear(0,0,0,255);
            ofClear(0, 0, 0, 0);
            ofSetColor(255, 255, 255, 255);
            ofDrawRectRounded(0, 0, grabbedStill[0].gsWidth, grabbedStill[0].gsHeight, grabbedStill[0].gsWidth/64);
            maskFbo.end();
            ofPopStyle();

            gmSetTitleInfo = TRUE; //create new title size und umbruch

            ofLog(OF_LOG_VERBOSE, "Allocations worked");
        } else if (_drawPlaceHolder){
            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);
        }
        gmCurrAllocating = false;
    }

    void drawGridOfStills(float _x, float _y, int _gridColumns, float _gridMargin, float _scrollAmount, float _scaleFactor, float _alpha, bool _isBeingPrinted, bool _isActive, bool _superKeyPressed, bool _shiftKeyPressed, bool _drawPlaceHolder){

        // draw all frames
        ofPushStyle();
        ofPushMatrix();
        ofEnableAlphaBlending();
        ofSetColor(FAK_ORANGECOLOR); // draw title rect

        ofSetColor(255, 255, 255, 255); // draw stills
        for(int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++)
        {
            float tempX = (_x + (gmThumbWidth+_gridMargin)*(i%_gridColumns)) * _scaleFactor;
            float tempY = (_y + (gmThumbHeight+_gridMargin)*(i/_gridColumns)) * _scaleFactor;
            drawStill(i, tempX, tempY, gmThumbWidth * _scaleFactor, gmThumbHeight * _scaleFactor, 1, _superKeyPressed, _shiftKeyPressed, _drawPlaceHolder);
        }

//        if (_isBeingPrinted) {
//            gmSetTitleInfo = TRUE; //create new title size und umbruch
//        }

        ofPopMatrix();
        ofPopStyle();

    }

    void drawStill(int i, float _x, float _y, float _w, float _h, float _alpha, bool _superKeyPressed, bool _shiftKeyPressed, bool _drawPlaceHolder){

        if (isMovieLoaded) {

            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);

            grabbedStill[i].gsX = _x;
            grabbedStill[i].gsY = _y;
            grabbedStill[i].gsDrawWidth = _w;
            grabbedStill[i].gsDrawHeight = _h;
            grabbedStill[i].gsResizeFactor = gmMovie.getWidth()/_w;

            if (grabbedStill[i].gsToBeUpdated) { // load textures in proper size
                if (!grabbedStill[i].gsToBeGrabbed) {
                    if (gmCalcResizeSwitch) {
                        grabbedStill[i].gsImage.resize(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
                    }
                    grabbedStill[i].gsTexture.loadData(grabbedStill[i].gsImage);
                    grabbedStill[i].gsToBeUpdated = FALSE;
                }
            }

            shader.begin(); // draw still with rounded corners
//            shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
            shader.setUniformTexture("maskTex", maskFbo.getTexture(), 1 );

            ofSetColor(255, 255, 255, 255);
            grabbedStill[i].gsTexture.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);

            shader.end();

            // draw selection
            ofPushStyle();
            ofSetColor(255, 255, 255, 30);
            if (grabbedStill[i].gsRollOver) {
                int tempSelectionWidth = 2;
                ofDrawRectangle(grabbedStill[i].gsX - tempSelectionWidth/2, grabbedStill[i].gsY - tempSelectionWidth/2, grabbedStill[i].gsDrawWidth + tempSelectionWidth, grabbedStill[i].gsDrawHeight + tempSelectionWidth);
            }
            ofPopStyle();

            // draw update image
            if (grabbedStill[i].gsToBeGrabbed) {
                ofPushMatrix();
                ofPushStyle();
                ofSetColor(0, 0, 0, 130);
                ofDrawRectangle(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
                ofSetColor(255, 255, 255, 200);
                updatingStill.drawSubsection(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight,updatingStill.getWidth()/2 - grabbedStill[i].gsDrawWidth/2, updatingStill.getHeight()/2 - grabbedStill[i].gsDrawHeight/2);
                ofPopStyle();
                ofPopMatrix();
            }

//            if (gmShowFramesUI) { // drawing UI
//                drawStillUI(i, grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight, _alpha);
//            }

            // drawing overlay graphics
                if (grabbedStill[i].gsRollOver) {

                    ofSetColor(255, 5);
                    if (grabbedStill[i].gsRollOver3) {
                        ofSetColor(255, 20);
                    }
                    ofDrawRectRounded(grabbedStill[i].gsX, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[0].gsDrawWidth/64);
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver3) {
                        ofSetColor(255);
                    }
                    setInPointImage.draw(grabbedStill[i].gsX, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);

                    ofSetColor(255, 5);
                    if (grabbedStill[i].gsRollOver4) {
                        ofSetColor(255, 20);
                    }
                    ofDrawRectRounded(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[0].gsDrawWidth/64);
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver4) {
                        ofSetColor(255);
                    }
                    setOutPointImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);

                    ofSetColor(255, 5);
                    if (grabbedStill[i].gsRollOver0) {
                        ofSetColor(255, 20);
                    }
                    ofDrawRectRounded(grabbedStill[i].gsX + grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight, grabbedStill[i].gsDrawHeight, grabbedStill[0].gsDrawWidth/64);
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver0) {
                        ofSetColor(255);
                    }
            //                    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
            //                    scrubImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth/2, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight/2, scrubImage.getWidth()/2, scrubImage.getHeight()/2);
            //                    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
                    scrubImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth/2 - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight, grabbedStill[i].gsDrawHeight);

                    ofSetColor(255, 5);
                    if (grabbedStill[i].gsRollOver1) {
                        ofSetColor(255, 20);
                    }
                    ofDrawRectRounded(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[0].gsDrawWidth/64);
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver1) {
                        ofSetColor(255);
                    }
                    if (_superKeyPressed) {
                        frameBackward3Image.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else if(_shiftKeyPressed){
                        frameBackward2Image.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else {
                        frameBackwardImage.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    }

                    ofSetColor(255, 5);
                    if (grabbedStill[i].gsRollOver2) {
                        ofSetColor(255, 20);
                    }
                    ofDrawRectRounded(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[0].gsDrawWidth/64);
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver2) {
                        ofSetColor(255);
                    }
                    if (_superKeyPressed) {
                        frameForward3Image.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else if(_shiftKeyPressed){
                        frameForward2Image.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else {
                        frameForwardImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    }

                }
            ofPopStyle();
            ofSetColor(255);
        } else if (_drawPlaceHolder){
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);

            ofPushMatrix();
            ofPushStyle();
            ofSetColor(FAK_MIDDLEGRAY);
            // updatingStill.drawSubsection(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight,updatingStill.width/2 - grabbedStill[i].gsDrawWidth/2, updatingStill.height/2 - grabbedStill[i].gsDrawHeight/2);

            ofDrawRectangle(_x, _y, _w, _h);

            ofPopStyle();
            ofPopMatrix();

            ofPopStyle();
            ofSetColor(255);        }
    }

    void start(){

//        startThread(true, false);   // blocking, verbose
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
    ofVideoPlayer gmMovieScrub;

    vector<fakGrabbedMovieStill> grabbedStill;
    vector<ofVec2f> gmOrderNumberVector;

    bool devTurnOffMovieSwitch = FALSE;
    bool isMovieLoaded = FALSE;

    float gmDuration;
    string gmMovieName;
    int gmTotalFrames;
    float gmImageRatio;
    float gmPixelRatio;
    int gmFrameRate;
    bool gmSetupFinished;
    int gmNumberLoadedCounter;
    int gmNumberGrabbedCounter;
    bool gmCalcResizeSwitch;
    bool vfFramesToTimeSwitch;
    bool gmScrubMovie;
    bool gmRollOver;
    int gmRollOverMovieID;
    int gmRollOverMovieButtonID;
    int gmScrubID;
    int gmNumberOfStills;
    bool gmHasNoFrames;
    int gmThreadCounter;
    int gmUpperLimitY;
    int gmLowerLimitY;
    int gmLeftLimitX;
    int gmRightLimitX;
    int gmThumbWidth;
    int gmThumbHeight;
    bool gmShowFramesUI;
    bool gmMouseEventsEnabled;
    bool gmCurrAllocating;

    ofImage setInPointImage;
    ofImage setOutPointImage;
    ofImage frameForwardImage;
    ofImage frameForward2Image;
    ofImage frameForward3Image;
    ofImage frameBackwardImage;
    ofImage frameBackward2Image;
    ofImage frameBackward3Image;
    ofImage corruptImage;
    ofImage updatingStill;
    ofImage headerImage;
    ofImage emptyImage;
    ofImage scrubImage;

    //ofxFontStash gmFontStashHelveticaLight;
    //ofxFontStash gmFontStashHelveticaMedium;
    //ofxFontStash gmFontStashUbuntu;
    //ofxFontStash gmFontStashFranchise;
    int tempFontSize[24] = {6, 10, 14, 18, 22, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 80, 92, 108, 128, 256, 300};

    int franchiseFontRightSize;
    int stringMargin;
    ofFilePath gmLoadedFilePath;
    bool gmSetTitleInfo;
    string gmInfCollected;
    string gmInfCol1, gmInfCol2, gmInfCol3;
    string gmMIFileName, gmMIFileExtension, gmMIFormat, gmMIFormatString, gmMIFileSizeString, gmMIDurationString1, gmMIFrameCount, gmMIWidth, gmMIHeight, gmMIDisplayAspectRatioString, gmMIFrameRateString;
    string gmMIVFormat, gmMIFormatInfo, gmMIBitRate, gmMIPixelAspectRatio, gmMIDisplayAspectRatio, gmMIFrameRate_ModeString, gmMIColorSpace, gmMIChromaSubsampling, gmMIBitDepthString, gmMIInterlacementString;
    string gmMIAFormat, gmMIAChannelsString, gmMIASamplingRate, gmMIFilePath;

    string gmMIFileNameClean, gmMIFilePathOhne;

    // used for rounded corner mask
    ofShader shader;
    ofFbo maskFbo;
};

#endif // FAKGRABBEDMOVIE_H
