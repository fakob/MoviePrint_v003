//
//  fakGrabbedFrame.h
//
//  Created by fakob on 24.07.2016
//

#ifndef FAKGRABBEDFRAME_H
#define FAKGRABBEDFRAME_H

class fakGrabbedFrame {

public:

    fakGrabbedFrame(){}

    // Properties
    ofImage gfImage; //image into which the videoframe is grabbed
    ofTexture gfTexture; //texture into which the grabbedStill is copied - this is drawn to screen - this is also resized if necessary
    int gfID; // ID in Array
    int gfFrameNumber; // current Framenumber
    int gfUpdateOrderNumber; // order which is used to update frames, reordering dependend on framenumber, so the playhead doesnt jump back and forth
    bool gfManipulated = FALSE; // indicating if the framenumber is automatically assigned or if it was manually manipulated
    bool gfToBeUpdated = TRUE; // This frame has changed somehow, it needs to be redrawn
    bool gfToBeGrabbed = TRUE; // The framenumber has changed so the frame has to be grabbed again (threadable)
    int gfWidth; //original width of grabbedStill
    int gfHeight; //original height of grabbedStill
    //    bool gfFrameCorrupt = FALSE; //set if frame is corrupt

};

#endif // FAKGRABBEDFRAME_H
