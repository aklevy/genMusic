#include "ofMain.h"

#include "ofxMaxim.h"


class SoundLine{

public:
    SoundLine();
    void reset();

    // update method
    void update(float x, int y);

    // drawing method
    void drawSoundLine(float x, float winHeight, float lineWidth);

    // sound method
    float getSound(float x);

    void setDefaultColor(ofColor col){_lineDefaultColor = col;}
    void setMovingColor(ofColor col){_lineMovingColor = col;}
    void setPlayingColor(ofColor col){_linePlayingColor = col;}

    // get line state
    bool isMoving(){return _bMove;}
    void setMove(bool b){_bMove = b;}
    bool isPlaying(){return _bPlay;}

private:

    // shape parameters
    int         _offsetY;

    // line state
    bool _bPlay;
    bool _bMove;

    // color to be displayed
    ofColor     _lineColor;

    // different color for different status
    ofColor     _lineDefaultColor;
    ofColor     _lineMovingColor;
    ofColor     _linePlayingColor;


    // sound parameters
    maxiEnv     _envSound;
    maxiOsc     _oscSound;
};
