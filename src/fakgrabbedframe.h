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
    ofImage gsImage; //image into which the videoframe is grabbed
    ofTexture gsTexture; //texture into which the grabbedStill is copied - this is drawn to screen - this is also resized if necessary
    int gsID; // ID in Array
    int gsFrameNumber; // current Framenumber
    int gsUpdateOrderNumber; // order which is used to update frames, reordering dependend on framenumber, so the playhead doesnt jump back and forth
    bool gsManipulated = FALSE; // indicating if the framenumber is automatically assigned or if it was manually manipulated
    bool gsToBeUpdated = TRUE; // This frame has changed somehow, it needs to be redrawn
    bool gsToBeGrabbed = TRUE; // The framenumber has changed so the frame has to be grabbed again (threadable)
    int gsWidth; //original width of grabbedStill
    int gsHeight; //original height of grabbedStill
    //    bool gsFrameCorrupt = FALSE; //set if frame is corrupt

};

#endif // FAKGRABBEDFRAME_H
