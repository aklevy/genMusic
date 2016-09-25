#pragma once

#include "ofMain.h"
#include "ofxGui.h"

struct circle
{
    ofPoint pos;
    float radius;
};
class ofApp : public ofBaseApp{
	public:
        ~ofApp();

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

        // line parameters
        ofParameterGroup        _lineParameters;
        ofParameter<int>    _lineNb;
        ofParameter<float>    _lineWidth;
        ofParameter<float>    _lineAmplitude;
        ofParameter<float>    _lineFrequence;
        ofParameter<ofColor>    _lineDefaultColor;
        ofParameter<ofColor>    _lineMovingColor;

        // Circle parameters
        ofParameterGroup        _circleParameters;
        ofParameter<ofColor>    _circleDefaultColor;
        ofParameter<bool>       _circleFill;
        ofParameter<float>      _circleGrowingSpeed;


       // holes
        std::vector<struct circle>  _circles;
        struct circle _currCircle;

        //shader
        ofShader _shader;

};
