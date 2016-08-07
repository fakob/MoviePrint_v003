#!/bin/sh

# set

DESTDIR='/Users/fakob/_jakob/work/fakob_MoviePrintApp_130515/03_Programming/02_Setups/01_OF/of_v0.9.2_osx_release/apps/myApps/MoviePrint_v003/bin'
TARGET='MoviePrint_v003_debug'

# echo "first parameter is $1"
# echo "first parameter is $2"

# DESTDIR = $1
# TARGET = $2

# echo "Destdir is $DESTDIR"
# echo "Target is $TARGET"

cp -f ../../../libs/fmodex/lib/osx/libfmodex.dylib "$DESTDIR/$TARGET.app/Contents/MacOS/libfmodex.dylib"; install_name_tool -change ./libfmodex.dylib @executable_path/libfmodex.dylib "$DESTDIR/$TARGET.app/Contents/MacOS/$TARGET";
mkdir -p "$DESTDIR/$TARGET.app/Contents/Resources/"
# cp -f "$ICON_FILE" "$DESTDIR/$TARGET.app/Contents/Resources/"

cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavcodec.56.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavutil.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavformat.56.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavdevice.56.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libswresample.1.2.100.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libswresample.1.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libswscale.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libswscale.3.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavutil.54.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavformat.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavcodec.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavdevice.56.4.100.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavfilter.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libswresample.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavdevice.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavutil.54.28.100.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavfilter.5.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavfilter.5.21.100.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libswscale.3.1.101.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavformat.56.40.100.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n
cp -fR ../../../addons/ofxAvCodec/libs/avcodec/lib/osx/libavcodec.56.47.100.dylib "$DESTDIR/$TARGET.app/Contents/MacOS" 2>/dev/null | echo -n