#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMaxim.h"

struct circle
{
    ofPoint pos;
    float radius;

    maxiOsc oscSound;
    int lifetime;

};
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

    // callbacks
    void reset();
    void lineNbModified(int& newval);

    //setup
    void setupGui();
    void setup();

    void update();

    // drawing method
    bool isLineMoving(float x);
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
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:

    float       _windowWidth;
    float       _windowHeight;
    //gui
    ofxPanel    _gui;
    ofxButton    _reset;

    // line parameters
    ofParameterGroup        _lineParameters;
    ofParameter<int>        _lineNb;
    ofParameter<float>      _lineWidth;
    ofParameter<float>      _lineAmplitude;
    ofParameter<float>      _lineFrequence;
    ofParameter<ofColor>    _lineDefaultColor;
    ofParameter<ofColor>    _lineMovingColor;

    // lines
    std::mutex _linemutex;
    std::vector<line>       _linesSound;

    // Circle parameters
    ofParameterGroup        _circleParameters;
    ofParameter<ofColor>    _circleDefaultColor;
    ofParameter<ofColor>    _currentCircleColor;
    ofParameter<bool>       _circleFill;
    ofParameter<float>      _circleGrowingSpeed;


    // circles
    std::vector<struct circle>  _circles;
    struct circle _currCircle;

    // ofxMaxim param
    int		bufferSize; /* buffer size */
    int		sampleRate;
    maxiOsc sawOsc;
    maxiFilter filt;
    maxiDistortion dist;
    maxiEnv         env;

    maxiSample mySample;

    // sound param
    ofParameterGroup        _soundParameters;
    ofParameter<int>        _freqMod;


    using lock = std::lock_guard<std::mutex>;};
