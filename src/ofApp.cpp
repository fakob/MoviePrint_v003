#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
}

//--------------------------------------------------------------
void ofApp::update(){
    loadedMovie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    loadedMovie.gmMovie.draw(20,20);
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

    if( dragInfo.files.size() > 0 ){

        ofLog(OF_LOG_VERBOSE, "Dragged items:" + ofToString(dragInfo.files));

        for (int i=0; i<dragInfo.files.size(); i++) {
            ofLog(OF_LOG_VERBOSE, "Dragged item:" + ofToString(i) + "-" + ofToString(dragInfo.files[i]));
        }

//        loadedMovie.load(dragInfo.files[0]);
//        loadedMovie.setLoopState(OF_LOOP_NORMAL);
//        loadedMovie.play();
        loadedMovie.loadNewMovie(dragInfo.files[0]);
        loadedMovie.gmMovie.play();
    }
}
