#pragma once

//#include "ofMain.h"
#include "soundCircle.h"
#include "soundLine.h"

#include "ofxGui.h"
//#include "ofxMaxim.h"
//#include "parameter.hpp"
//#include "network.hpp"

struct line
{
    bool bPlay;
    bool bMove;
    int  offsetY;
    ofColor color;

    maxiOsc oscSound;
};
class ofApp : public ofBaseApp{
public:
    ~ofApp();

    // listeners
    void reset();
    void changeInput(bool& newval);

    void lineNbModified(int& newval);
    void lineDefColModified(ofColor& newval);
    void lineMovColModified(ofColor& newval);

    void circleColModified(ofColor& newval);
    void currentCircleColModified(ofColor& newval);

    //setup
    void setupGui();
    void setup();

    void update();

    // checking if the line should be moving (i.e. touching circle)
    bool isLineTouchingCircle(float x);

    // drawing method
    void drawInFbo();
    void draw();


    // Audio output and input methods

    void audioOut(float * output, int bufferSize, int nChannels);


    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);


private:

    float       _windowWidth;
    float       _windowHeight;
    ofFbo               _fbo;

    //gui
    ofxPanel    _gui;
    ofxButton    _reset;
    ofParameter<bool>       _inputHotHand;

    // line parameters
    ofParameterGroup        _lineParameters;
    ofParameter<int>        _lineNb;
    ofParameter<float>      _lineWidth;
    ofParameter<float>      _lineAmplitude;
    ofParameter<float>      _lineFrequence;
    ofParameter<ofColor>    _lineDefaultColor;
    ofParameter<ofColor>    _lineMovingColor;

    // lines
    std::mutex              _lineMutex;
    std::vector<SoundLine>  _vecSoundLines;

    // Circle parameters
    ofParameterGroup        _circleParameters;
    ofParameter<ofColor>    _circleDefaultColor;
    ofParameter<ofColor>    _currentCircleColor;
    ofParameter<bool>       _drawAllCircles;
   // ofParameter<bool>       _circleFill;
    ofParameter<float>      _circleGrowingSpeed;


    // circles
    std::mutex              _circleMutex;
    std::vector<SoundCircle>  _vecSoundCircles;
    SoundCircle _currCircle;

    // ofxMaxim param
    int		bufferSize; /* buffer size */
    int		sampleRate;

    // sound param
    ofParameterGroup        _soundParameters;
    ofParameter<int>        _freqMod;
    ofParameter<float>      _volumeLine;
    ofParameter<float>      _volumeCircle;


    using lock = std::lock_guard<std::mutex>;
};
