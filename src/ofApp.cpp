#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    drawNotify = true; // ofxNotify
    showPlaceHolder = false; // added for developing

//    setResourcePath();

    setupFinished = FALSE;
    updateNewPrintGrid = FALSE;
    showListView = FALSE;
    lockedDueToInteraction = false;
    lockedDueToPrinting = false;
    updateMovieFromList = FALSE;
    printListNotImage = FALSE;
    currPrintingList = FALSE;
    movieLoading = FALSE;
    movieIsBeingGrabbed = FALSE;
    movieProperlyLoaded = TRUE;
    scrollGrid = FALSE;
    scrollList = FALSE;
    windowResizedOnce = 0;
    updateOnce = 0;
    ofBackground(30, 30, 30);
    ofSetFrameRate(60);
    loadValue = 0;
    scrubWindowW = 640;
    scrubWindowGridNumber = 0;
    scrubDelta = 0;
    scrubbingJustStarted = true;
    totalFrames = 0;
    itemToPrint = 0;
    loadNewMovieToBeScrubbedBool = FALSE;
    windowWasResized = false;
    allMenusAreClosed = true;
    allMenusAreClosedOnce = 0;
    switchFromLogoToDropZone = false;

    updateGridTimeArrayToMoviePrintDataSet = false;

    // UI Values
    leftMargin = 5;
    rightMargin = 5;
    topMargin = 5;
    bottomMargin = 5;
    headerHeight = 40;
    footerHeight = 40;
    displayGridMargin = 5;
    loaderBarHeight = 20;
    timeSliderHeight = 10;
    gridColumns = 3;
    gridRows = 3;
    printNumberOfThumbs = 9;
    menuWidth = 255;
    listWidth = 1320;

    overwriteMoviePrint = false;

    moviePrintDataSet.printGridColumns = 3;
    moviePrintDataSet.printGridRows = 3;
    moviePrintDataSet.printGridMargin = 5;
    moviePrintDataSet.printDisplayVideoAudioInfo = true;
    moviePrintDataSet.printDisplayTimecodeFramesOff = 0;
    moviePrintDataSet.printSingleFrames = false;
    moviePrintDataSet.printFormat = OF_IMAGE_FORMAT_PNG;
    moviePrintDataSet.printSizeWidth = 1024;


    threadIsRunning = FALSE;

    maxUndoSteps = 10 + 1;

    showPrintScreen = FALSE;
    finishedPrinting = TRUE;
    writeMoviePrint = false;

    showLoadMovieScreen = FALSE;
    finishedLoadingMovie = TRUE;
    showUpdateScreen = FALSE;
    finishedUpdating = TRUE;
    showTimeline = FALSE;
    finishedTimeline = TRUE;

    updateMovieFromDrop = FALSE;

    rollOverClicked = FALSE;

    showFBO = FALSE;

    gridRows = 6;
    displayGridSetWithColumnsAndRows = false;
    printGridSetWithColumnsAndRows = true;
    showMoviePrintPreview = false;

    thumbWidth = 256;
    thumbHeight = 144;
    printHeaderHeight = thumbHeight/4;
    scrollBarWidth = 12;
    scrollBarMargin = 2;
    scrollAmountRel = 0;
    scrollListAmountRel = 0;


    fboToPreviewWidth = 1320 - leftMargin - rightMargin - thumbWidth*2 - displayGridMargin*2;
    fboToPreviewHeight = 660 - headerHeight - footerHeight;

    fboToPreview.allocate(fboToPreviewWidth, fboToPreviewHeight, GL_RGBA);

    counterToUpdate = 0;
    counterToLoad = 0;
    counterToPrint = 0;

    showMenu = FALSE;
    showTopMenu = false;
//    tweenTimelineInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
//    tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
//    tweenBlendStartDropImageCounter.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,100,0);


    startImage.load("images/MoviePrint_StartBildschirm_v002_00000.png");
    dropZoneImage.load("images/MoviePrint_DropZone_v002_00000.png");
    backgroundImage.load("images/MoviePrint_Background_v002_00000.jpg");
    printImage.load("images/MoviePrint_Print_v002_00000.png");
    printListImage.load("images/MoviePrint_PrintList_v002_00000.png");
    loadMovieImage.load("images/MoviePrint_Loading_v002_00000.png");
    updatingImage.load("images/MoviePrint_Updating_v002_00000.png");
    inPointImage.load("images/MoviePrint_InPoint_v001_00000.png");
    outPointImage.load("images/MoviePrint_OutPoint_v001_00000.png");
    layoutHeaderImage.load("images/MoviePrint_Layout_Header_v001_00000.png");
    helpMenuImage.load("images/HelpMenu_v001.png");
    backgroundImagePreview.load("images/MoviePrint_PreviewBackground_v001_00000.png");

//    fontStashHelveticaLight.setup("HelveticaNeueLTCom-Lt.ttf");
//    fontStashHelveticaMedium.setup("HelveticaNeueLTCom-Md.ttf");


    // load standard movie
    loadedFile = "Nothing";
    saveMoviePrintPath = appPathUpStr + "/MoviePrints/";

    loadedMovie.gmUpperLimitY = headerHeight;
    loadedMovie.gmLowerLimitY = ofGetHeight() - footerHeight;
    loadedMovie.gmLeftLimitX = leftMargin;
    loadedMovie.gmRightLimitX = ofGetWidth() - rightMargin;
    calculateNewPrintGrid();
    loadedMovie.setup(loadedFile, numberOfStills, thumbWidth, thumbHeight, showPlaceHolder);
    totalFrames = 100;
    loadedMovie.allocateNewNumberOfStills(numberOfStills, thumbWidth, thumbHeight, showPlaceHolder, false);

    updateInOut = FALSE;
    manipulateSlider = FALSE;
    updateDropped = FALSE;

//    scrollMultiplier = 50.0;
//    scrollBar.setup(0, ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, footerHeight/2 + bottomMargin, scrollBarWidth, 16, scrollMultiplier, scrollBarMargin);

//    scrollBar.setScrollHeight((float)displayGridHeight);
//    scrollBar.registerMouseEvents();
//    scrollBar.registerTouchEvents();
//    ofAddListener(scrollBar.sbScrollingGoingOn, this, &testApp::scrollEvent);

//    scrollBarList.setup(0, ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, footerHeight/2 + bottomMargin, scrollBarWidth, 16, scrollMultiplier, scrollBarMargin);
//    scrollBarList.registerMouseEvents();
//    scrollBarList.registerTouchEvents();
//    ofAddListener(scrollBarList.sbScrollingGoingOn, this, &testApp::scrollEvent);

//    setGUITimeline();
//    setGUISettings();
//    setGUISettingsMoviePrint();
//    guiSettingsMoviePrint->loadSettings("guiMoviePrintSettings.xml");


//    menuMovieInfo.setupMenu(1,0,0,0,0,headerHeight, true, 'T', false);
//    menuMovieInfo.registerMouseEvents();
//    ofAddListener(menuMovieInfo.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
//    ofAddListener(menuMovieInfo.mMenuIsBeingClosed, this, &testApp::menuIsClosed);

//    menuMoviePrintSettings.setupMenu(5,0,0,0,0,headerHeight, true, 'T', false);
//    menuMoviePrintSettings.registerMouseEvents();
//    ofAddListener(menuMoviePrintSettings.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
//    ofAddListener(menuMoviePrintSettings.mMenuIsBeingClosed, this, &testApp::menuIsClosed);

//    menuSettings.setupMenu(3,0,0,0,0,headerHeight, true, 'T', false);
//    menuSettings.registerMouseEvents();
//    ofAddListener(menuSettings.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
//    ofAddListener(menuSettings.mMenuIsBeingClosed, this, &testApp::menuIsClosed);

//    menuHelp.setupMenu(2,0,0,0,0,headerHeight, true, 'T', false);
//    menuHelp.registerMouseEvents();
//    ofAddListener(menuHelp.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
//    ofAddListener(menuHelp.mMenuIsBeingClosed, this, &testApp::menuIsClosed);

//    menuTimeline.setupMenu(0,0,0,0,0,footerHeight/2, true, 'B', false);
//    menuTimeline.registerMouseEvents();

//    menuMoveToList.setupMenu(6,0,0,0,0,leftMargin*2, true, 'L', false);
//    menuMoveToList.registerMouseEvents();
//    ofAddListener(menuMoveToList.mMenuIsBeingClicked, this, &testApp::menuIsClicked);

//    moveInOutTimeline();


//    previousMoviePrintDataSet.clear();
    addToUndo = false;
    undoPosition = 0;
//    addMoviePrintDataSet(undoPosition); // add loaded settings as first undo step

    setupFinished = TRUE;
}

//--------------------------------------------------------------
void ofApp::update(){
//    loadedMovie.update();

    if (updateMovieFromDrop) {
//        printListNotImage = FALSE;
//        showListView = FALSE;
        finishedLoadingMovie = FALSE;
        showLoadMovieScreen = TRUE;
//        counterToUpdate++;
//        if (counterToUpdate > 1) {
            ofLog(OF_LOG_VERBOSE, "droppedFiles[0].path()" + ofToString(droppedFiles[0].path()));

            loadNewMovie(droppedFiles[0].path(), TRUE, FALSE, TRUE);
            if (loadedMovie.isMovieLoaded()) {
                moveToMovie();
            }
            updateMovieFromDrop = FALSE;
//            counterToUpdate = 0;
//        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){


//    if (!(tweenListInOut.update() == 0.0)) { // stop drawing when position is at showMovieView

//        drawList(scrollListAmountRel);
//        scrollBarList.draw();

//    }

//    if (!(tweenListInOut.update() == 1.0)) { // stop drawing when position is at showListView

        if (!loadedMovie.isMovieLoaded()) { // if no movie is loaded
            if (!showListView) { // if no List View
//                guiTimeline->setVisible(FALSE);
//                drawStartScreen();
            }

        } else {

            // draw all frames
            drawDisplayGrid(1, FALSE, FALSE, scrollAmountRel, showPlaceHolder);

            // draw the In and Out Point Manipulations
            if (updateInOut) {

                ofPushStyle();

                ofEnableAlphaBlending();
//                ofSetColor(0,(int)(tweenFading.update()/255)*155);
                ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
//                ofSetColor(255,255,255,(int)tweenFading.update());

//                loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
                loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + listWidth * 1, ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
//                loadedMovie.drawStillUI(scrubWindowGridNumber, ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (tweenFading.update()/255));

//                ofSetColor(255, 255, 255, (int)(tweenFading.update()/255)*255);

//                if (uiRangeSliderTimeline->hitLow) {
//                    inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-inPointImage.getHeight()/2);
//                }
//                if (uiRangeSliderTimeline->hitHigh) {
//                    outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-outPointImage.getHeight()/2);
//                }
//                if (uiRangeSliderTimeline->hitCenter) {
//                    inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-inPointImage.getHeight()/2);
//                    outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-outPointImage.getHeight()/2);
//                }

//                if(tweenFading.update() < 5){
//                    updateInOut = FALSE;
//                    manipulateSlider = FALSE;
//                }

                ofSetColor(255);

                ofPopStyle();

            }

//            // draw the scrubbed video
//            if (updateScrub) {
//                drawScrubScreen(1.0);
//            }

//            scrollBar.draw();

        }

//    }

//    drawUI(1, FALSE);

//    if(showLoadMovieScreen){
//        drawLoadMovieScreen();
//    } else if (showPrintScreen) {
//        drawPrintScreen();
//    } else {
//        if(showUpdateScreen && loadedMovie.isMovieLoaded()){
//            drawUpdateScreen();
//        }
//    }

    ofxNotify::draw(drawNotify);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    ofLog(OF_LOG_VERBOSE, "Drag event------------------------------------------------");
    ofxNotify() << "Drag event";

//    if (!lockedDueToInteraction && !lockedDueToPrinting) {

//        if (setupFinished) {

            if( dragInfo.files.size() > 0 ){

                loadNewMovie("", FALSE, TRUE, FALSE);

                ofxNotify() << "dragInfo.files[0]" + ofToString(dragInfo.files[0]);
                ofxNotify() << "dragInfo.files[0]" + ofToString(ofToDataPath(dragInfo.files[0]));



//                droppedList.disableMouseEvents(droppedFiles.size());
                droppedFiles.clear();
                for (int i=0; i<dragInfo.files.size(); i++) {
                    ofFile testFile(dragInfo.files[i]);
                    if (!testFile.isDirectory()) {
                        if (checkExtension(ofToString(testFile.getExtension()))){
//                            droppedFiles.push_back(dragInfo.files[i]);
                            droppedFiles.push_back(testFile);
                        }
                    }
                }

//                droppedList.setup(droppedFiles);
//                updateTheListScrollBar();

                if(droppedFiles.size() > 1){
//                    moveToList();
                } else if(droppedFiles.size() > 0){
                    ofLog(OF_LOG_VERBOSE, "Start LoadingMovie------------------------------------------------");
                    ofxNotify() << "updateMovieFromDrop = TRUE";
                    updateMovieFromDrop = TRUE;

                }

            }
//        }
//    } else {
//        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
//    }
}

//--------------------------------------------------------------
void ofApp::moveToMovie(){
//    ofLog(OF_LOG_VERBOSE, "moveToMovie()-------------------------------------------");
//    lockedDueToInteraction = true;
//    closeAllMenus();

//    showListView = FALSE;

//    droppedList.disableMouseEvents(droppedFiles.size());
//    loadedMovie.enableMouseEvents();

//    scrollBarList.unregisterMouseEvents();
//    scrollBarList.unregisterTouchEvents();
//    scrollBar.registerMouseEvents();
//    scrollBar.registerTouchEvents();

//    guiTimeline->setVisible(TRUE);
//    menuTimeline.registerMouseEvents();
//    menuTimeline.setMenuActive();

//    printListNotImage = FALSE;
//    updateInOut = FALSE;
//    manipulateSlider = FALSE;

//    if (tweenListInOut.update() != 0.0) {
//        tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),0);
//    }

}

//--------------------------------------------------------------
void ofApp::loadNewMovie(string _newMoviePath, bool _wholeRange, bool _loadInBackground, bool _loadScrubMovie){

    movieLoading = TRUE;
    if (!_loadInBackground) {
        moveToMovie();
    }

    loadedMovie.stop(TRUE);

    ofxNotify() << "Movie has started to load";
    loadedMovie.loadNewMovieToBeGrabbed(_newMoviePath, numberOfStills, showPlaceHolder, !_loadInBackground);
    calculateNewPrintGrid();
    if (loadedMovie.gmTotalFrames <=1) {
        movieProperlyLoaded = FALSE;
        ofxNotify() << "Movie could not be properly loaded";
        totalFrames = numberOfStills;
    } else {
        movieProperlyLoaded = TRUE;
        totalFrames = loadedMovie.gmTotalFrames;
    }
    ofLog(OF_LOG_VERBOSE, "_newMoviePath" + ofToString(_newMoviePath) );
    ofLog(OF_LOG_VERBOSE, "totalFrames" + ofToString(totalFrames) );

//    updateTimeSlider(_wholeRange);
    updateGridTimeArrayWithAutomaticInterval();
    updateAllStills();

//    if (_loadScrubMovie) {
//        loadNewMovieToBeScrubbedBool = TRUE;
//    }

//    if (!_loadInBackground) {
//        showListView = FALSE;
//        droppedList.glUpdateMovieFromList = FALSE;
//    }
    movieLoading = FALSE;
    if (movieProperlyLoaded){
        ofxNotify() << "Movie has finished to load";
    } else{
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
    }
//    updateTheScrollBar();

    ofLog(OF_LOG_VERBOSE, "scrollAmountRel: " + ofToString(scrollAmountRel));

//    timer.setStartTime();
    finishedLoadingMovie = TRUE;

    ofLog(OF_LOG_VERBOSE, "_loadInBackground: " + ofToString(_loadInBackground));
    ofLog(OF_LOG_VERBOSE, "Finished Loading Movie--------------------------------------------");

//    // reset all undo steps and save initial DataSet
//    previousMoviePrintDataSet.clear();
//    addToUndo = false;
//    undoPosition = 0;
//    addMoviePrintDataSet(0);

//    if (!loadedMovie.isMovieLoaded()) {
//        guiTimeline->setVisible(FALSE);
//        scrollBar.unregisterMouseEvents();
//        loadedMovie.disableMouseEvents();
//    }


//    stringMovieInfo.clear();
//    stringMovieInfo.push_back("Format: ");
//    stringMovieInfo.push_back("Format/String: ");
//    stringMovieInfo.push_back("FileSize: ");
//    stringMovieInfo.push_back("Duration: ");
//    stringMovieInfo.push_back("FrameCount: ");
//    stringMovieInfo.push_back("Size: ");
//    stringMovieInfo.push_back("DisplayAspectRatio: ");
//    stringMovieInfo.push_back("FrameRate: ");
//    stringMovieInfo.push_back("Codec: ");
//    stringMovieInfo.push_back("Codec/Info: ");
//    stringMovieInfo.push_back("BitRate: ");
//    stringMovieInfo.push_back("PixelAspectRatio: ");
//    stringMovieInfo.push_back("DisplayAspectRatio: ");
//    stringMovieInfo.push_back("FrameRate_Mode: ");
//    stringMovieInfo.push_back("ColorSpace: ");
//    stringMovieInfo.push_back("ChromaSubsampling: ");
//    stringMovieInfo.push_back("BitDepth: ");
//    stringMovieInfo.push_back("Interlacement: ");
//    stringMovieInfo.push_back("AudioCodec: ");
//    stringMovieInfo.push_back("Channels: ");
//    stringMovieInfo.push_back("SamplingRate: ");

//    stringMovieData.clear();
//    stringMovieData.push_back(loadedMovie.gmMIFormat);
//    stringMovieData.push_back(loadedMovie.gmMIFormatString);
//    stringMovieData.push_back(loadedMovie.gmMIFileSizeString);
//    stringMovieData.push_back(loadedMovie.gmMIDurationString1);
//    stringMovieData.push_back(loadedMovie.gmMIFrameCount);
//    stringMovieData.push_back(loadedMovie.gmMIWidth);
//    stringMovieData.push_back(loadedMovie.gmMIDisplayAspectRatioString);
//    stringMovieData.push_back(loadedMovie.gmMIFrameRateString);
//    stringMovieData.push_back(loadedMovie.gmMIVFormat);
//    stringMovieData.push_back(loadedMovie.gmMIFormatInfo);
//    stringMovieData.push_back(loadedMovie.gmMIBitRate);
//    stringMovieData.push_back(loadedMovie.gmMIPixelAspectRatio);
//    stringMovieData.push_back(loadedMovie.gmMIDisplayAspectRatio);
//    stringMovieData.push_back(loadedMovie.gmMIFrameRate_ModeString);
//    stringMovieData.push_back(loadedMovie.gmMIColorSpace);
//    stringMovieData.push_back(loadedMovie.gmMIChromaSubsampling);
//    stringMovieData.push_back(loadedMovie.gmMIBitDepthString);
//    stringMovieData.push_back(loadedMovie.gmMIInterlacementString);
//    stringMovieData.push_back(loadedMovie.gmMIAFormat);
//    stringMovieData.push_back(loadedMovie.gmMIAChannelsString);
//    stringMovieData.push_back(loadedMovie.gmMIASamplingRate);

}

//--------------------------------------------------------------
bool ofApp::checkExtension(string _tempExtension){
    bool tempReturn = FALSE;
    int length = _tempExtension.length();
    for(int i=0; i < length; ++i)
    {
        _tempExtension[i] = tolower(_tempExtension[i]);
    }

    if (_tempExtension == "mp4" ||
        _tempExtension == "mov" ||
        _tempExtension == "m4v" ||
        _tempExtension == "qt" ||
        _tempExtension == "wmv" ||
        _tempExtension == "avi" ||
        _tempExtension == "pdf" ||
//        _tempExtension == "jpg" ||
//        _tempExtension == "jpe" ||
//        _tempExtension == "jpeg" ||
//        _tempExtension == "gif" ||
//        _tempExtension == "png" ||
//        _tempExtension == "tif" ||
//        _tempExtension == "tiff" ||
//        _tempExtension == "bmp" ||
        _tempExtension == "3gp" ||
        _tempExtension == "3gp2" ||
        _tempExtension == "3g2" ||
        _tempExtension == "3gpp" ||
        _tempExtension == "mkv" ||
        _tempExtension == "dv" ||
//        _tempExtension == "flv" ||
//        _tempExtension == "swf" ||
//        _tempExtension == "vob" ||
        _tempExtension == "divx" ||
//        _tempExtension == "mxf" ||
        _tempExtension == "m1v" ||
        _tempExtension == "mpeg" ||
        _tempExtension == "mpeg1" ||
        _tempExtension == "mpeg2" ||
        _tempExtension == "mpeg4" ||
        _tempExtension == "mpg" ||
        _tempExtension == "mpv" ||
        _tempExtension == "mpv2" ||
        _tempExtension == "yuv" ||
        _tempExtension == "m2v")
    {
        tempReturn = TRUE;
    }

    return tempReturn;
}

//--------------------------------------------------------------
void ofApp::calculateNewPrintGrid(){


    float gridRatio;
    if (loadedMovie.gmMovie.isLoaded()) { // if no movie is loaded yet, then take a 16 by 9 and square pixel ratio
        scrubWindowH = scrubWindowW * (1/loadedMovie.gmImageRatio) * loadedMovie.gmPixelRatio;
    } else {
        scrubWindowH = scrubWindowW * 0.5625 * 1;
    }
    loadedMovie.stop(TRUE);
    if (isnan(loadedMovie.gmImageRatio) || !loadedMovie.gmMovie.isLoaded()) {
        gridRatio = 0.5625;
    } else {
        gridRatio = 1.0/loadedMovie.gmImageRatio;
    }
    while (loadedMovie.isThreadRunning()) {
    }

    thumbHeight = thumbWidth*gridRatio;


    if (displayGridSetWithColumnsAndRows) {
        numberOfStills = moviePrintDataSet.printGridColumns*moviePrintDataSet.printGridRows;
    } else {
        numberOfStills = printNumberOfThumbs;
        moviePrintDataSet.printGridRows = ceil(numberOfStills/(float)moviePrintDataSet.printGridColumns);

    }

    if (!(moviePrintDataSet.gridTimeArray.empty())){
        moviePrintDataSet.gridTimeArray.clear();
    }
    moviePrintDataSet.gridTimeArray.resize(numberOfStills,0);
    if (moviePrintDataSet.gridTimeArray.empty()){
        ofLog(OF_LOG_VERBOSE, "Error: memory could not be allocated" );
    } else {
        for (int i=0; i<numberOfStills; i++) {
            moviePrintDataSet.gridTimeArray[i] = i;
        }
    }

    updateGridTimeArrayWithAutomaticInterval();

    loadedMovie.gmThumbWidth = thumbWidth;
    loadedMovie.gmThumbHeight = thumbHeight;

//    calculateNewPrintSize();

//    updateNewPrintGrid = true;
//    tweenTimeDelay.setParameters(1,easinglinear,ofxTween::easeInOut,255.0,0.0,500,0);

//    updateDisplayGrid();

    ofxNotify() << "New Grid is Calculated - Total Number of Stills: " + ofToString(numberOfStills);

}

//--------------------------------------------------------------
void ofApp::updateGridTimeArrayWithAutomaticInterval(){

    if (uiSliderValueLow < 0) {
        uiSliderValueLow = 0;
    }
    if (uiSliderValueHigh > (totalFrames-1)) {
        uiSliderValueHigh = (totalFrames-1);
    }

    for (int i=0; i<numberOfStills; i++) {
        if (numberOfStills == 1) {
            moviePrintDataSet.gridTimeArray[i] = ofMap(0.5, 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);

        } else {
            moviePrintDataSet.gridTimeArray[i] = ofMap(float(i)/(numberOfStills - 1), 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateAllStills(){
    ofLog(OF_LOG_VERBOSE, "Start Updating------------------------------------------------");
    finishedUpdating = FALSE;
    showUpdateScreen = TRUE;

    loadedMovie.updateAllFrameNumbers(&moviePrintDataSet.gridTimeArray);

//    if (updateGridTimeArrayToMoviePrintDataSet) {
//        addGridTimeArrayToMoviePrintDataSet();
//    }

    movieIsBeingGrabbed = TRUE;
    loadedMovie.start();

    ofxNotify() << "Thread is started - " + ofToString(numberOfStills) + " Stills are being updated";
}

//--------------------------------------------------------------
void ofApp::drawDisplayGrid(float _scaleFactor, bool _hideInPNG, bool _isBeingPrinted, float _scrollAmountRel, bool _showPlaceHolder){
    ofPushMatrix();
    ofPushStyle();
    float _scrollAmount = 0;
//    if (scrollBar.sbActive) {
//        _scrollAmount = ((displayGridHeight - (ofGetWindowHeight() - headerHeight - topMargin - bottomMargin)) * -1) * _scrollAmountRel;
//    }
//    if (isnan(_scrollAmount)) {
//        _scrollAmount = 0;
//    }
//    float tempX = (leftMargin + listWidth * tweenListInOut.update()) * _scaleFactor;
    float tempX = (leftMargin + listWidth * 0) * _scaleFactor;
    float tempY = (_scrollAmount + headerHeight + topMargin)  * _scaleFactor;
//    ofLog(OF_LOG_VERBOSE, "tempX:"+ ofToString(tempX) +  " tempY:"+ ofToString(tempY) +  "_scrollAmount:"+ ofToString(_scrollAmount));
    loadedMovie.drawGridOfStills(tempX, tempY, gridColumns, displayGridMargin, _scrollAmount, _scaleFactor, 1, _isBeingPrinted, TRUE, superKeyPressed, shiftKeyPressed, _showPlaceHolder);
    ofPopStyle();
    ofPopMatrix();
}
