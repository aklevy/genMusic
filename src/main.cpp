#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppGlutWindow.h"
#include "ofAppGLFWWindow.h"
//========================================================================
int main( ){
 /*   ofAppGlutWindow window;
    //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);
   //    window.setGlutDisplayString("rgba double samples>=4");
    window.setGlutDisplayString( "rgb double depth alpha samples=4");
    window.setGlutDisplayString( "rgba double depth alpha samples=4");
*/
    ofAppGLFWWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
