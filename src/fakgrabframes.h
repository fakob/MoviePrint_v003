/*

#ifndef FAKGRABFRAMES_H
#define FAKGRABFRAMES_H

#include <string>
#include <iostream>
#include "ofMain.h"
#include "fakGrabbedMovieStill.h"

//#include "ofxAvAudioPlayer.h"
//#include "ofxAvVideoPlayer.h"

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

class fakGrabFrames : public ofThread
{
public:
    fakGrabFrames(){};

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

        gmSetupFinished = TRUE;
        gmShowFramesUI = TRUE;

    }

    bool loadNewMovieToBeGrabbed(string vfMovieName, int _numberOfStills, bool _showPlaceHolder, bool _addListener){

        setNumberOfStills(_numberOfStills);
        stop(FALSE);

        isMovieLoaded2 = FALSE;

        if (!_showPlaceHolder) {
            ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function");

            // used for AvCodec addon
//            gmMovie.setupAudioOut(2, 44100); // resample to stereo, 44khz

            gmMovie.load(vfMovieName);
            ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function");
            ofLog(OF_LOG_VERBOSE, "_____________________________________ " + ofToString(vfMovieName));

            if (gmMovie.isLoaded()) {
                isMovieLoaded2 = TRUE;
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
//                gmPixelRatio = gmMovie.getPixelFormat();
                // quickfix for ofxAvCodec
                gmPixelRatio = 1.0;
                ofLog(OF_LOG_VERBOSE, "ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) + " totalFrames:" + ofToString(gmTotalFrames) + " getDuration:" + ofToString(gmMovie.getDuration()));

            } else {
                gmTotalFrames = gmNumberOfStills;
                ofLog(OF_LOG_VERBOSE, "gmTotalFrames set manually");

            }

        }
        allocateNewNumberOfStills(gmNumberOfStills, gmThumbWidth, gmThumbHeight, _showPlaceHolder, _addListener);

//        getMovieInformation(vfMovieName);

        gmSetTitleInfo = TRUE;

        return isMovieLoaded();
    }

//    void audioOut( float * buffer, int nFrames, int nChannels ){
//        gmMovie.audioOut( buffer, nFrames, nChannels );
//    }

    void getMovieInformation(string _vfMovieName){

        gmMIFilePathOhne = getMoviePathName();
        gmMIFilePath = "FilePath: " + gmMIFilePath;


//        //Information about MediaInfo
//        MediaInfo MI;
//        MI.Open(__T(_vfMovieName));

//        MI.Option(__T("Inform"), __T("General;Name : %FileName%.%FileExtension%\\r\\n:::Format : %Format%\\r\\n:::Format/String : %Format/String%\\r\\n:::FileSize : %FileSize/String%\\r\\n:::Duration : %Duration/String1%\\r\\n:::\nVideo;FrameCount : %FrameCount%\\r\\n:::Size : %Width%x%Height%\\r\\n:::DisplayAspectRatio : %DisplayAspectRatio/String%\\r\\n:::FrameRate : %FrameRate/String%"));

//        gmInfCol1 = MI.Inform();
//        gmInfCol1 = ReplaceString(gmInfCol1, ":::", "\n");

//        gmMIFileName = MI.Get(Stream_General, 0, __T("FileName"), Info_Text).c_str();
//        gmMIFileExtension = MI.Get(Stream_General, 0, __T("FileExtension"), Info_Text).c_str();
//        gmMIFormat = MI.Get(Stream_General, 0, __T("Format"), Info_Text).c_str();
//        gmMIFormatString = MI.Get(Stream_General, 0, __T("Format/String"), Info_Text).c_str();
//        gmMIFileSizeString = MI.Get(Stream_General, 0, __T("FileSize/String"), Info_Text).c_str();
//        gmMIDurationString1 = MI.Get(Stream_General, 0, __T("Duration/String1"), Info_Text).c_str();

//        gmMIFrameCount = MI.Get(Stream_Video, 0, __T("FrameCount"), Info_Text).c_str();
//        gmMIWidth = MI.Get(Stream_Video, 0, __T("Width"), Info_Text).c_str();
//        gmMIHeight = MI.Get(Stream_Video, 0, __T("Height"), Info_Text).c_str();
//        gmMIWidth = gmMIWidth + "x" + gmMIHeight;
//        gmMIDisplayAspectRatioString = MI.Get(Stream_Video, 0, __T("DisplayAspectRatio/String"), Info_Text).c_str();
//        gmMIFrameRateString = MI.Get(Stream_Video, 0, __T("FrameRate/String"), Info_Text).c_str();
//        gmMIVFormat = MI.Get(Stream_Video, 0, __T("Format"), Info_Text).c_str();
//        gmMIFormatInfo = MI.Get(Stream_Video, 0, __T("Format/Info"), Info_Text).c_str();
//        gmMIBitRate = MI.Get(Stream_Video, 0, __T("BitRate"), Info_Text).c_str();
//        gmMIPixelAspectRatio = MI.Get(Stream_Video, 0, __T("PixelAspectRatio"), Info_Text).c_str();
//        gmMIDisplayAspectRatio = MI.Get(Stream_Video, 0, __T("DisplayAspectRatio"), Info_Text).c_str();
//        gmMIFrameRate_ModeString = MI.Get(Stream_Video, 0, __T("FrameRate_Mode/String"), Info_Text).c_str();
//        gmMIColorSpace = MI.Get(Stream_Video, 0, __T("ColorSpace"), Info_Text).c_str();
//        gmMIChromaSubsampling = MI.Get(Stream_Video, 0, __T("ChromaSubsampling"), Info_Text).c_str();
//        gmMIBitDepthString = MI.Get(Stream_Video, 0, __T("BitDepth/String"), Info_Text).c_str();
//        gmMIInterlacementString = MI.Get(Stream_Video, 0, __T("Interlacement/String"), Info_Text).c_str();

//        gmMIAFormat = MI.Get(Stream_Audio, 0, __T("Format"), Info_Text).c_str();
//        gmMIAChannelsString = MI.Get(Stream_Audio, 0, __T("Channel(s)/String"), Info_Text).c_str();
//        gmMIASamplingRate = MI.Get(Stream_Audio, 0, __T("SamplingRate/String"), Info_Text).c_str();

//        gmMIFileNameClean = gmMIFileName + "." + gmMIFileExtension;

//        MI.Option(__T("Inform"), __T("Video;Video\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::PixelAspectRatio : %PixelAspectRatio%\\r\\n:::DisplayAspectRatio : %DisplayAspectRatio%\\r\\n:::FrameRate_Mode/String : %FrameRate_Mode/String%\\r\\n:::ColorSpace : %ColorSpace%\\r\\n:::ChromaSubsampling : %ChromaSubsampling%\\r\\n:::BitDepth/String : %BitDepth/String%\\r\\n:::Interlacement/String : %Interlacement/String%"));

//        gmInfCol2 = MI.Inform().c_str();
//        gmInfCol2 = ReplaceString(gmInfCol2, ":::", "\n");

//        MI.Option(__T("Inform"), __T("Audio;Audio\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::Channel(s)/String : %Channel(s)/String%\\r\\n:::ChannelPositons : %ChannelPositons%\\r\\n:::SamplingRate/String : %SamplingRate/String%"));

//        gmInfCol3 = MI.Inform().c_str();
//        gmInfCol3 = ReplaceString(gmInfCol3, ":::", "\n");

//        MI.Close();

    }

    string StringToUpper(string strToConvert){
        std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
        return strToConvert;
    }

    string getMoviePathName(){
        gmMIFilePath = ofToString(gmMovie.getMoviePath());
        vector<string> tempVectorString = ofSplitString(gmMIFilePath, "/");
        tempVectorString.pop_back();
        return ofJoinString(tempVectorString, "/") + "/";
    }

    void update(){
        if (isMovieLoaded()) {
            gmMovie.update();
        }
    }

    void play(){
        if (isMovieLoaded()) {
            gmMovie.play();
        }
    }

    bool isMovieLoaded(){
//        ofLog(OF_LOG_VERBOSE, "____________isMovieLoaded() "+ ofToString(gmMovie.isLoaded()));
        return gmMovie.isLoaded();
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
        if (isMovieLoaded()) {
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
        if (isMovieLoaded()) {
            gmCurrAllocating = true;
            gmThumbWidth = _gmThumbWidth;
            gmThumbHeight = _gmThumbHeight;

            stop(TRUE);
            while (isThreadRunning()) {
                ofLog(OF_LOG_VERBOSE, "allocateNewNumberOfStills is waiting for thread to stop");
            }

            // unregister All Mouse Events of the Stills (old gmNumberOfStills)
            disableMouseEvents();

            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);

            if (_addListener) {
                enableMouseEvents();
            }
            for(int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
                if (_addListener) {
                    ofAddListener(grabbedStill[i].gsClickedInside, this, &fakGrabFrames::scrubMovie);
                    ofAddListener(grabbedStill[i].gsMovedInside, this, &fakGrabFrames::rollOverMovie);
                }
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

    void setAllToBeGrabbedAndToBeUpdated(){
        if (isMovieLoaded()) {
            for (int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++) {
                grabbedStill[i].gsToBeUpdated = TRUE;
                grabbedStill[i].gsToBeGrabbed = TRUE;
                grabbedStill[i].gsManipulated = FALSE;
            }
        }
    }

    void grabToImage(int i, int _frame){

        ofLog(OF_LOG_VERBOSE, "before grabToImage: " + ofToString(gmMovie.getPosition()) + " _frame: " + ofToString(_frame) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

        if (isMovieLoaded()) {

            if (_frame < 0) {
                _frame = 0;
            }
            if (_frame > gmTotalFrames-1) {
                _frame = gmTotalFrames-1;
            }

            if (gmHasNoFrames) { // movies die "keine frames haben" benoetigen setPosition, deshalb sind auch meist die ersten paar frames "kaputt"
                if (_frame < 5) {
                    _frame = 5;
                }
                gmMovie.setPosition((float)(_frame-2)/(float)(gmTotalFrames-1)); //setPosition Movies brauchen das setzen des frames davor und dann nextFrame
                gmMovie.nextFrame();
                gmMovie.nextFrame();
                if (gmThreadCounter < 2) { // der erste frame muss ein wenig warten, bis das movie bereit ist
                    ofSleepMillis(TimeToWaitForMovie);
                }
                ofLog(OF_LOG_VERBOSE, "setPosition: " + ofToString(gmMovie.getPosition()) + " _frame: " + ofToString(_frame) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

            } else {
//                if (_frame==0) {
//                    gmMovie.setFrame(0);
//                    gmMovie.update();
//                    if (gmThreadCounter < 2) { // der erste frame muss ein wenig warten, bis das movie bereit ist
//                        ofSleepMillis(TimeToWaitForMovie);
//                    }
////                    while (!gmMovie.isFrameNew()) {
////                        ofLog(OF_LOG_VERBOSE, "grabToImage: waiting for frame to be ready - isFrameNew()" + ofToString(gmMovie.isFrameNew()));
////                    }
//                } else {
//                    gmMovie.setFrame(_frame);
//                    gmMovie.update();
//                    if (gmThreadCounter < 2) { // der erste frame muss ein wenig warten, bis das movie bereit ist
//                        ofSleepMillis(TimeToWaitForMovie);
//                    }
////                    while (!gmMovie.isFrameNew()) {
////                        ofLog(OF_LOG_VERBOSE, "grabToImage: waiting for frame to be ready");
////                    }
//                }
                gmMovie.play();
                gmMovie.setFrame(_frame);
                gmMovie.update();
                gmMovie.stop();
//                ofSleepMillis(TimeToWaitForMovie);
                ofLog(OF_LOG_VERBOSE, "setPosition: " + ofToString(gmMovie.getPosition()) + " _frame: " + ofToString(_frame) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));

            }
            if (grabbedStill[i].gsImage.isAllocated() && !gmCurrAllocating) {
                grabbedStill[i].gsImage.setFromPixels(gmMovie.getPixels());
                grabbedStill[i].gsToBeGrabbed = FALSE;
            } else {
                ofLog(OF_LOG_VERBOSE, "CRASH AVOIDED grabbedStill[i].gsImage.isAllocated() FALSE _______________________________");
            }
        }
        ofLog(OF_LOG_VERBOSE, "after grabToImage: " + ofToString(gmMovie.getPosition()) + " _frame: " + ofToString(_frame) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));
    }

    int numberLoaded(){
        gmNumberLoadedCounter = 0;
        if (isMovieLoaded()) {
            for(int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
                if(!grabbedStill[i].gsToBeUpdated){
                    gmNumberLoadedCounter++;
                }
            }
        }
        return gmNumberLoadedCounter;
    }

    int numberGrabbed(){
        gmNumberGrabbedCounter = 0;
        if (isMovieLoaded()) {
            for(int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
                if(!grabbedStill[i].gsToBeGrabbed){
                    gmNumberGrabbedCounter++;
                }
            }
        }
        return gmNumberGrabbedCounter;
    }

    float percLoaded(){
        int gmNumberLoaded = numberLoaded();
        return (float)gmNumberLoaded/(gmNumberOfStills-1);
    }

    bool allGrabbed(){
        int allGrabbed = 0;
        if (isMovieLoaded()) {

            for(int i=0; i<returnSizeOfGrabbedStillAndLogIfItDiffersFromGmNumberOfStills(); i++)
            {
                if(!grabbedStill[i].gsToBeGrabbed){
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

    string framesToTime(int _frames){

        int frames  =    _frames % gmFrameRate;
        int seconds =   (_frames / gmFrameRate) % 60;
        int minutes =  ((_frames / gmFrameRate) / 60) % 60;
        int hours   = (((_frames / gmFrameRate) / 60) / 60) % 24;
        return (ofToString(hours,2,'0') + ":" + ofToString(minutes,2,'0') + ":" + ofToString(seconds,2,'0') + ":" + ofToString(frames,2,'0'));

    }


    // Thread funcions

    void threadedFunction(){

        if (gmSetupFinished && isMovieLoaded()) { // only start when setup is finished and movie is loaded
            lock();
            do {
//                for (int i = 0; i<gmNumberOfStills; i++) {
//                    if (grabbedStill[i].gsToBeGrabbed) {
//                        gmThreadCounter++;
//                        grabToImage(i, grabbedStill[i].gsFrameNumber);
//                    }
//                }

                for (int i = 0; i<gmOrderNumberVector.size(); i++) { // frames are being updated in the order of their framenumber
                    if (grabbedStill[gmOrderNumberVector.at(i).x].gsToBeGrabbed) {
                        gmThreadCounter++;
//                        ofLog(OF_LOG_VERBOSE, "In Thread Function - gsUpdateOrderNumber:" + ofToString(grabbedStill[gmOrderNumberVector.at(i).x].gsUpdateOrderNumber) + " Frame:" + ofToString(grabbedStill[gmOrderNumberVector.at(i).x].gsFrameNumber) + " gmOrderNumberVector.at(i).x:" + ofToString(gmOrderNumberVector.at(i).x));
                        grabToImage(gmOrderNumberVector.at(i).x, grabbedStill[gmOrderNumberVector.at(i).x].gsFrameNumber);
                    }
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
    ofVideoPlayer gmMovieScrub;

    vector<fakGrabbedMovieStill> grabbedStill;
    vector<ofVec2f> gmOrderNumberVector;

    bool devTurnOffMovieSwitch = FALSE;
    bool isMovieLoaded2 = FALSE;

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

    // used for ofxAvCodec addon
//    ofxAvVideoPlayer gmMovie;

};

#endif // FAKGRABFRAMES_H
*/
