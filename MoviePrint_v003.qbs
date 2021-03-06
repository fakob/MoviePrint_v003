import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '../../..'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "resources.qrc",
            "src/fakgrabbedframe.h",
            "src/fakgrabbedframeoverlay.h",
            "src/fakgrabbedlist.h",
            "src/fakgrabbedlistitem.h",
            "src/fakgrabbedmovie.h",
            "src/fakgrabframes.h",
            "src/fakmenu.h",
            "src/fakscrollbar.h",
            "src/faktween.h",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
        ]

        of.addons: [
            "ofxNotify",
            "ofxEasing",
            "ofxMSATimer",
            "ofxImgui",
            "ofxFontStash",
            "ofxCv",
            "ofxOpenCv"
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: ["Qtkit"]       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }

//        Properties{
//            condition: qbs.buildVariant.contains("debug") && platform === "osx"
//            bundle.infoPlist: ({"CFBundleIconFile":"MoviePrint_Logo_v002_00000.icns"})
//        }

//        Properties{
//            condition: qbs.buildVariant.contains("release") && platform === "osx"
//            bundle.infoPlist: ({"CFBundleIconFile":"MoviePrint_Logo_v002_00000.icns"})
//        }

//        Properties{
//            condition: qbs.buildVariant.contains("release") && platform === "osx"
//            bundle.infoPlist: ({"CFBundleIdentifier":"com.fakob.MoviePrint"})
//        }

//        Properties{
//            condition: qbs.buildVariant.contains("release") && platform === "osx"
//            bundle.infoPlist: ({"CFBundleShortVersionString":"0.3.3.0"})
//        }

//        Properties{
//            condition: qbs.buildVariant.contains("release") && platform === "osx"
//            bundle.infoPlist: ({"CFBundleVersion":"0.3.3.0"})
//        }

//        Properties{
//            condition: qbs.buildVariant.contains("release") && platform === "osx"
//            bundle.infoPlist: ({"LSApplicationCategoryType":"public.app-category.video"})
//        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
