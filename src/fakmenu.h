

//
//  fakMenu.h
//  MoviePrint
//
//  Created by fakob on 14/02/21.
//
//

#ifndef FAKMENU_H
#define FAKMENU_H

#include "ofMain.h"
#include "ofxEasing.h"
//#include "ofxTween.h"
//#include "ofxUI.h"

#define FAK_ORANGECOLOR ofColor(238, 71, 0, 255)
#define FAK_DARKORANGECOLOR ofColor(99, 30, 0, 255)
#define FAK_DARKDARKORANGECOLOR ofColor(75, 21, 0, 255)
#define FAK_MIDDLEDARKORANGECOLOR ofColor(170, 50, 0, 255)
#define FAK_LIGHTERMIDDLEDARKORANGECOLOR ofColor(185, 55, 0, 255)

#define FAK_ORANGE1 ofColor(255, 80, 6, 255)
#define FAK_ORANGE2 ofColor(255, 183, 153, 255)
#define FAK_ORANGE3 ofColor(255, 147, 101, 255)
#define FAK_ORANGE4 ofColor(255, 168, 131, 255)
#define FAK_ORANGE5 ofColor(255, 211, 193, 255)

#define FAK_WHITE ofColor(255, 255, 255, 255)
#define FAK_BLACK ofColor(0, 0, 0, 255)
#define FAK_SHADOW ofColor(0, 0, 0, 130)
#define FAK_GRAY ofColor(59, 59, 59, 255)


#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)
#define FAK_LIGHTGRAY ofColor(205, 205, 205, 255)
#define FAK_MIDDLEGRAY ofColor(195, 195, 195, 255)

class fakMenu{

public:

    fakMenu(){

    }

    // functions

    void setupMenu(int _ID, float _mMenuX, float _mMenuY, float _mMenuWidth, float _mMenuHeight, float _mMenuRollOverDimension, bool _mButtonActive, char _mLocationOfMenu, bool _mNoBackground){
        mMenuX = _mMenuX;
        mMenuY = _mMenuY;
        mMenuWidth = _mMenuWidth;
        mMenuHeight = _mMenuHeight;
        mMenuRollOverDimension = _mMenuRollOverDimension;
//        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
        tweenMenuInOut.initialTime = ofGetElapsedTimef() + ofRandom(0, 0.3);
        tweenMenuInOut.duration = ofRandom(0.6, 1.0);
        tweenMenuInOut.minValue = 1.0;
        tweenMenuInOut.maxValue = 0.0;
//        tweenMenuInOut2.setParameters(1,easingelastic,ofxTween::easeOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
        tweenMenuInOut2.initialTime = ofGetElapsedTimef() + ofRandom(0, 0.3);
        tweenMenuInOut2.duration = ofRandom(0.6, 1.0);
        tweenMenuInOut2.minValue = 1.0;
        tweenMenuInOut2.maxValue = 0.0;
        mMenuOffsetX = 2;
        mMenuOffsetY = 28;
        mShadowMargin = 1.5;
        mMenuStripeHeight = 8;
        mInsideMenuHead = FALSE;
        mMenuActive = _mButtonActive;
        mLocationOfMenu = _mLocationOfMenu;
        mNoBackground = _mNoBackground;
        mIsOpenManually = false;
        mMenuID = _ID;

        switch (mMenuID) {
            case 1:
                mMenuImage.load("MoviePrint_Layout_Menu1_v001_00001.png");
                mBackgroundColor = FAK_ORANGE1;
                break;
            case 4:
                mMenuImage.load("MoviePrint_Layout_Menu2_v001_00001.png");
                mBackgroundColor = FAK_ORANGE4;
                break;
            case 3:
                mMenuImage.load("MoviePrint_Layout_Menu3_v001_00001.png");
                mBackgroundColor = FAK_ORANGE3;
                break;
            case 2:
                mMenuImage.load("MoviePrint_Layout_Menu4_v001_00001.png");
                mBackgroundColor = FAK_ORANGE2;
                break;
            case 5:
                mMenuImage.load("MoviePrint_Layout_Menu5_v001_00001.png");
                mBackgroundColor = FAK_ORANGE5;
                break;
            case 6:
                mMenuImage.load("MoviePrint_Layout_Menu6_v001_00001.png");
                mBackgroundColor = FAK_ORANGE3;
                break;
            default:
                mBackgroundColor = FAK_GRAY;
                break;
        }

    }

    void setTweenIn(){
//        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,500,0);
        tweenMenuInOut.initialTime = ofGetElapsedTimef();
        tweenMenuInOut.duration = 0.5;
        tweenMenuInOut.minValue = 0.0;
        tweenMenuInOut.maxValue = 1.0;
//        tweenMenuInOut2.setParameters(1,easingexpo,ofxTween::easeOut,0.0,1.0,300,0);
        tweenMenuInOut2.initialTime = ofGetElapsedTimef();
        tweenMenuInOut2.duration = 0.3;
        tweenMenuInOut2.minValue = 0.0;
        tweenMenuInOut2.maxValue = 1.0;
        ofNotifyEvent(mMenuIsBeingOpened, mMenuID, this);
    }

    void setTweenOut(){
//        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,500,0);
        tweenMenuInOut.initialTime = ofGetElapsedTimef();
        tweenMenuInOut.duration = 0.5;
        tweenMenuInOut.minValue = 1.0;
        tweenMenuInOut.maxValue = 0.0;
//        tweenMenuInOut2.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,300,0);
        tweenMenuInOut2.initialTime = ofGetElapsedTimef();
        tweenMenuInOut2.duration = 0.3;
        tweenMenuInOut2.minValue = 1.0;
        tweenMenuInOut2.maxValue = 0.0;
        ofNotifyEvent(mMenuIsBeingClosed, mMenuID, this);
    }

    bool insideMenuHead(float _x, float _y ){
        float minX = mMenuX;
        float maxX = mMenuX + mMenuWidth;
        float minY, maxY;
        if (mLocationOfMenu == 'T') {
            minY = mMenuY;
            maxY = mMenuY + mMenuRollOverDimension;
        } else if (mLocationOfMenu == 'B') {
            minY = mMenuY - mMenuRollOverDimension;
            maxY = mMenuY;
        } else if (mLocationOfMenu == 'L') {
            minX = mMenuX;
            maxX = mMenuX + mMenuRollOverDimension;
            minY = mMenuY;
            maxY = mMenuY + mMenuHeight;
        }
        return _x >= minX && _x < maxX && _y >= minY && _y < maxY;
    }

    bool insideMenu(float _x, float _y ){
        float minX = mMenuX;
        float maxX = mMenuX + mMenuWidth;
        float minY, maxY;
        if (mLocationOfMenu == 'T') {
            minY = mMenuY;
            maxY = mMenuY + mMenuHeight;
        } else if (mLocationOfMenu == 'B') {
            minY = mMenuY - mMenuHeight - mMenuRollOverDimension;
            maxY = mMenuY;
        } else if (mLocationOfMenu == 'L') {
            minX = mMenuX;
            maxX = mMenuX + mMenuRollOverDimension + mMenuWidth;
            minY = mMenuY;
            maxY = mMenuY + mMenuHeight;
        }
        return _x >= minX && _x < maxX && _y >= minY && _y < maxY;
    }

    void openMenuManually(){
        mIsOpenManually = true;
        if (tweenMenuInOut.value != 1.0){
            setTweenIn();
        }
    }

    void closeMenuManually(){
        mIsOpenManually = false;
        if (tweenMenuInOut.value != 0.0){
            mInsideMenuHead = false;
            setTweenOut();
        }
    }

    void mouseMoved(ofMouseEventArgs & args){
        if (mMenuActive) {
            if (mInsideMenuHead) {
                if (!insideMenu(args.x, args.y)) {
                    mInsideMenuHead = false;
                    setTweenOut();
                }
            } else {
                if (insideMenuHead(args.x, args.y)) {
                    mInsideMenuHead = TRUE;
                    setTweenIn();
                }
            }
        }
    }

    void mouseDragged(ofMouseEventArgs & args){
    }

    void mouseReleased(ofMouseEventArgs & args){
    }

    void mousePressed(ofMouseEventArgs & args){
        if (mInsideMenuHead) {
            ofNotifyEvent(mMenuIsBeingClicked, mMenuID, this);
        }
    }

    void mouseScrolled(ofMouseEventArgs & args){
    }

    void mouseEntered(ofMouseEventArgs & args){
    }

    void mouseExited(ofMouseEventArgs & args){
    }

    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }

    void unRegisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }

    void setPosition(float _mMenuX, float _mMenuY){
        mMenuX = _mMenuX;
        mMenuY = _mMenuY;
    }

    void setSize(float _mMenuWidth, float _mMenuHeight){
        mMenuWidth = _mMenuWidth;
        mMenuHeight = _mMenuHeight;
    }

    float getPositionX(){
        return mMenuX;
    }

    float getPositionY(){
        return mMenuY;
    }

    float getSizeW(){
        return mMenuWidth;
    }

    float getSizeH(){
        return mMenuRollOverDimension + (mMenuHeight - mMenuRollOverDimension) * tweenMenuInOut.value;
    }

    float getRelSizeH(){
        return tweenMenuInOut.value;
    }

    float getRelSizeH2(){
        return tweenMenuInOut2.value;
    }

    void updateMenu(){
        tweenMenuInOut.value = ofxeasing::map_clamp(ofGetElapsedTimef(), tweenMenuInOut.initialTime, (tweenMenuInOut.initialTime + tweenMenuInOut.duration), tweenMenuInOut.minValue, tweenMenuInOut.maxValue, &ofxeasing::exp::easeInOut);
        tweenMenuInOut2.value = ofxeasing::map_clamp(ofGetElapsedTimef(), tweenMenuInOut2.initialTime, (tweenMenuInOut2.initialTime + tweenMenuInOut2.duration), tweenMenuInOut2.minValue, tweenMenuInOut2.maxValue, &ofxeasing::exp::easeOut);
    }

    bool getMenuActivated(){
        return mInsideMenuHead || mIsOpenManually;
    }


    void setMenuActive(){
        mMenuActive = true;
    }

    void setMenuInactive(){
        closeMenuManually();
        mMenuActive = false;
    }

    bool getInsideMenuHead(){
        return mInsideMenuHead;
    }

    void drawMenu(){
        updateMenu();
        ofPushMatrix();
        ofPushStyle();
        if (mLocationOfMenu == 'T') {
            ofEnableAlphaBlending();
            ofSetColor(FAK_WHITE);
            mMenuImage.draw(mMenuX - mMenuOffsetX, mMenuY, mMenuImage.getWidth(), mMenuImage.getHeight());
//            ofSetColor(FAK_SHADOW);
//            ofRectRounded(mMenuX - mShadowMargin*0.3 + mMenuOffsetX, mMenuY + mMenuOffsetY, mMenuWidth - mMenuOffsetX + mShadowMargin*2, mMenuStripeHeight + mMenuHeight * tweenMenuInOut.value,4.0 * tweenMenuInOut.value);
            if (!mNoBackground) {
                ofColor tempColor = mBackgroundColor;
                ofSetColor(tempColor.lerp(FAK_ORANGE1, tweenMenuInOut.value));
                ofDrawRectRounded(mMenuX, mMenuY + mMenuOffsetY, mMenuWidth, mMenuStripeHeight + (mMenuHeight - mMenuOffsetY - mMenuStripeHeight) * tweenMenuInOut.value, 4.0 * tweenMenuInOut.value);
            } else {
                ofSetColor(mBackgroundColor);
                ofDrawRectangle(mMenuX, mMenuY + mMenuOffsetY, mMenuWidth, mMenuStripeHeight);
            }
        } else if (mLocationOfMenu == 'B') {
            ofEnableAlphaBlending();
            ofSetColor(FAK_SHADOW);
            ofDrawRectangle(mMenuX, mMenuY - (mMenuRollOverDimension + 3.4 + mMenuHeight * tweenMenuInOut.value), mMenuWidth, mMenuRollOverDimension + mMenuHeight * tweenMenuInOut.value);
            ofSetColor(mBackgroundColor);
            ofDrawRectangle(mMenuX, mMenuY - (mMenuRollOverDimension + mMenuHeight * tweenMenuInOut.value), mMenuWidth, mMenuRollOverDimension + mMenuHeight * tweenMenuInOut.value);
        } else if (mLocationOfMenu == 'L') {
            ofEnableAlphaBlending();
            ofColor tempColor = mBackgroundColor;
//            tempColor.set(tempColor.r, tempColor.g, tempColor.b, ((sin(ofGetElapsedTimef()*3)+1.0)/2.0)*200+30); //pulsating
            ofSetColor(tempColor.lerp(FAK_ORANGE3, tweenMenuInOut.value));
            ofDrawRectangle(mMenuX, mMenuY, mMenuRollOverDimension + mMenuWidth * tweenMenuInOut.value, mMenuHeight);
            ofSetColor(255, 255, 255);
            mMenuImage.draw(mMenuX - mMenuRollOverDimension/2.0 + mMenuWidth * tweenMenuInOut.value/2.0, mMenuY);
        }
        ofPopStyle();
        ofPopMatrix();
    }



    // Properties

    ofEvent<int> mMenuIsBeingOpened;
    ofEvent<int> mMenuIsBeingClosed;
    ofEvent<int> mMenuIsBeingClicked;

    int mMenuID;
    bool mInsideMenuHead;
    bool mMenuActive;
    char mLocationOfMenu; // 'T' for Top, 'B' for Bottom, 'L' for Left, 'R' for Right
    bool mIsOpenManually;
    bool mNoBackground;

    float mMenuX;
    float mMenuY;
    float mMenuWidth;
    float mMenuHeight;
    float mMenuRollOverDimension;
    int mMenuOffsetX;
    int mMenuOffsetY;
    int mMenuStripeHeight; //stripe of menu which is already visible
    float mShadowMargin;
    ofImage mMenuImage;
    ofColor mBackgroundColor;

    //--------------------------------------------------------------
    struct tweenStruct {
        float value;
        float initialTime;
        float duration;
        float minValue;
        float maxValue;
    };

    tweenStruct tweenMenuInOut;
    tweenStruct tweenMenuInOut2;
//    ofxTween tweenMenuInOut;
//    ofxTween tweenMenuInOut2;

//    ofxEasingBack 	easingback;
//    ofxEasingBounce 	easingbounce;
//    ofxEasingCirc 	easingcirc;
//    ofxEasingCubic 	easingcubic;
//    ofxEasingElastic easingelastic;
//    ofxEasingExpo 	easingexpo;
//    ofxEasingLinear 	easinglinear;
//    ofxEasingQuad 	easingquad;
//    ofxEasingQuart 	easingquart;
//    ofxEasingQuint 	easingquint;
//    ofxEasingSine	easingsine;

};

#endif // FAKMENU_H
