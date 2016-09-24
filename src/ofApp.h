#pragma once

#include "ofMain.h"
#include "ofxGui.h"

struct hole
{
    ofPoint pos;
    float radius;
};
class ofApp : public ofBaseApp{
	public:
        //setup
        void reset();
        void setupGui();
		void setup();

        void update();

        // drawing method
        ofColor getLineColor(float x);
           void draw();
		

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
        //gui
        ofxPanel    _gui;
        ofxButton    _reset;
        ofParameter<bool>    _bloody;

        ofParameter<int>    _lineNb;

       // holes
        std::vector<struct hole>  _holes;
        struct hole _currHole;

        //shader
        ofShader _shader;

};
