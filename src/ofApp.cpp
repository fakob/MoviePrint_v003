#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    drawNotify = true; // ofxNotify
    showPlaceHolder = false; // added for developing

    gui.setup();

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

    tweenTimelineInOut.initialTime = ofGetElapsedTimef() + ofRandom(0, 300);
    tweenTimelineInOut.duration = ofRandom(600, 1000);
    tweenTimelineInOut.minValue = 1.0;
    tweenTimelineInOut.maxValue = 0.0;
    tweenListInOut.initialTime = ofGetElapsedTimef() + ofRandom(0, 300);
    tweenListInOut.duration = ofRandom(600, 1000);
    tweenListInOut.minValue = 1.0;
    tweenListInOut.maxValue = 0.0;
    tweenBlendStartDropImageCounter.initialTime = ofGetElapsedTimef();
    tweenBlendStartDropImageCounter.duration = 100.0;
    tweenBlendStartDropImageCounter.minValue = 0.0;
    tweenBlendStartDropImageCounter.maxValue = 1.0;

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
//    loadedFile = "movies/FrameTestMovie_4zu3_v001.mov";
    loadedFile = "";
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
//    ofAddListener(scrollBar.sbScrollingGoingOn, this, &ofApp::scrollEvent);

//    scrollBarList.setup(0, ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, footerHeight/2 + bottomMargin, scrollBarWidth, 16, scrollMultiplier, scrollBarMargin);
//    scrollBarList.registerMouseEvents();
//    scrollBarList.registerTouchEvents();
//    ofAddListener(scrollBarList.sbScrollingGoingOn, this, &ofApp::scrollEvent);

//    setGUITimeline();
//    setGUISettings();
//    setGUISettingsMoviePrint();
//    guiSettingsMoviePrint->loadSettings("guiMoviePrintSettings.xml");


    menuMovieInfo.setupMenu(1,0,0,0,0,headerHeight, true, 'T', false);
    menuMovieInfo.registerMouseEvents();
    ofAddListener(menuMovieInfo.mMenuIsBeingOpened, this, &ofApp::menuIsOpened);
    ofAddListener(menuMovieInfo.mMenuIsBeingClosed, this, &ofApp::menuIsClosed);

    menuMoviePrintSettings.setupMenu(5,0,0,0,0,headerHeight, true, 'T', false);
    menuMoviePrintSettings.registerMouseEvents();
    ofAddListener(menuMoviePrintSettings.mMenuIsBeingOpened, this, &ofApp::menuIsOpened);
    ofAddListener(menuMoviePrintSettings.mMenuIsBeingClosed, this, &ofApp::menuIsClosed);

    menuSettings.setupMenu(3,0,0,0,0,headerHeight, true, 'T', false);
    menuSettings.registerMouseEvents();
    ofAddListener(menuSettings.mMenuIsBeingOpened, this, &ofApp::menuIsOpened);
    ofAddListener(menuSettings.mMenuIsBeingClosed, this, &ofApp::menuIsClosed);

    menuHelp.setupMenu(2,0,0,0,0,headerHeight, true, 'T', false);
    menuHelp.registerMouseEvents();
    ofAddListener(menuHelp.mMenuIsBeingOpened, this, &ofApp::menuIsOpened);
    ofAddListener(menuHelp.mMenuIsBeingClosed, this, &ofApp::menuIsClosed);

//    menuTimeline.setupMenu(0,0,0,0,0,footerHeight/2, true, 'B', false);
//    menuTimeline.registerMouseEvents();

//    menuMoveToList.setupMenu(6,0,0,0,0,leftMargin*2, true, 'L', false);
//    menuMoveToList.registerMouseEvents();
//    ofAddListener(menuMoveToList.mMenuIsBeingClicked, this, &ofApp::menuIsClicked);

//    moveInOutTimeline();


//    previousMoviePrintDataSet.clear();
    addToUndo = false;
    undoPosition = 0;
//    addMoviePrintDataSet(undoPosition); // add loaded settings as first undo step

    uiSliderValueHigh = 1000;    // added this temporarily until new timeline slider is in place

    setupFinished = TRUE;
}

//--------------------------------------------------------------
void ofApp::setGUITimeline(){

//    // setup gui
//    guiTimeline = new ofxUICanvas(ofGetWidth()/2-displayGridWidth/2-OFX_UI_GLOBAL_WIDGET_SPACING, ofGetHeight()-(footerHeight/2) - timeSliderHeight/2, ofGetWidth(),footerHeight); //ofxUICanvas(float x, float y, float width, float height)
//    drawPadding = FALSE;
//    guiTimeline->setDrawBack(FALSE);

//	guiTimeline->addWidgetDown(new ofxUIRangeSlider("RSLIDER", 0.0, (float)totalFrames, 0.0, 100.0, displayGridWidth, timeSliderHeight));
//    uiRangeSliderTimeline = (ofxUIRangeSlider *) guiTimeline->getWidget("RSLIDER");
//    uiRangeSliderTimeline->setLabelPrecision(0);
//    uiRangeSliderTimeline->setLabelVisible(FALSE);

//    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE, ofColor(0,0,0,0));
//    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT, ofColor(0,0,0,0));
//    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL, FAK_ORANGE1);
//    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL_HIGHLIGHT, FAK_ORANGE1);

//    uiRangeSliderTimeline->setColorBack(FAK_ORANGE5);

//    uiRangeSliderTimeline->setColorPadded(FAK_ORANGE5);

//    ofAddListener(guiTimeline->newGUIEvent, this, &testApp::guiEvent);

}

//--------------------------------------------------------------
void ofApp::setGUISettingsMoviePrint(){

//	float dim = 16;
//	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING*2;
//    float length = menuWidth-xInit;

//    guiSettingsMoviePrint = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
//    guiSettingsMoviePrint->setFont("HelveticaNeueLTCom-LtCn.ttf");
//    guiSettingsMoviePrint->setWidgetSpacing(10);

//    guiSettingsMoviePrint->addLabelButton("Select Output Folder", false,length-xInit,dim);
//    guiSettingsMoviePrint->addLabel("SelectedOutputFolder", saveMoviePrintPath, OFX_UI_FONT_SMALL);
//    uiLabelOutputFolder = (ofxUILabel *) guiSettingsMoviePrint->getWidget("SelectedOutputFolder");
//    uiLabelOutputFolder->setLabel(cropFrontOfString(saveMoviePrintPath, 40, "..."));

//    guiSettingsMoviePrint->addToggle("Overwrite MoviePrint", &overwriteMoviePrint, dim*1.5, dim);
//    uiToggleOverwriteMoviePrint = (ofxUIToggle *) guiSettingsMoviePrint->getWidget("Overwrite MoviePrint");

//    guiSettingsMoviePrint->addSpacer(length-xInit, 1);
////    guiSettingsMoviePrint->addLabel("SET RASTER", OFX_UI_FONT_MEDIUM);
//	guiSettingsMoviePrint->addIntSlider("PrintColumns", 1, 10, &moviePrintDataSet.printGridColumns, length-xInit,dim);
//	guiSettingsMoviePrint->addIntSlider("PrintRows", 1, 20, &moviePrintDataSet.printGridRows, length-xInit,dim);
//    uiSliderPrintColumns = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintColumns");
//    uiSliderPrintRows = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintRows");
//    guiSettingsMoviePrint->addSpacer(length-xInit, 0);
//   	guiSettingsMoviePrint->addIntSlider("PrintMargin", 0, 30, &moviePrintDataSet.printGridMargin, length-xInit,dim);
//    uiSliderPrintMargin = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintMargin");

//    guiSettingsMoviePrint->addSpacer(length-xInit, 1);
////    guiSettingsMoviePrint->addLabel("SHOW INFO", OFX_UI_FONT_MEDIUM);

//    guiSettingsMoviePrint->addToggle("Display Header", &moviePrintDataSet.printDisplayVideoAudioInfo, dim*1.5, dim);
//    uiToggleHeaderDisplay = (ofxUIToggle *) guiSettingsMoviePrint->getWidget("Display Header");

//    vector<string> names;
//	names.push_back("Display Frames");
//	names.push_back("Display TimeCode");
//	names.push_back("off");
//    uiRadioSetFrameDisplay = guiSettingsMoviePrint->addRadio("RADIO_HORIZONTAL", names, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);

//    guiSettingsMoviePrint->addSpacer(length-xInit, 1);

//    guiSettingsMoviePrint->addToggle("Save also individual frames", &moviePrintDataSet.printSingleFrames, dim*1.5, dim);
//    uiToggleSingleFrames = (ofxUIToggle *) guiSettingsMoviePrint->getWidget("Save also individual frames");

//    guiSettingsMoviePrint->addSpacer(length-xInit, 1);

////    guiSettingsMoviePrint->addLabel("Choose Output Format", OFX_UI_FONT_MEDIUM);
//    vector<string> names3;
//    names3.push_back("png with alpha");
//    names3.push_back("jpg");
////    names3.push_back("gif");
//    guiSettingsMoviePrint->addRadio("Choose Output Format", names3, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
//    uiRadioPrintOutputFormat =(ofxUIRadio *) guiSettingsMoviePrint->getWidget("Choose Output Format");

//    vector<string> names4;
//    names4.push_back("1024px width");
//    names4.push_back("2048px width");
//    names4.push_back("3072px width");
//    names4.push_back("4096px width");
//    guiSettingsMoviePrint->addRadio("MoviePrint Width", names4, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
//    uiRadioPrintOutputWidth =(ofxUIRadio *) guiSettingsMoviePrint->getWidget("MoviePrint Width");


//    guiSettingsMoviePrint->setColorBack(FAK_TRANSPARENT);
//	ofAddListener(guiSettingsMoviePrint->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void ofApp::setGUISettings(){

//    float dim = 16;
//    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING*2;
//    float length = menuWidth-xInit;

//    guiSettings = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
//    guiSettings->setFont("HelveticaNeueLTCom-LtCn.ttf");
//    guiSettings->setWidgetSpacing(10);


//    guiSettings->addLabelButton("Save MoviePrint", false,length-xInit,dim*8);
//    guiSettings->addSpacer(length-xInit, 1);

//    guiSettings->addLabelButton("Refresh", false,length-xInit,dim);

//    guiSettings->addLabelButton("Undo", false,length/2-xInit*1,dim);
//    guiSettings->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
//    guiSettings->addLabelButton("Redo", false,length/2-OFX_UI_GLOBAL_WIDGET_SPACING*3,dim);
//    guiSettings->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

//    uiButtonUndo =(ofxUIButton *) guiSettings->getWidget("Undo");
//    uiButtonRedo =(ofxUIButton *) guiSettings->getWidget("Redo");

//    guiSettings->addLabelButton("Show MoviePrint Preview", false,length-xInit,dim);

//    guiSettings->setColorBack(FAK_TRANSPARENT);
//    ofAddListener(guiSettings->newGUIEvent,this,&testApp::guiEvent);


}

//--------------------------------------------------------------
void ofApp::update(){
    loadedMovie.update();

    tweenTimelineInOut.value = ofxeasing::map_clamp(ofGetElapsedTimef(), tweenTimelineInOut.initialTime, (tweenTimelineInOut.initialTime + tweenTimelineInOut.duration), tweenTimelineInOut.minValue, tweenTimelineInOut.maxValue, &ofxeasing::exp::easeInOut);
    tweenListInOut.value = ofxeasing::map_clamp(ofGetElapsedTimef(), tweenListInOut.initialTime, (tweenListInOut.initialTime + tweenListInOut.duration), tweenListInOut.minValue, tweenListInOut.maxValue, &ofxeasing::exp::easeInOut);
    tweenBlendStartDropImageCounter.value = ofxeasing::map_clamp(ofGetElapsedTimef(), tweenBlendStartDropImageCounter.initialTime, (tweenBlendStartDropImageCounter.initialTime + tweenBlendStartDropImageCounter.duration), tweenBlendStartDropImageCounter.minValue, tweenBlendStartDropImageCounter.maxValue, &ofxeasing::exp::easeInOut);
    tweenTimeDelay.value = ofxeasing::map_clamp(ofGetElapsedTimef(), tweenTimeDelay.initialTime, (tweenTimeDelay.initialTime + tweenTimeDelay.duration), tweenTimeDelay.minValue, tweenTimeDelay.maxValue, &ofxeasing::exp::easeInOut);

    threadIsRunning = loadedMovie.isThreadRunning();

    // set window to minium width !!NOT WORKING
//    if (windowWasResized && !ofGetMousePressed()) {
//        ofSetWindowShape(fmin(ofGetWindowWidth(),1320),ofGetWindowHeight());
//        windowWasResized = false;
//    }

//    loadedMovie.update();

    if (tweenListInOut.value == 0.0 || tweenListInOut.value == 1.0) {
        lockedDueToInteraction = false;
    }

    if (!finishedUpdating) {
        if (!threadIsRunning && !ofGetMousePressed()) {
            timer.setStartTime();
            finishedUpdating = TRUE;
            ofLog(OF_LOG_VERBOSE, "Finished Updating Movie--------------------------------------------");
        }
    }
    if (showPrintScreen) {
        if (finishedPrinting) {
            if (timer.getElapsedSeconds() > 0.5) {
                showPrintScreen = FALSE;
            }
        }
    }
    if (showLoadMovieScreen) {
        if (finishedLoadingMovie) {
            if (timer.getElapsedSeconds() > 0.5) {
                showLoadMovieScreen = FALSE;
            }
        }

    }
    if (showUpdateScreen) {
        if (finishedUpdating) {
            if (timer.getElapsedSeconds() > 0.5) {
                showUpdateScreen = FALSE;
            }
        }
    }
    if (showTimeline) {
        if (finishedTimeline) {
            if (tweenTimelineInOut.value > 0.5) {
                showTimeline = FALSE;
                moveInOutTimeline();
            }
        }
    }

    if (movieIsBeingGrabbed) {
        if (loadedMovie.allGrabbed() || !loadedMovie.isMovieLoaded()) {
            movieIsBeingGrabbed = FALSE;
        }

    }

    // update Loader
    loadValue = ofMap(loadedMovie.percLoaded(), 0, 1.0, 0.0, 1.0, true);

    // update Movie for Scrubbing or InOut manipulations
    if (loadedMovie.gmScrubMovie || updateInOut) {
        loadedMovie.gmMovieScrub.update();
    }

//    handlingEventOverlays();

    if (showMoviePrintPreview){
        if (showListView) {
            writeFboToPreview(fmin(fboToPreview.getWidth()/(float)printGridWidth, fboToPreview.getHeight()/(float)printGridHeight), true);
        } else {
            writeFboToPreview(fmin(fboToPreview.getWidth()/(float)printGridWidth, fboToPreview.getHeight()/(float)printGridHeight), false);
        }
    }

//    // calculate rollout of ofxUI pos, scal
//    guiSettingsMoviePrint->setPosition(menuMoviePrintSettings.getPositionX(), menuMoviePrintSettings.getPositionY()+headerHeight);
//    guiSettingsMoviePrint->setHeight(menuMoviePrintSettings.getSizeH()-headerHeight);

//    guiSettings->setPosition(menuSettings.getPositionX(), menuSettings.getPositionY()+headerHeight);
//    guiSettings->setHeight(menuSettings.getSizeH()-headerHeight);
//    //    guiSettings->setHeight(menuSettings.getSizeH()-headerHeight);

//    guiTimeline->setPosition(leftMargin - OFX_UI_GLOBAL_WIDGET_SPACING, ofGetWindowHeight() - footerHeight/2 +1 - (footerHeight/4) * menuTimeline.getRelSizeH());


    if (loadedMovie.isMovieLoaded()) { // if no movie is loaded or we are in dev mode then only draw rects

        // update while scrubbing
        if (loadedMovie.gmScrubMovie) {

            updateInOut = FALSE;
            updateScrub = TRUE;
            int i = loadedMovie.gmScrubID;

            if (scrubbingJustStarted) {
                scrubbingJustStarted = false;
                scrubDelta = 0.0;
                scrubMouseDelta = 0.0;
                scrubInitialFrame = loadedMovie.grabbedStill[i].gsFrameNumber;
            }

            scrubMouseDelta = (ofGetMouseX() - loadedMovie.grabbedStill[i].gsX - thumbWidth/2);

            if (abs(scrubMouseDelta) >= 0 && (abs(scrubMouseDelta) < thumbWidth/6.0)) {
                scrubMouseDelta = 0;
            }
            if (scrubMouseDelta >= thumbWidth/6.0) {
                scrubMouseDelta = scrubMouseDelta - thumbWidth/6.0;
            } else if (scrubMouseDelta <= -thumbWidth/6.0) {
                scrubMouseDelta = scrubMouseDelta + thumbWidth/6.0;
            }

            ofLog(OF_LOG_VERBOSE, "scrubMouseDelta before:" + ofToString(scrubMouseDelta));

            float expScrubMouseDelta;

            if (scrubMouseDelta < 0) {
                expScrubMouseDelta = exp(fmin(6.0,(abs(scrubMouseDelta)/20.0) - 0)) * -1;
            } else {
                expScrubMouseDelta = exp(fmin(6.0,(scrubMouseDelta/20.0) - 0));
            }

            if(shiftKeyPressed) {
                scrubDelta = scrubDelta + expScrubMouseDelta/10.0;
            } else{
                scrubDelta = scrubDelta + expScrubMouseDelta/100.0;
            }

            if ((scrubDelta + scrubInitialFrame) > loadedMovie.gmTotalFrames-1) {
                scrubDelta = loadedMovie.gmTotalFrames-1 - scrubInitialFrame;
            }
            if ((scrubDelta + scrubInitialFrame) < 0) {
                scrubDelta = 0 - scrubInitialFrame;
            }


            ofLog(OF_LOG_VERBOSE, "scrubMouseDelta after:" + ofToString(expScrubMouseDelta));
            ofLog(OF_LOG_VERBOSE, "scrubDelta:" + ofToString(scrubDelta));
            // new Frame Number is being cropped by the movies first and last frame
            int newFrameNumber = scrubDelta + scrubInitialFrame;
            if (newFrameNumber > loadedMovie.gmTotalFrames-1) {
                newFrameNumber = loadedMovie.gmTotalFrames-1;
            }
            if (newFrameNumber < 0) {
                newFrameNumber = 0;
            }

            if (loadedMovie.gmHasNoFrames) {
                int tempNewFrameNumber = newFrameNumber;
                if (tempNewFrameNumber < 5) {
                    tempNewFrameNumber = 5;
                }
                loadedMovie.gmMovieScrub.setPosition((float)(tempNewFrameNumber - 2)/(float)(loadedMovie.gmTotalFrames-1));
                loadedMovie.gmMovieScrub.nextFrame();
                loadedMovie.gmMovieScrub.nextFrame();
            } else {
                loadedMovie.gmMovieScrub.setFrame(newFrameNumber);
            }
            loadedMovie.grabbedStill[i].gsFrameNumber = newFrameNumber;
        }

//        // update while Slider sends event
//        if (manipulateSlider) {
//            updateScrub = FALSE;
//            updateInOut = TRUE;
//            if (uiRangeSliderTimeline->hitLow) {
//                scrubWindowGridNumber = 0;
//                if (loadedMovie.gmHasNoFrames) {
//                    loadedMovie.gmMovieScrub.setPosition((float)(uiSliderValueLow - 1)/(float)(loadedMovie.gmTotalFrames-1));
//                    loadedMovie.gmMovieScrub.nextFrame();
//                } else {
//                    loadedMovie.gmMovieScrub.setFrame(uiSliderValueLow);
//                }
//                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = uiSliderValueLow;
//            } else if (uiRangeSliderTimeline->hitHigh)
//            {
//                scrubWindowGridNumber = numberOfStills-1;
//                if (loadedMovie.gmHasNoFrames) {
//                    loadedMovie.gmMovieScrub.setPosition((float)(uiSliderValueHigh - 1)/(float)(loadedMovie.gmTotalFrames-1));
//                    loadedMovie.gmMovieScrub.nextFrame();
//                } else {
//                    loadedMovie.gmMovieScrub.setFrame(uiSliderValueHigh);
//                }
//                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = uiSliderValueHigh;
//            } else if (uiRangeSliderTimeline->hitCenter)
//            {
//                scrubWindowGridNumber = numberOfStills/2;
//                if (loadedMovie.gmHasNoFrames) {
//                    loadedMovie.gmMovieScrub.setPosition((float)(((uiSliderValueHigh - uiSliderValueLow)/2 + uiSliderValueLow) - 1)/(float)(loadedMovie.gmTotalFrames-1));
//                    loadedMovie.gmMovieScrub.nextFrame();
//                } else {
//                    loadedMovie.gmMovieScrub.setFrame((uiSliderValueHigh - uiSliderValueLow)/2 + uiSliderValueLow);
//                }
//                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = (uiSliderValueHigh - uiSliderValueLow)/2 + uiSliderValueLow;
//            }
//        }
    }

    if (updateNewPrintGrid == TRUE && !currPrintingList && !ofGetMousePressed()) {
        updateInOut = FALSE;
        updateScrub = FALSE;

        if(tweenTimeDelay.value < 5){
            updateNewPrintGrid = FALSE;
            if (!showListView) {
                loadedMovie.allocateNewNumberOfStills(numberOfStills, thumbWidth, thumbHeight, showPlaceHolder, true);
                updateAllStills();
            }
        }
    }

//    if (droppedList.glUpdateMovieFromList) {
//        printListNotImage = FALSE;
//        showListView = FALSE;
//        finishedLoadingMovie = FALSE;
//        showLoadMovieScreen = TRUE;
//        moveToMovie();
//        counterToUpdate++;
//        if (counterToUpdate > 1) {
//            loadNewMovie(droppedList.glDroppedItem[droppedList.glActiveID].gliFile.path(), TRUE, FALSE, TRUE);
//            droppedList.glUpdateMovieFromList = FALSE;
//            counterToUpdate = 0;
//        }
//    }

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

//    if (scrollBar.sbActive) {
//    //        ofLog(OF_LOG_VERBOSE, "scrollBar Active:" + ofToString(scrollAmountRel) );
//        if (scrollGrid) {
//    //            ofLog(OF_LOG_VERBOSE, "scrollGrid True:" + ofToString(scrollAmountRel) );
//            if (!scrollBar.sbCalculateScrollInertia && !scrollBar.sbScrollBarDrag) {
//                scrollGrid = false;
//            } else {
//            scrollBar.update();
//            scrollAmountRel = scrollBar.getRelativePos();
//    //            ofLog(OF_LOG_VERBOSE, "scrollBarAmount:" + ofToString(scrollAmountRel) );
//            }
//        }
//    } else {
//        scrollAmountRel = 0;

//    }

//    if (scrollBarList.sbActive) {
//    //        ofLog(OF_LOG_VERBOSE, "scrollBarList Active:" + ofToString(scrollListAmountRel) );
//        if (scrollList) {
//    //            ofLog(OF_LOG_VERBOSE, "scrollList True:" + ofToString(scrollListAmountRel) );
//            if (!scrollBarList.sbCalculateScrollInertia && !scrollBarList.sbScrollBarDrag) {
//                scrollList = false;
//            } else {
//            scrollBarList.update();
//            scrollListAmountRel = scrollBarList.getRelativePos();
//    //            ofLog(OF_LOG_VERBOSE, "scrollBarAmount:" + ofToString(scrollListAmountRel) );
//            }
//        }
//    } else {
//        scrollListAmountRel = 0;
//    }

//    if (currPrintingList) {
//        updateScrub = FALSE;
//        updateInOut = FALSE;
//        printListToFile();
//    }

//    if (showPrintScreen && !finishedPrinting) {
//        // to ensure that print screen is showing before printing starts
//        counterToPrint++;
//        if (counterToPrint > 1) {
//            if (printListNotImage) {
//                startListPrinting();
//            } else {
//                startPrinting();
//            }
//            counterToPrint = 0;
//        }
//    }


    // sollte kurze wait schleife fuer das scrubvideoloading sein - leider funktioniert das nicht so ganz - man kann zwar das scrubvideo etwas spaeter loaden, waerenddessen haelt aber trotzdem alles an
    if (loadNewMovieToBeScrubbedBool) {
        if (timer.getElapsedSeconds() > 0.1) {
            loadedMovie.loadNewMovieToBeScrubbed(loadedMovie.gmMovie.getMoviePath());
            loadNewMovieToBeScrubbedBool = FALSE;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){


    // gui
    gui.begin();
//    bool* opened;
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiSetCond_Always);
    ImGui::Begin("Another Window");

    if(ImGui::Button("Select Output Folder")) {
        string movieFileName = loadedMovie.gmMovie.getMoviePath();
        movieFileName = loadedFilePath.getFileName(movieFileName, TRUE) + "_MoviePrint";

        string formatExtension;
        if (moviePrintDataSet.printFormat == OF_IMAGE_FORMAT_JPEG) {
            formatExtension = "jpg";
        } else {
            formatExtension = "png";
        }

        ofFileDialogResult saveFileResult = ofSystemSaveDialog(movieFileName + "." + formatExtension, "Select a Folder");
        if (saveFileResult.bSuccess){
            vector<string> tempVectorString = ofSplitString(saveFileResult.getPath(), "/");
            tempVectorString.pop_back();
            saveMoviePrintPath = ofJoinString(tempVectorString, "/") + "/";
            ofLogVerbose("User selected saveMoviePrintPath: "  + ofToString(saveMoviePrintPath));
        } else {
            ofLogVerbose("User hit cancel");
        }
    }
    ImGui::Text("Path: %s", &saveMoviePrintPath);
    ImGui::Checkbox("Overwrite MoviePrint", &overwriteMoviePrint);
    ImGui::Separator();
    ImGui::SliderInt("PrintColumns", &moviePrintDataSet.printGridColumns, 1,10);
    ImGui::SliderInt("PrintRows", &moviePrintDataSet.printGridRows, 1,20);
    ImGui::SliderInt("PrintMargin", &moviePrintDataSet.printGridMargin, 0,30);
    ImGui::Separator();
    ImGui::Checkbox("Display Header", &moviePrintDataSet.printDisplayVideoAudioInfo);
    ImGui::RadioButton("Display Frames", &moviePrintDataSet.printDisplayTimecodeFramesOff, 0);
    ImGui::RadioButton("Display TimeCode", &moviePrintDataSet.printDisplayTimecodeFramesOff, 1);
    ImGui::RadioButton("off", &moviePrintDataSet.printDisplayTimecodeFramesOff, 2);
    ImGui::Separator();
    ImGui::Checkbox("Save also individual frames", &moviePrintDataSet.printSingleFrames);
    ImGui::Separator();


    // has problem as it does not load moviePrintDataSet.printFormat -> see int and enum for solution
    static int tempR = 0;
    if (ImGui::RadioButton("png with alpha", &tempR, 0)) {
        moviePrintDataSet.printFormat = OF_IMAGE_FORMAT_PNG;
    }
    if (ImGui::RadioButton("jpg", &tempR, 1)) {
        moviePrintDataSet.printFormat = OF_IMAGE_FORMAT_JPEG;
    }
    ImGui::Separator();
    static int tempS = 0;
    if (ImGui::RadioButton("1024px width", &tempS, 0)) {
        moviePrintDataSet.printSizeWidth = 1024;
    }
    if (ImGui::RadioButton("2048px width", &tempS, 1)) {
        moviePrintDataSet.printSizeWidth = 2048;
    }
    if (ImGui::RadioButton("3072px width", &tempS, 2)) {
        moviePrintDataSet.printSizeWidth = 3072;
    }
    if (ImGui::RadioButton("4096px width", &tempS, 3)) {
        moviePrintDataSet.printSizeWidth = 4096;
    }

    ImGui::End();


    gui.end();

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

    drawUI(1, FALSE);

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
void ofApp::drawUI(int _scaleFactor, bool _hideInPrint){
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255);

    // overlay von MoviePrintPreview
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    ofSetColor(0, 0, 0, tweenMoviePrintPreview.value * 100);
    ofDrawRectangle(ofGetWindowWidth()/2, (ofGetWindowHeight()-headerHeight-footerHeight/2)/2 + headerHeight, ofGetWindowWidth(), (ofGetWindowHeight()-headerHeight-footerHeight/2));
    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
    ofPopStyle();

//    if (droppedFiles.size() > 1) { // draw
//        if (loadedMovie.isMovieLoaded) {
//            if (showListView == FALSE) {
//                menuMoveToList.setPosition(0, 0);
//                menuMoveToList.setSize(leftMargin*2, ofGetWindowHeight());
//                menuMoveToList.drawMenu();
//            }
//        }
//    }

    layoutHeaderImage.draw(0, 0, ofGetWindowWidth() * _scaleFactor, layoutHeaderImage.getHeight() * _scaleFactor);

    ofPushStyle();
    for(int i=0; i<gridColumns; i++)
    {
        switch (i%5) {
            case 0:
                ofSetColor(FAK_ORANGE1);
                break;
            case 1:
                ofSetColor(FAK_ORANGE2);
                break;
            case 2:
                ofSetColor(FAK_ORANGE3);
                break;
            case 3:
                ofSetColor(FAK_ORANGE4);
                break;
            case 4:
                ofSetColor(FAK_ORANGE5);
                break;
            default:
                ofSetColor(255, 255, 255, 255);
                break;
        }
        // draw orange stripes
        ofDrawRectangle((0 + displayGridMargin + (thumbWidth+displayGridMargin) * i) * _scaleFactor, (0 + headerHeight*0.7) * _scaleFactor, thumbWidth * _scaleFactor, headerHeight* 0.2 * _scaleFactor);
    }
    ofPopStyle();

    float tempY = 0;
    int tempXPos = 0;
    int menuHeightInRows = 4;
    int originalThumbHeight = 144;

    tempXPos = gridColumns/2;
    menuSettings.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuSettings.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*2 - displayGridMargin);
    menuSettings.drawMenu();

    if (loadedMovie.isMovieLoaded()) {
        tempXPos = 0;
        menuMovieInfo.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
        menuMovieInfo.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
        menuMovieInfo.drawMenu();
//        drawMovieInfo((leftMargin + displayGridMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, headerHeight + displayGridMargin*3, menuMovieInfo.getRelSizeH());
//        fontStashHelveticaMedium.draw(loadedMovie.gmMIFileName, 10, (int)(leftMargin + 33 * _scaleFactor), (int)((0 + headerHeight*0.6) * _scaleFactor));
    }

    tempXPos = gridColumns-2;
    menuHelp.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuHelp.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuHelp.drawMenu();
    ofSetColor(255, 255, 255, menuHelp.getRelSizeH() * 255);
    helpMenuImage.draw((leftMargin + displayGridMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, headerHeight + displayGridMargin*1, helpMenuImage.getWidth(), helpMenuImage.getHeight() * menuHelp.getRelSizeH());


    tempXPos = gridColumns-1;

    ofSetColor(255, 255, 255, 255);
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    float tempXPosLerp = ofLerp((ofGetWindowWidth()-scrollBarWidth)/2.0, ((leftMargin + (thumbWidth + displayGridMargin)*tempXPos)/2.0) * _scaleFactor, menuMoviePrintSettings.getRelSizeH());
//        fboToPreview.draw(tempXPosLerp, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows/2.0 - displayGridMargin, tweenMoviePrintPreview.update() * fboToPreviewWidth, tweenMoviePrintPreview.update() * fboToPreviewHeight);
    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner

    menuMoviePrintSettings.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuMoviePrintSettings.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuMoviePrintSettings.drawMenu();


    ofSetColor(255, 255, 255, 255);

    if (!showListView) {
        if (loadedMovie.isMovieLoaded()) {
            menuTimeline.setPosition(0, ofGetWindowHeight());
            menuTimeline.setSize(ofGetWindowWidth(), footerHeight/2);
            menuTimeline.drawMenu();
        }
    }

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::toggleMoviePrintPreview(){
    showMoviePrintPreview = !showMoviePrintPreview;
//    if (showMoviePrintPreview) {
//        tweenMoviePrintPreview.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,300,0);
//    } else {
//        tweenMoviePrintPreview.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,300,0);
//    }
}

//--------------------------------------------------------------
void ofApp::setVisibilityMoviePrintPreview(bool _visibility){
    if (showMoviePrintPreview != _visibility) {
        toggleMoviePrintPreview();
    }
}

//--------------------------------------------------------------
void ofApp::menuIsOpened(int &e){
//    ofLog(OF_LOG_VERBOSE, "menuIsOpened:" + ofToString(e));
    if (e == 5) {
        setVisibilityMoviePrintPreview(true);
    }
    if (e == 3) {
        setVisibilityMoviePrintPreview(false);
    }
    allMenusAreClosed = false;
    allMenusAreClosedOnce = 1;
}

//--------------------------------------------------------------
void ofApp::menuIsClosed(int &e){
//    ofLog(OF_LOG_VERBOSE, "menuIsClosed:" + ofToString(e));
    if (e == 5) {
        setVisibilityMoviePrintPreview(false);
    }
    if (!menuMovieInfo.getMenuActivated() && !menuMoviePrintSettings.getMenuActivated() && !menuHelp.getMenuActivated() && !menuSettings.getMenuActivated()) {
        allMenusAreClosed = true;
        allMenusAreClosedOnce = 0;
//        ofLog(OF_LOG_VERBOSE, "allMenusAreClosed:" + ofToString(allMenusAreClosedOnce));
    }
}

//--------------------------------------------------------------
void ofApp::menuIsClicked(int &e){
//    ofLog(OF_LOG_VERBOSE, "menuIsClicked:" + ofToString(e));
    if (e == 6) {
        if (droppedFiles.size() > 1) {
            if (loadedMovie.isMovieLoaded()) {
                showListView = !showListView;
                if (showListView == TRUE) {
                    moveToList();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::writeFboToPreview(float _scaleFactor, bool _showPlaceHolder){
    fboToPreview.begin();
    ofClear(255,255,255, 0);
    drawMoviePrintPreview(_scaleFactor, _showPlaceHolder);
    fboToPreview.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLogVerbose(__FUNCTION__);
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
    ofLog(OF_LOG_VERBOSE, "moveToMovie()-------------------------------------------");
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

    if (tweenListInOut.value != 0.0) {
    //        tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),0);
        tweenListInOut.initialTime = ofGetElapsedTimef();
        tweenListInOut.duration = ofRandom(600, 1000);
        tweenListInOut.minValue = 1.0;
        tweenListInOut.maxValue = 0.0;
    }

}

//--------------------------------------------------------------
void ofApp::moveInOutTimeline(){

    if (showTimeline) {
        tweenTimelineInOut.initialTime = ofGetElapsedTimef();
        tweenTimelineInOut.duration = ofRandom(600, 1000);
        tweenTimelineInOut.minValue = 1.0;
        tweenTimelineInOut.maxValue = 0.0;

    } else {
        tweenTimelineInOut.initialTime = ofGetElapsedTimef();
        tweenTimelineInOut.duration = ofRandom(600, 1000);
        tweenTimelineInOut.minValue = 0.0;
        tweenTimelineInOut.maxValue = 1.0;
    }

    if (showTimeline){
        loadedMovie.setAllLimitsLower(ofGetHeight() - footerHeight);
//        droppedList.setAllLimitsLower(ofGetHeight() - footerHeight);
    } else {
        loadedMovie.setAllLimitsLower(ofGetHeight());
//        droppedList.setAllLimitsLower(ofGetHeight());
    }

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

    calculateNewPrintSize();

    updateNewPrintGrid = true;
//    tweenTimeDelay.setParameters(1,easinglinear,ofxTween::easeInOut,255.0,0.0,500,0);
    tweenTimeDelay.initialTime = ofGetElapsedTimef();
    tweenTimeDelay.duration = 500.0;
    tweenTimeDelay.minValue = 255.0;
    tweenTimeDelay.maxValue = 0.0;

    updateDisplayGrid();

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

//--------------------------------------------------------------
void ofApp::drawMoviePrintPreview(float _scaleFactor, bool _showPlaceHolder){
    ofPushStyle();
    ofPushMatrix();
    _scaleFactor = _scaleFactor * 0.95;
    float tempX = (fboToPreviewWidth - _scaleFactor * printGridWidth) / 2;
    float tempY = (fboToPreviewHeight - _scaleFactor * printGridHeight) / 2;
    ofTranslate(tempX, tempY);
    ofSetColor(255);
    backgroundImagePreview.draw(0, 0, _scaleFactor * printGridWidth, _scaleFactor * printGridHeight);
    loadedMovie.drawMoviePrint(0, 0, moviePrintDataSet.printGridColumns, moviePrintDataSet.printGridRows, moviePrintDataSet.printGridMargin, _scaleFactor, 1, _showPlaceHolder, printHeaderHeight, moviePrintDataSet.printDisplayVideoAudioInfo, true);

    // drawing frame
    float tempFrameWidth = 3;
    ofSetColor(220);
    ofDrawRectangle(0, 0 - tempFrameWidth, _scaleFactor * printGridWidth + tempFrameWidth, tempFrameWidth);
    ofDrawRectangle(0 - tempFrameWidth, 0 - tempFrameWidth, tempFrameWidth, _scaleFactor * printGridHeight + tempFrameWidth);
    ofDrawRectangle(0 + _scaleFactor * printGridWidth, 0, tempFrameWidth, _scaleFactor * printGridHeight + tempFrameWidth);
    ofDrawRectangle(0 - tempFrameWidth, 0 + _scaleFactor * printGridHeight, _scaleFactor * printGridWidth + tempFrameWidth, tempFrameWidth);
    // drawing shadow
    ofSetColor(0,200);
    ofDrawRectangle(0 + _scaleFactor * printGridWidth + tempFrameWidth, 0, tempFrameWidth, _scaleFactor * printGridHeight + tempFrameWidth*2);
    ofDrawRectangle(0, 0 + _scaleFactor * printGridHeight + tempFrameWidth, _scaleFactor * printGridWidth + tempFrameWidth, tempFrameWidth);
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::calculateNewPrintSize(){
    printGridWidth = (thumbWidth + moviePrintDataSet.printGridMargin) * moviePrintDataSet.printGridColumns + moviePrintDataSet.printGridMargin;
    if (moviePrintDataSet.printDisplayVideoAudioInfo) {
        printGridHeight = (thumbHeight + moviePrintDataSet.printGridMargin) * moviePrintDataSet.printGridRows + moviePrintDataSet.printGridMargin + printHeaderHeight;
    } else {
        printGridHeight = (thumbHeight + moviePrintDataSet.printGridMargin) * moviePrintDataSet.printGridRows + moviePrintDataSet.printGridMargin;
    }
    //    ofLog(OF_LOG_VERBOSE, "printGridSize" + ofToString(printGridWidth) + "x" + ofToString(printGridHeight));
}

//--------------------------------------------------------------
void ofApp::updateDisplayGrid(){

    gridColumns = fmax(1, (int)(ofGetWindowWidth() - leftMargin - scrollBarWidth + displayGridMargin) / (thumbWidth + displayGridMargin));

    displayGridWidth = (gridColumns * (thumbWidth + displayGridMargin) - displayGridMargin);
    gridRows = ceil(numberOfStills/(float)gridColumns);
    displayGridHeight = (gridRows * (thumbHeight + displayGridMargin)) - displayGridMargin;
    //    ofLog(OF_LOG_VERBOSE, "displayGridMargin: " + ofToString(displayGridMargin));
    //    ofLog(OF_LOG_VERBOSE, "displayGridHeight: " + ofToString(thumbHeight));
    //    ofLog(OF_LOG_VERBOSE, "gridAreaHeight: " + ofToString(displayGridHeight));

//    updateAllLimits();
//    updateTheScrollBar();
//    updateTheListScrollBar();
//    updateTimeline();

    ofxNotify() << "updateDisplayGrid - Total Number of Stills: " + ofToString(numberOfStills);

    //    ofLog(OF_LOG_VERBOSE, "displayGridWidth: " + ofToString(displayGridWidth));
    //    ofLog(OF_LOG_VERBOSE, "displayGridHeight: " + ofToString(displayGridHeight));

}

//--------------------------------------------------------------
void ofApp::moveToList(){
    ofLog(OF_LOG_VERBOSE, "moveToList()-------------------------------------------");
    lockedDueToInteraction = true;
    closeAllMenus();

    showListView = TRUE;

//    scrollBar.unregisterMouseEvents();
//    scrollBar.unregisterTouchEvents();

//    scrollBarList.registerMouseEvents();
//    scrollBarList.registerTouchEvents();

    loadedMovie.stop(TRUE);
    loadedMovie.disableMouseEvents();

//    guiTimeline->setVisible(FALSE);
//    menuTimeline.unRegisterMouseEvents();
//    menuTimeline.setMenuInactive();

//    droppedList.enableMouseEvents();
    printListNotImage = TRUE;

    if (tweenListInOut.value != 1.0) {
    //        tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,ofRandom(600, 1000),0);
        tweenListInOut.initialTime = ofGetElapsedTimef();
        tweenListInOut.duration = ofRandom(600, 1000);
        tweenListInOut.minValue = 0.0;
        tweenListInOut.maxValue = 1.0;
    }
}

//--------------------------------------------------------------
void ofApp::handlingEventOverlays(){
    // check if one of the topMenus is active and in this case turn of the mouseEvents for the thumbs
//    if (menuMovieInfo.getMenuActivated() || menuMoviePrintSettings.getMenuActivated() || menuHelp.getMenuActivated() || menuSettings.getMenuActivated()) {
//        if (loadedMovie.getMouseEventsEnabled()) {
//            loadedMovie.disableMouseEvents();
//        }
//        if (showListView) {
//            droppedList.disableMouseEvents(droppedFiles.size());
//        }
//    } else {
//        if (allMenusAreClosedOnce == 0) {
//            if (!lockedDueToInteraction && !lockedDueToPrinting) {
//                allMenusAreClosedOnce++;
//                if (!loadedMovie.getMouseEventsEnabled() && !showListView) {
//                    loadedMovie.enableMouseEvents();
//                }
//                if (showListView) {
//                    droppedList.enableMouseEvents();
//                }
//            }
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::closeAllMenus(){
//    menuMoviePrintSettings.closeMenuManually();
//    menuSettings.closeMenuManually();
//    menuMovieInfo.closeMenuManually();
//    menuHelp.closeMenuManually();
//    ofLog(OF_LOG_VERBOSE, "closeMenuManually-------------------------------------------");
}

//--------------------------------------------------------------
void ofApp::inactivateAllMenus(){
//    menuMoviePrintSettings.setMenuInactive();
//    menuSettings.setMenuInactive();
//    menuMovieInfo.setMenuInactive();
//    menuHelp.setMenuInactive();
//    ofLog(OF_LOG_VERBOSE, "inactivateAllMenus-------------------------------------------");
}

//--------------------------------------------------------------
void ofApp::activateAllMenus(){
//    menuMoviePrintSettings.setMenuActive();
//    menuSettings.setMenuActive();
//    menuMovieInfo.setMenuActive();
//    menuHelp.setMenuActive();
//    ofLog(OF_LOG_VERBOSE, "activateAllMenus-------------------------------------------");
}

//--------------------------------------------------------------
void ofApp::setResourcePath(){
    CFURLRef appUrl = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef appPath = CFURLCopyFileSystemPath(appUrl, kCFURLPOSIXPathStyle);
    char appPathChars[PATH_MAX];
    CFStringGetCString(appPath, appPathChars, FILENAME_MAX, kCFStringEncodingASCII );
    appPathStr = string(appPathChars);
    appPathUpStr = appPathStr.substr(0,appPathStr.find_last_of("\\/"));
    CFRelease(appUrl);
    CFRelease(appPath);

    CFURLRef resourceUrl = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
    CFStringRef resourcePath = CFURLCopyFileSystemPath(resourceUrl, kCFURLPOSIXPathStyle);
    char resourcePathChars[PATH_MAX];
    CFStringGetCString(resourcePath, resourcePathChars, FILENAME_MAX, kCFStringEncodingASCII );
    string resourcePathStr = string(resourcePathChars);
    CFRelease(resourcePath);
    CFRelease(resourceUrl);

    string newPath = appPathStr + "/" + resourcePathStr + "/"; // the absolute path to the resources folder
    ofLog(OF_LOG_VERBOSE, "appPathStr: " + ofToString(appPathStr) );
    ofLog(OF_LOG_VERBOSE, "appPathUpStr: " + ofToString(appPathUpStr) );
    ofLog(OF_LOG_VERBOSE, "newDataPath: " + ofToString(newPath) );
    ofSetDataPathRoot(newPath); // tell OF to look for resources here
}

//--------------------------------------------------------------
bool ofApp::fequal(float _x, float _y, float _t){
    float diff = abs(_x-_y);
    if (diff < _t) {
        return TRUE;
    } else  {
        return FALSE;
    }
}
