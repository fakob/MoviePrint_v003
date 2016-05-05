//
//  fakScrollBar.h
//  fakMoviePrint
//
//  Created by fakob on 13/05/15.
//
//

#ifndef FAKSCROLLBAR_H
#define FAKSCROLLBAR_H

//#include "ofxMultiTouchPad.h"

class fakScrollBar {

public:

    fakScrollBar(){}


    // Functions

    void setup(int _sbID, int _windowWidth, int _windowHeight, int _headerHeight, int _footerHeight, int _sbScrollAreaWidth, float _sbLoose, float _scrollMultiplier, int _sbScrollBarMargin){
        sbFirstClick = false;
        sbFirstClickMouseY = 0;
        sbScrollAreaX = _windowWidth -  _sbScrollAreaWidth;
        sbScrollAreaY = _headerHeight;
        sbScrollAreaWidth = _sbScrollAreaWidth;
        sbScrollAreaHeight = _windowHeight - _headerHeight - _footerHeight;
        sbScrollBarWidth = _sbScrollAreaWidth - _sbScrollBarMargin*2;
        sbScrollBarHeight = sbScrollAreaHeight/2;
        sbScrollBarX = _windowWidth -  _sbScrollAreaWidth + _sbScrollBarMargin;
        sbScrollBarY = sbScrollAreaY + sbScrollAreaHeight/2 - sbScrollBarHeight/2;
        sbScrollBarYNew = sbScrollBarY;
        sbScrollBarYMin = sbScrollAreaY;
        sbScrollBarYMax = sbScrollAreaY + sbScrollAreaHeight - sbScrollBarHeight;
        sbDecelarationConstant = _sbLoose;
        setToTop();
        sbScrollBarDrag = false;
        sbOvershootHeight = 40;
        sbMouseIsScrolling = false;
        sbCalculateScrollInertia = false;
        scrollMultiplier = _scrollMultiplier;
        sbFirstNoTouchMouseEvent = true;
        sbScrollBarMargin = _sbScrollBarMargin;

        // should not be lower then 1000ms as the magic mouse sends still after having stopped scrolling
        // as the magic mouse sends shaky values it is difficult to control it
        sbBounceBackTimeMs = 1000;

        sbReturnToBaseConstant = 0.5;
        sbBounceDecelerationConstant = 10.0;
        sbMaxVelocity = 10.0;

        sbNumberOfTouches = 0;
        sbMaxNumberOfTouches = 0;
    }

    void setScrollHeight(float _sbScrollHeight){
        float _sbRatio = (float)sbScrollAreaHeight/_sbScrollHeight;
        if (_sbRatio > 1.0) {
            _sbRatio = 1.0;
            sbActive = FALSE;
        } else {
            sbActive = TRUE;
        }
        sbScrollBarHeight = sbScrollAreaHeight*_sbRatio;
        sbScrollBarYMax = sbScrollAreaY + sbScrollAreaHeight - sbScrollBarHeight;
    }

    void updateScrollBar(int _newWindowWidth, int _newWindowHeight, int _headerHeight, int _footerHeight, int _newScrollHeight){
        unregisterMouseEvents();
//        setup(0, _newWindowWidth-sbScrollBarWidth - sbScrollBarMargin*2, _headerHeight, sbScrollBarWidth, _newWindowHeight-_headerHeight-_footerHeight, _newWindowWidth-sbScrollBarWidth, _newWindowHeight/2, sbScrollBarWidth, sbDecelarationConstant, scrollMultiplier, sbScrollBarMargin);
        setup(0, _newWindowWidth, _newWindowHeight, _headerHeight, _footerHeight, sbScrollAreaWidth, sbDecelarationConstant, scrollMultiplier, sbScrollBarMargin);
        setScrollHeight((float)_newScrollHeight);
        if (sbActive) {
            registerMouseEvents();
        }
//        ofLog(OF_LOG_VERBOSE, "sbActive:" + ofToString(sbActive) );
    }

    void setScrollBarWidth(int _scrollBarWidth){
        sbScrollBarWidth = _scrollBarWidth;
    }

    float valMinMax(float val, float minv, float maxv) {
        return min(max(val, minv), maxv);
    }

    float getRelativePos(){
        if (sbActive) {
            float relativeValue = (sbScrollBarY-sbScrollBarYMin)/(sbScrollBarYMax-sbScrollBarYMin);
            return (relativeValue);
        } else {
            return 0.0;
        }
    }

    void setToTop(){
        sbScrollBarY = sbScrollBarYMin;
    }

    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }

    void unregisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }

//    void registerTouchEvents(){
//        ofAddListener(pad.update, this, &fakScrollBar::padUpdates);
//        ofAddListener(pad.touchRemoved, this, &fakScrollBar::removedTouch);
//    }

//    void unregisterTouchEvents(){
//        ofRemoveListener(pad.update, this, &fakScrollBar::padUpdates);
//    }

    bool insideScrollArea(float _x, float _y ){
        return _x >= sbScrollAreaX && _x < sbScrollAreaX + sbScrollAreaWidth && _y >= sbScrollAreaY && _y < sbScrollAreaY + sbScrollAreaHeight;
    }

    bool insideScrollBar(float _x, float _y ){
        return _x >= sbScrollBarX && _x < sbScrollBarX + sbScrollBarWidth && _y >= sbScrollBarY && _y < sbScrollBarY + sbScrollBarHeight;
    }

    void mouseMoved(ofMouseEventArgs & args){
        sbRollOverScrollBar = FALSE;
        sbRollOverScrollArea = FALSE;
        if (insideScrollArea(args.x, args.y)) {
            sbRollOverScrollArea = TRUE;
            if (insideScrollBar(args.x, args.y)) {
                sbRollOverScrollBar = TRUE;
            }
        }
    }

    void mouseDragged(ofMouseEventArgs & args){
        if (sbRollOverScrollArea) {
            sbScrollBarDrag = TRUE;
        }
        sbMousePos = ofVec2f(args.x, args.y);
        ofNotifyEvent(sbScrollingGoingOn, args, this);
    }

    void mouseReleased(ofMouseEventArgs & args){
        sbMousePos = ofVec2f(args.x, args.y);
        sbFirstClick = false;
        sbScrollBarDrag = false;
        ofNotifyEvent(sbScrollingGoingOn, args, this);
    }

    void mousePressed(ofMouseEventArgs & args){
        if (sbRollOverScrollArea) {
            sbScrollBarDrag = TRUE;
        }
        sbMousePos = ofVec2f(args.x, args.y);
        ofNotifyEvent(sbScrollingGoingOn, args, this);
    }

    void mouseScrolled(ofMouseEventArgs & args){
        //        ofLog(OF_LOG_VERBOSE, "scrollAmount x:y " + ofToString(args.x) + ":" + ofToString(args.y) );
        sbScrollBarDrag = false;

//        if (sbMaxNumberOfTouches > 0){ // check if touchmouse
//            if ((sbNumberOfTouches != 0)) {
//                sbMouseScrollVelocity = ofVec2f(args.x * scrollMultiplier, args.y * scrollMultiplier);
//                sbMouseIsScrolling = true;
//                sbCalculateScrollInertia = true;
//                ofNotifyEvent(sbScrollingGoingOn, args, this);
//                ofResetElapsedTimeCounter();
//            }
//        } else { // no touchmouse used
//            if ((ofGetElapsedTimeMillis() > 50) || (ofGetElapsedTimeMillis() < 10)) {
//                int noTouchMouseMultiplier = 2;
//                if (sbFirstNoTouchMouseEvent) {
//                    sbDecelarationConstant = sbDecelarationConstant / 2.0;
//                    sbFirstNoTouchMouseEvent = false;
//                }
//                sbMouseScrollVelocity = ofVec2f(args.x * scrollMultiplier * noTouchMouseMultiplier, args.y * scrollMultiplier * noTouchMouseMultiplier);
//                sbMouseIsScrolling = true;
//                sbCalculateScrollInertia = true;
//                ofNotifyEvent(sbScrollingGoingOn, args, this);
//                ofResetElapsedTimeCounter();
//            }
//        }

        sbMouseScrollVelocity = ofVec2f(args.scrollX * scrollMultiplier, args.scrollY * scrollMultiplier);
        sbMouseIsScrolling = true;
        sbCalculateScrollInertia = true;
        ofNotifyEvent(sbScrollingGoingOn, args, this);
        ofResetElapsedTimeCounter();
    }

    void mouseEntered(ofMouseEventArgs & args){}
    void mouseExited(ofMouseEventArgs & args){}

    void padUpdates(int & t) {
        sbNumberOfTouches = t;
        sbMaxNumberOfTouches = fmax(t, sbNumberOfTouches);
    }

    void removedTouch(int & r) {
        sbNumberOfTouches = r;
    }

    void update(){
        if (sbActive) {


            if (sbScrollBarDrag) {
//                ofLog(OF_LOG_VERBOSE, "_____________barUpdate " + ofToString(sbMousePos.y) );
                if (sbRollOverScrollBar) {
                    if (!sbFirstClick) {
                        sbFirstClickMouseY = sbMousePos.y - sbScrollBarY;
                        sbFirstClick = true;
                    }
                    sbScrollBarYNew = valMinMax(sbMousePos.y - sbFirstClickMouseY, sbScrollBarYMin, sbScrollBarYMax);
                    if ((abs(sbScrollBarYNew - sbScrollBarY)) > 1) {
                        sbScrollBarY = sbScrollBarY + (sbScrollBarYNew-sbScrollBarY)/sbDecelarationConstant;
                    }
                } else { // click insidescrollarea but not inside insidescrollbar
                    sbScrollBarYNew = valMinMax(sbMousePos.y - sbScrollBarHeight/2, sbScrollBarYMin, sbScrollBarYMax);
                    if ((abs(sbScrollBarYNew - sbScrollBarY)) > 1) {
                        sbScrollBarY = sbScrollBarY + (sbScrollBarYNew-sbScrollBarY)/sbDecelarationConstant;
                    }
                }

            } else if (sbCalculateScrollInertia){
//                ofLog(OF_LOG_VERBOSE, "_____________scrollUpdate " + ofToString(sbMouseScrollVelocity.y) );

                if ((sbScrollBarY > sbScrollBarYMax)) {

                    if ( sbMouseScrollVelocity.y <= 0 ) { // Return to bottom position
                        sbMouseScrollVelocity.y = sbReturnToBaseConstant * abs(sbScrollBarY - sbScrollBarYMax) *-1;
//                        ofLog(OF_LOG_VERBOSE, "Return " + ofToString(sbScrollBarY,2));
                    } else if (sbNumberOfTouches > 0){ // if still holding then do nothing
                        sbMouseScrollVelocity.y = sbMouseScrollVelocity.y * ((sbOvershootHeight - abs(sbScrollBarY - sbScrollBarYMax))/(float)sbOvershootHeight);
                        sbMouseScrollVelocity.y = fmin(sbMouseScrollVelocity.y, sbMaxVelocity);
                        sbScrollBarY = fmin(sbScrollBarY,sbScrollBarYMax+sbOvershootHeight);
                        ofLog(OF_LOG_VERBOSE, "sbMouseScrollVelocity " + ofToString(sbMouseScrollVelocity.y,2));
                        ofLog(OF_LOG_VERBOSE, "Holding " + ofToString(sbScrollBarY,2));
                    } else { // Slow down
                        float change = sbBounceDecelerationConstant * getTimeRampDown();
                        sbMouseScrollVelocity.y -= change;
                        sbMouseScrollVelocity.y = fmin(sbMouseScrollVelocity.y, sbMaxVelocity);
//                        ofLog(OF_LOG_VERBOSE, "Slow down " + ofToString(sbScrollBarY,2));
                    }

                    sbScrollBarY = sbScrollBarY + sbMouseScrollVelocity.y * getTimeRampDown();

                    if (sbScrollBarY < (sbScrollBarYMax + 0.001)){ // when close to aim (threshold 0.001), lead to aim manually
                        sbScrollBarY = sbScrollBarYMax;
                        sbMouseScrollVelocity = ofVec2f(0.0,0.0);
                    }

                } else if ((sbScrollBarY < sbScrollBarYMin)) {

                    if ( sbMouseScrollVelocity.y > 0 ) { // Return to bottom position
                        sbMouseScrollVelocity.y = sbReturnToBaseConstant * abs(sbScrollBarY - sbScrollBarYMin);
                    } else if (sbNumberOfTouches > 0){ // if still holding then do nothing
                        sbMouseScrollVelocity.y = sbMouseScrollVelocity.y * ((sbOvershootHeight - abs(sbScrollBarY - sbScrollBarYMin))/(float)sbOvershootHeight);
                        sbMouseScrollVelocity.y = fmin(sbMouseScrollVelocity.y, sbMaxVelocity);
                        sbScrollBarY = fmax(sbScrollBarY,sbScrollBarYMin-sbOvershootHeight);
                        ofLog(OF_LOG_VERBOSE, "sbMouseScrollVelocity " + ofToString(sbMouseScrollVelocity.y,2));
                        ofLog(OF_LOG_VERBOSE, "Holding " + ofToString(sbScrollBarY,2));
                    } else { // Slow down
                        float change = sbBounceDecelerationConstant * getTimeRampDown();
                        sbMouseScrollVelocity.y += change;
                        sbMouseScrollVelocity.y = fmax(sbMouseScrollVelocity.y, sbMaxVelocity * -1);
                    }

                    sbScrollBarY = sbScrollBarY + sbMouseScrollVelocity.y * getTimeRampDown();

                    if (sbScrollBarY > (sbScrollBarYMin - 0.001)){ // when close to aim (threshold 0.001), lead to aim manually
                        sbScrollBarY = sbScrollBarYMin;
                        sbMouseScrollVelocity = ofVec2f(0.0,0.0);
                    }

                } else {
                    sbMouseScrollVelocity.y = sbMouseScrollVelocity.y / sbDecelarationConstant;
                    sbScrollBarY = sbScrollBarY + sbMouseScrollVelocity.y * getTimeRampDown();
                }

                if (ofGetElapsedTimeMillis() > 50) {
                    sbMouseIsScrolling = false;
                    if (ofGetElapsedTimeMillis() > sbBounceBackTimeMs) {
                        sbCalculateScrollInertia = false;
                    }
                }
            }
        }
    }

    float getTimeRampDown(){
        return (1.0 - valMinMax(ofGetElapsedTimeMillis()/(float)sbBounceBackTimeMs, 0.0, 1.0));
    }

    void draw(){
        if (sbActive) {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255, 255, 255, 15);
            ofDrawRectangle(sbScrollAreaX, sbScrollAreaY, sbScrollAreaWidth, sbScrollAreaHeight);
            ofSetColor(238, 71, 0, 127);
            ofDrawRectRounded(sbScrollBarX, sbScrollBarY, sbScrollBarWidth, sbScrollBarHeight, sbScrollBarWidth);
            ofPopStyle();
        }
    }

    // Properties
    ofEvent<ofVec2f> sbClickedInside;
    ofEvent<ofVec2f> sbScrollingGoingOn;

    int sbID; // ID in Array
    float sbScrollBarX; //x position of Slider upper left corner
    float sbScrollBarY; //y position of Slider upper left corner
    float sbScrollBarYNew;
    int sbScrollBarWidth; //drawn width of ScrollBar
    int sbScrollBarHeight; //drawn height of ScrollBar
    float sbScrollBarYMin;
    float sbScrollBarYMax;
    int sbScrollBarMargin;
    bool sbActive;

    int sbScrollAreaWidth;
    int sbScrollAreaHeight;
    int sbScrollAreaX;//x position of ScrollBar upper left corner
    int sbScrollAreaY;//y position of ScrollBar upper left corner

    bool sbFirstClick;
    int sbFirstClickMouseY;
    bool sbRollOverScrollBar;
    bool sbRollOverScrollArea;

    bool sbScrollBarDrag;
    bool sbMouseIsScrolling;
    bool sbCalculateScrollInertia;

    ofVec2f sbMousePos;
    ofVec2f sbMouseScrollVelocity;

    int sbOvershootHeight;
    float scrollMultiplier;

    bool sbFirstNoTouchMouseEvent;
    float sbDecelarationConstant;
    float sbReturnToBaseConstant;
    float sbBounceDecelerationConstant;
    float sbMaxVelocity;
    int sbBounceBackTimeMs;

//    ofxMultiTouchPad pad;
    int sbNumberOfTouches;
    int sbMaxNumberOfTouches; //check if touchmouse is used
};

#endif // FAKSCROLLBAR_H
