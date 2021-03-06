#ifndef FAKGRABFRAMES_H
#define FAKGRABFRAMES_H

#include <string>
#include <iostream>
#include "ofMain.h"
#include "fakgrabbedframe.h"
#include "ofxNotify.h"

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
        gfsSetupFinished = false;
        bool loadingWasSuccessful = false;
        loadingWasSuccessful = loadNewMovieToBeGrabbed(vfMovieName);
        if (loadingWasSuccessful) {
            loadNewMovieToBeScrubbed(vfMovieName);
            allocateNewNumberOfFrames(_numberOfFrames);
        }
        gfsSetupFinished = true;
        isFrameAccurate = true; //false uses CV_CAP_PROP_POS_MSEC and jumps to milliseconds, but it is not faster only really unaccurate apparently
    }

    bool checkIfSpecialCharacters(string _x) {
        if (_x.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_/ .") != std::string::npos)
        {
            return true;
        } else {
            return false;
        }
    }

    bool loadNewMovieToBeGrabbed(string vfMovieName){
        stop(FALSE);

        ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function" + ofToString(vfMovieName));
        ofFile fileToRead(vfMovieName);
        gfsMoviePath = fileToRead.getAbsolutePath();

        if (checkIfSpecialCharacters(gfsMoviePath)) {
            ofxNotify() << "Sorry, MoviePrint can currently not read files where the filename or path contains special characters";
        }

        if (fileToRead.doesFileExist(gfsMoviePath)) {
            ofLog(OF_LOG_VERBOSE, "_____________________________________ File exists " + ofToString(gfsMoviePath));
            movieFile.open(gfsMoviePath);
             ofLog(OF_LOG_VERBOSE, "fileToRead.getAbsolutePath(): " + ofToString(fileToRead.getAbsolutePath()));
            // movieFile.read(matOrig);

            if(movieFile.isOpened()){
                // get some meta data about frame.
                gfsFrameRate = float(movieFile.get(CV_CAP_PROP_FPS));
                gfsTotalFrames = int(movieFile.get(CV_CAP_PROP_FRAME_COUNT));
                gfsFrameHeight = int(movieFile.get(CV_CAP_PROP_FRAME_HEIGHT));
                gfsFrameWidth = int(movieFile.get(CV_CAP_PROP_FRAME_WIDTH));
                //                double posFrames = movieFile.get(CV_CAP_PROP_POS_FRAMES);
                //                double posMsec = movieFile.get(CV_CAP_PROP_POS_MSEC);
                gfsImageRatio = gfsFrameWidth/(float)gfsFrameHeight;
                gfsPixelRatio = 1.0;
                gfsFrameMS = 1000 / gfsFrameRate;

                gfsMIFilePath = fileToRead.path();
                gfsMIFilePathOhne = extractMoviePathWithoutFilename(gfsMIFilePath);

                gfsMIFileName = fileToRead.getBaseName();
                gfsMIFileExtension = fileToRead.getExtension();
                gfsMIFileNameAndExtension = gfsMIFileName + "." + gfsMIFileExtension;


    //getting CV_CAP_PROP_FOURCC does not work yet
    //            int ex = static_cast<int>(movieFile.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
    //            char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
    //            ofLog(OF_LOG_VERBOSE, "EXT[]: " + ofToString(EXT));

    //            printf("fourcc=%d\n", f); // outputs fourcc=6
    //            char fourcc[5];
    //            fourCCStringFromCode((int)f, fourcc);
    //            printf("fourcc=%s\n", fourcc);

                double f = movieFile.get(CV_CAP_PROP_FOURCC);
                gfsMIFormat = ofToString(f);
    //            ofLog(OF_LOG_VERBOSE, "CV_CAP_PROP_FOURCC: " + ofToString(f));

                // Seek to the end of the video.
                movieFile.set(CV_CAP_PROP_POS_AVI_RATIO, 1);
                // Get video length (because we're at the end).
                gfsDurationMS = movieFile.get(CV_CAP_PROP_POS_MSEC);

                gfsMIFormatString = "";
                gfsMIFileSizeString = ofToString(fileToRead.getSize());
                gfsMIDuration = ofToString(gfsDurationMS/1000);
                gfsMIDurationCalculated = ofToString((float)gfsTotalFrames/gfsFrameRate) + " sec";
                gfsMIFrameCount = ofToString(gfsTotalFrames);
                gfsMIWidth = ofToString(gfsFrameWidth) + " x " + ofToString(gfsFrameHeight);
                gfsMIDisplayAspectRatioString = ofToString((float)gfsFrameWidth/(float)gfsFrameHeight);
                gfsMIFrameRateString = ofToString(gfsFrameRate) + " fps";
                gfsMIVFormat = "";
                gfsMIFormatInfo = "";
                gfsMIBitRate = "";
                gfsMIPixelAspectRatio = "";
                gfsMIDisplayAspectRatio = "";
                gfsMIFrameRate_ModeString = "";
                gfsMIColorSpace = "";
                gfsMIChromaSubsampling = "";
                gfsMIBitDepthString = "";
                gfsMIInterlacementString = "";
                gfsMIAFormat = "";
                gfsMIAChannelsString = "";
                gfsMIASamplingRate = "";

            }

            ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function" + ofToString(vfMovieName));

            if (isMovieLoaded()) {
                ofLog(OF_LOG_VERBOSE, "Width: " + ofToString(gfsFrameWidth) + " Height: " + ofToString(gfsFrameHeight) + " ImageRatio:" + ofToString(gfsImageRatio) + " PixelRatio:" + ofToString(gfsPixelRatio)  + " Framerate:" + ofToString(gfsFrameRate) + " totalFrames:" + ofToString(gfsTotalFrames));
                ofLog(OF_LOG_VERBOSE, "Movie loaded");
                if ((gfsFrameWidth == 0) || (gfsFrameHeight == 0)) {
                    ofLog(OF_LOG_VERBOSE, "Movie has no width or height, will load empty movie to unload movie");
                    movieFile.open(""); // load empty to unload movie
                    return false;
                }
            } else {
                ofLog(OF_LOG_VERBOSE, "Movie not loaded");
                return false;
            }
            return isMovieLoaded();
        } else {
            ofLog(OF_LOG_VERBOSE, "_____________________________________ File does not exist " + ofToString(gfsMoviePath));
            return false;
        }

    }

    string extractMoviePathWithoutFilename(string _gfsMIFilePath){
        vector<string> tempVectorString = ofSplitString(_gfsMIFilePath, "/");
        tempVectorString.pop_back();
        return ofJoinString(tempVectorString, "/") + "/";
    }

//    void fourCCStringFromCode(int code, char fourCC[5]) {
//        for (int i = 0; i < 4; i++) {
//            fourCC[3 - i] = code >> (i * 8);
//        }
//        fourCC[4] = '\0';
//    }

    bool loadNewMovieToBeScrubbed(string vfMovieName){
//        stop(FALSE);

        ofLog(OF_LOG_VERBOSE, "_____________________________________ Scrub start loadMovie function" + ofToString(vfMovieName));
        ofFile fileToRead(vfMovieName);
        gfsMoviePath = fileToRead.getAbsolutePath();
        movieFileScrub.open(gfsMoviePath);

//        while (!isScrubMovieLoaded()) {
//            ofLog(OF_LOG_VERBOSE, "_____________________________________ Scrub waiting for movie to load - " + ofToString(gfsMoviePath));
//        }

        ofLog(OF_LOG_VERBOSE, "_____________________________________ Scrub end loadMovie function" + ofToString(vfMovieName));

        if (isScrubMovieLoaded()) {
            ofLog(OF_LOG_VERBOSE, "Scrub Movie loaded");
        } else {
            ofLog(OF_LOG_VERBOSE, "Scrub Movie not loaded");
        }
        return isScrubMovieLoaded();
    }

    void allocateNewNumberOfFrames(int _numberOfFrames){

        gfsNumberOfFrames = _numberOfFrames;

        if (isMovieLoaded()) {
            gfsCurrAllocating = true;

            grabbedFrame.clear();
            grabbedFrame.resize(gfsNumberOfFrames);

            for(int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames(); i++)
            {
                grabbedFrame[i].gfID = i;
                grabbedFrame[i].gfWidth = gfsFrameWidth;
                grabbedFrame[i].gfHeight = gfsFrameHeight;
                grabbedFrame[i].gfImage.setUseTexture(false); // can also not use texture yet - later: grabbedFrame[i].gfTexture.loadData(grabbedFrame[i].gfImage);
                grabbedFrame[i].gfImage.allocate(grabbedFrame[i].gfWidth, grabbedFrame[i].gfHeight, OF_IMAGE_COLOR);
                grabbedFrame[i].gfTexture.allocate(grabbedFrame[i].gfWidth, grabbedFrame[i].gfHeight,GL_RGB);
//                grabbedFrame[i].gfToBeUpdated = TRUE;
                grabbedFrame[i].gfToBeGrabbed = TRUE;
                grabbedFrame[i].gfFrameNumber = ofMap(i, 0, (returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames() - 1), 0, (gfsTotalFrames - 1), TRUE);
//                ofLog(OF_LOG_VERBOSE, "grabbedFrame[i].gfFrameNumber: " + ofToString(grabbedFrame[i].gfFrameNumber) + " gfsTotalFrames: " + ofToString(gfsTotalFrames));
            }
            ofLog(OF_LOG_VERBOSE, "Allocations worked");
        } else{
            ofLog(OF_LOG_VERBOSE, "Movie not loaded");
        }
        gfsCurrAllocating = false;
    }

    string getMoviePath(){
        return gfsMoviePath;
    }

    void update(){
        if (isMovieLoaded()) {
//            ofLog(OF_LOG_VERBOSE, "____________gfsMovie.update() ");
//            for (int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames(); i++) {
//                grabbedFrame[i].gfTexture.loadScreenData(0,0,200,200);
//                grabbedFrame[i].gfImage.update();
//            }
//            for (int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames(); i++) {
//                ofLog(OF_LOG_VERBOSE, "grabbedFrame[i].gfImage.isUsingTexture():" + ofToString(grabbedFrame[i].gfImage.isUsingTexture()));
//                if (grabbedFrame[i].gfToBeUpdated && !grabbedFrame[i].gfImage.isUsingTexture()) { // load textures in proper size
//                    if (!grabbedFrame[i].gfToBeGrabbed ) {
//                        //                    if (gfsCalcResizeSwitch) {
//                        //                        grabbedStill[i].gsImage.resize(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
//                        //                    }
//                        grabbedFrame[i].gfTexture.loadData(grabbedFrame[i].gfImage);
////                        grabbedFrame[i].gfTexture.loadScreenData(0,0,400,400);
//                        grabbedFrame[i].gfImage.update();
//                        grabbedFrame[i].gfToBeUpdated = FALSE;
//                        ofLog(OF_LOG_VERBOSE, "Texture updated:" + ofToString(i));
//                    }
//                }
//            }
        }
        if (isScrubMovieLoaded()) {

        }
    }


    int returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames(){
        int tempNumberOfFrames = gfsNumberOfFrames;
//        ofLog(OF_LOG_VERBOSE, "returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames() gfsNumberOfFrames:" + ofToString(gfsNumberOfFrames));
        if (gfsNumberOfFrames != grabbedFrame.size()) {
            ofLog(OF_LOG_VERBOSE, "XXX___ Size of gfsNumberOfFrames and grabbedFrame.size() differs -> grabbedFrame.size() is used");
            tempNumberOfFrames = grabbedFrame.size();
        }
        return tempNumberOfFrames;
    }

    void setAllToBeGrabbedAndToBeUpdated(){
        ofLog(OF_LOG_VERBOSE, "____________setAllToBeGrabbedAndToBeUpdated()");
        for (int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames(); i++) {
            setOneToBeGrabbedAndToBeUpdated(i, FALSE);
        }
    }

    void setOneToBeGrabbedAndToBeUpdated(int _i, bool _manuallyManipulated){
        ofLog(OF_LOG_VERBOSE, "____________setOneToBeGrabbedAndToBeUpdated() "+ ofToString(isMovieLoaded()));
        if (isMovieLoaded()) {
            grabbedFrame[_i].gfToBeUpdated = TRUE;
            grabbedFrame[_i].gfToBeGrabbed = TRUE;
            grabbedFrame[_i].gfManipulated = _manuallyManipulated;
        }
    }

    bool isMovieLoaded(){
//        ofLog(OF_LOG_VERBOSE, "____________isMovieLoaded() "+ ofToString(isMovieLoaded()));
        return movieFile.isOpened();
    }

    bool isScrubMovieLoaded(){
        //        ofLog(OF_LOG_VERBOSE, "____________isMovieLoaded() "+ ofToString(isMovieLoaded()));
        return movieFileScrub.isOpened();
    }

    void play(){
        if (isMovieLoaded()) {
//            ofLog(OF_LOG_VERBOSE, "____________gfsMovie.play() ");
//            gfsMovie.play();
        }
    }

    int getWidth(){
        return gfsFrameWidth;
    }

    int getHeight(){
        return gfsFrameHeight;
    }

    float getImageRatio(){
        return gfsImageRatio;
    }

    float getPixelRatio(){
        return gfsPixelRatio;
    }

    int getTotalFrames(){
        return gfsTotalFrames;
    }

    float getFrameRate(){
        return gfsFrameRate;
    }

    void grabNextFrame(int i, bool _useThreads){
        setOneToBeGrabbedAndToBeUpdated(i, TRUE);
        grabbedFrame[i].gfFrameNumber = grabbedFrame[i].gfFrameNumber + 1;
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
        grabbedFrame[i].gfFrameNumber = grabbedFrame[i].gfFrameNumber - 1;
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
        grabbedFrame[i].gfFrameNumber = _frame;
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

    int setMoviePosition(bool _scrub, int _frame, bool _isFrameAccurate){
        _frame = min((gfsTotalFrames-1), max(_frame, 0));
        if (_scrub) {
//            ofLog(OF_LOG_VERBOSE, "setScrubMoviePosition: " + ofToString(_frame));
            if (_isFrameAccurate) {
                movieFileScrub.set(CV_CAP_PROP_POS_FRAMES, (double)_frame);
            } else {
                movieFileScrub.set(CV_CAP_PROP_POS_MSEC, (double)(_frame * gfsFrameMS));
            }
            movieFileScrub.read(matOrigScrub);
        } else {
//            ofLog(OF_LOG_VERBOSE, "setMoviePosition: " + ofToString(_frame));
            if (_isFrameAccurate) {
                movieFile.set(CV_CAP_PROP_POS_FRAMES, (double)_frame);
            } else {
                movieFile.set(CV_CAP_PROP_POS_MSEC, (double)(_frame * gfsFrameMS));
            }
            movieFile.read(matOrig);
        }
        return _frame;
    }

    void setScrub(int _frame){
//        ofLog(OF_LOG_VERBOSE, "setScrub: " + ofToString(_frame));
        setMoviePosition(true, _frame, isFrameAccurate);
        cv::cvtColor(matOrigScrub, matOrigScrub, cv::COLOR_BGR2RGB);
        ofxCv::copy(matOrigScrub, scrubImg);
        scrubImg.update();
    }

    void setFrameScrub(int _i, int _frame){
//        ofLog(OF_LOG_VERBOSE, "setFrameScrub: " + ofToString(_frame));
        grabbedFrame[_i].gfFrameNumber = setMoviePosition(true, _frame, isFrameAccurate);
        cv::cvtColor(matOrigScrub, matOrigScrub, cv::COLOR_BGR2RGB);
        ofxCv::copy(matOrigScrub, scrubImg);
        scrubImg.update();
    }

    void grabToImage(int _i, int _frame, bool _inThread){

        string str = "";
        if (_inThread) {
            str = "In Thread function - ";
        }
//        ofLog(OF_LOG_VERBOSE, str + "1 _frame: " + ofToString(_frame) + " getPosition: " + ofToString(gfsMovie.getPosition()) + " getCurrentFrame: " + ofToString(gfsMovie.getCurrentFrame()));

        if (isMovieLoaded()) {
            grabbedFrame[_i].gfFrameNumber = setMoviePosition(false, _frame, isFrameAccurate);

            if (grabbedFrame[_i].gfImage.isAllocated() && !gfsCurrAllocating) {
                cv::cvtColor(matOrig, matOrig, cv::COLOR_BGR2RGB);
                ofxCv::copy(matOrig, grabbedFrame[_i].gfImage);
                grabbedFrame[_i].gfImage.update();
                grabbedFrame[_i].gfToBeGrabbed = FALSE;
//                ofLog(OF_LOG_VERBOSE, str + "Frame " + ofToString(i) + " saved");
            } else {
//                ofLog(OF_LOG_VERBOSE, str + "CRASH AVOIDED grabbedFrame[_i].gfImage.isAllocated() FALSE _______________________________");
            }
        }
//        ofLog(OF_LOG_VERBOSE, str + "2 _frame: " + ofToString(_frame) + " getPosition: " + ofToString(gfsMovie.getPosition()) + " getCurrentFrame: " + ofToString(gfsMovie.getCurrentFrame()));
    }

    bool allGrabbed(){
        int allGrabbed = 0;
        if (isMovieLoaded()) {

            for(int i=0; i<returnSizeOfgrabbedFrameAndLogIfItDiffersFromGfsNumberOfFrames(); i++)
            {
                if(!grabbedFrame[i].gfToBeGrabbed){
                    allGrabbed++;
                }
            }
        }
        if (allGrabbed == gfsNumberOfFrames){
            return TRUE;
        } else {
            return FALSE;
        }

    }

    void grabToImageAll(){
        if (gfsSetupFinished && isMovieLoaded()) { // only start when setup is finished and movie is loaded
            do {
                for (int i = 0; i<grabbedFrame.size(); i++) { // frames are being updated in the order of their framenumber
                    if (grabbedFrame[i].gfToBeGrabbed) {
                        ofLog(OF_LOG_VERBOSE, "In NoneThread Function - gsUpdateOrderNumber:" + ofToString(grabbedFrame[i].gfUpdateOrderNumber) + " Frame:" + ofToString(grabbedFrame[i].gfFrameNumber));
                        grabToImage(i, grabbedFrame[i].gfFrameNumber, false);
                    }
                }
            } while (!allGrabbed());
        }
    }

    // Thread funcions

    void threadedFunction(){

        if (gfsSetupFinished && isMovieLoaded()) { // only start when setup is finished and movie is loaded
            lock();
            do {
                for (int i = 0; i<grabbedFrame.size(); i++) {
                    if (grabbedFrame[i].gfToBeGrabbed) {
                        gfsThreadCounter++;
//                        ofLog(OF_LOG_VERBOSE, "In Thread Function - i:" + ofToString(i) + " Frame:" + ofToString(grabbedFrame[i].gfFrameNumber));
                        grabToImage(i, grabbedFrame[i].gfFrameNumber, true);                    }
                }
            } while (!allGrabbed());
            unlock();
        }
        stop(TRUE);
        ofLog(OF_LOG_VERBOSE, "Closing Thread Function-----------------------------------------------" );

    }

    void start(){

        startThread(true);   // blocking, verbose
        gfsThreadCounter = 0;

    }

    void stop(bool rightNow){

        if (!rightNow) {
            waitForThread();
        }
        ofLog(OF_LOG_VERBOSE, "Stoping Thread Function-----------------------------------------------" );
        stopThread();

    }

    //properties

//    ofVideoPlayer gfsMovie;

//    vector<grabbedFrameStruct> grabbedFrame;
    vector<fakGrabbedFrame> grabbedFrame;
    vector<ofVec2f> gfsOrderNumberVector;

    int gfsFrameWidth;
    int gfsFrameHeight;
    bool gfsCurrAllocating;
    bool gfsHasNoFrames;
    float gfsFrameRate;
    int gfsTotalFrames;
    float gfsImageRatio;
    float gfsPixelRatio;
    int gfsNumberOfFrames;
    bool gfsSetupFinished;
    int gfsThreadCounter;
    string gfsMoviePath;

    cv::VideoCapture movieFile;
    cv::VideoCapture movieFileScrub;
    cv::Mat matOrig;
    cv::Mat matOrigScrub;

    ofImage scrubImg;

    string gfsMIFileName, gfsMIFileExtension, gfsMIFormat, gfsMIFormatString, gfsMIFileSizeString, gfsMIDurationCalculated, gfsMIDuration, gfsMIFrameCount, gfsMIWidth, gfsMIHeight, gfsMIDisplayAspectRatioString, gfsMIFrameRateString;
    string gfsMIVFormat, gfsMIFormatInfo, gfsMIBitRate, gfsMIPixelAspectRatio, gfsMIDisplayAspectRatio, gfsMIFrameRate_ModeString, gfsMIColorSpace, gfsMIChromaSubsampling, gfsMIBitDepthString, gfsMIInterlacementString;
    string gfsMIAFormat, gfsMIAChannelsString, gfsMIASamplingRate, gfsMIFilePath, gfsMIFilePathOhne, gfsMIFileNameAndExtension;

    double gfsDurationMS;
    double gfsFrameMS;
    bool isFrameAccurate;
};


#endif // FAKGRABFRAMES_H
