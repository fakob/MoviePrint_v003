#ifndef FAKGRABBEDMOVIE_H
#define FAKGRABBEDMOVIE_H

#include <string>
#include <iostream>
#include "ofMain.h"
#include "fakGrabbedMovieStill.h"

class fakGrabbedMovie
{
public:
    fakGrabbedMovie(){};

bool loadNewMovie(string vfMovieName){

//    stop(FALSE);

    isMovieLoaded = FALSE;

    ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function");
    gmMovie.load(vfMovieName);
    ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function");

    if (gmMovie.isLoaded()) {
        isMovieLoaded = TRUE;
        gmTotalFrames = gmMovie.getTotalNumFrames();
        gmDuration = gmMovie.getDuration();
        gmFrameRate = ceil(gmTotalFrames / gmDuration);
        gmImageRatio = gmMovie.getWidth()/gmMovie.getHeight();
        gmPixelRatio = gmMovie.getPixelFormat();
        ofLog(OF_LOG_VERBOSE, "ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) + " totalFrames:" + ofToString(gmTotalFrames) + " getDuration:" + ofToString(gmMovie.getDuration()));

    } else {
        ofLog(OF_LOG_VERBOSE, "Movie not loaded");
    }


    return isMovieLoaded;
}

void update(){
    if (isMovieLoaded) {
        gmMovie.update();
    }
}

//void allocateNewStills(int _gmThumbWidth, int _gmThumbHeight) {
//    if (isMovieLoaded) {
//        gmCurrAllocating = true;
//        gmThumbWidth = _gmThumbWidth;
//        gmThumbHeight = _gmThumbHeight;


//        gmCurrAllocating = false;
//    }
//}


    //properties

    ofVideoPlayer gmMovie;
    bool isMovieLoaded = false;
    int gmTotalFrames;
    float gmDuration;
    float gmImageRatio;
    float gmPixelRatio;
    float gmFrameRate;
    int gmThumbWidth;
    int gmThumbHeight;
    bool gmCurrAllocating;

    vector<fakGrabbedMovieStill> grabbedStill;

};

#endif // FAKGRABBEDMOVIE_H
