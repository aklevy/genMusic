#pragma once

//#include "ofMain.h"
#include "soundCircle.h"
#include "soundLine.h"

#include "ofxGui.h"
//#include "ofxMaxim.h"
#include "ofxOssia.h"

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
    ofApp();

    // listeners
    void reset(bool& newval);
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

    void inputFromHotHand(ofVec3f& newval);
    void inputFromHotHandX(float& newval);
    void inputFromHotHandY(float& newval);

    void inputToCircle(int x, int y, float z = 0.);

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

    // Window dimension
    float       _windowWidth;
    float       _windowHeight;



    // Network for communication with i-score
    ofxOssia             _ossia;

    // Gui
    ofxPanel    _gui;

    ossia::Parameter<bool>    _reset;
    ossia::Parameter<bool>       _inputHotHand;
    ossia::Parameter<ofVec3f>    _valueHotHand;
    ossia::Parameter<float>    _valueHotHandX;
    ossia::Parameter<float>    _valueHotHandY;

    // Separation line
    ossia::Parameter<float>      _sepLineX;
    ossia::Parameter<float>      _sepLineWidth;
    bool                    _sepLineSelected;

    /*
     *  Sound Lines
     *
     * */

    // line parameters
    ossia::ParameterGroup       _lineParameters;
    ossia::Parameter<int>       _lineNb;
    ossia::Parameter<float>     _lineWidth;
    ossia::Parameter<float>     _lineAmplitude;
    ossia::Parameter<float>     _lineFrequence;
    ossia::Parameter<float>     _lineNoise;
    ofParameter<ofColor>        _lineDefaultColor;
    ofParameter<ofColor>        _lineMovingColor;

    // lines
    std::mutex                  _lineMutex;
    std::vector<SoundLine>      _vecSoundLines;

    /*
     *  Sound Circles
     *
     * */
    // Circle parameters
    ossia::ParameterGroup         _circleParameters;
    ofParameter<ofColor>    _circleDefaultColor;
    ofParameter<ofColor>    _currentCircleColor;
    // ofParameter<bool>       _circleFill;
    ofParameter<float>      _circleGrowingSpeed;
    ossia::Parameter<float>      _circleFrequence;
    ofParameter<bool>       _drawAllCircles;


    // circles
    std::mutex              _circleMutex;
    std::vector<SoundCircle> _vecSoundCircles;
    SoundCircle             _currCircle;
    ofFbo                   _fbo; // fbo containing circles' drawings

    // ofxMaxim param
    int		bufferSize; /* buffer size */
    int		sampleRate;
    maxiOsc     _oscSoundSepLine;

    // sound param
    ofParameterGroup        _soundParameters;
    ofParameter<int>        _freqMod;
    ofParameter<float>      _volumeLine;
    ofParameter<float>      _volumeCircle;


    using lock = std::lock_guard<std::mutex>;
};
