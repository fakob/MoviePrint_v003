# MoviePrint_v003

// I am not a developer. You will see that, when you see the code :-)

This is a slightly adapted version of the MoviePrint tool. The OSX version can be downloaded here - http://www.fakob.com/2014/movieprint-an-osx-tool/
This version is a first try to support deployment also to other platforms. Therefor OpenCV has been used to read the movie files. Qt Creator has been used to support different platforms.

It was successfully build on OSX 10.11.6 with Qt Creator 3.6.1
openframeworks v0.9.3 is needed including the following addons
ofxNotify
ofxEasing
ofxMSATimer
ofxImgui
ofxFontStash
ofxCv
ofxOpenCV

// Known issues
- The resources (images and fonts) are not copied into the package. The data folder has to be on the same level as the built app.
- OpenCV seems to have a problem reading some video files containing none ASCII characters
- No support for Pixel aspect ratio different then 1.0
- First frame is currently ignored

// This version has not been thorougly tested
