#include "ofMain.h"

#include "ofxMaxim.h"


class SoundCircle{

public:
    SoundCircle();
    void reset(int soundDur);
    void setup(ofPoint pos, float rad, int soundDur);

    // update method
    void update(float dt);
    void updateRadius(float speed, bool bLimit);

    // drawing method
    void drawDashedCircle();
    void drawSoundCircle();

    // sound method
    int getSoundDuration(){return _soundDuration;}
    float getSound();

    bool isTouchingLine(float x);
    bool isSameCircle(float x, float y, float err);

    void setColor(ofColor col){_circleColor = col;}
    void setDashColor(ofColor col){_dashCircleColor = col;}

    void toBeRemoved(){_bErase = true;}
    bool isToBeRemoved(){return _bErase;}
private:
    // shape parameters
    ofPoint     _position;
    float       _radius;
    ofColor     _circleColor;

    // dash circle parameters
    float       _dashWidth;
    ofColor     _dashCircleColor;

    // sound parameters
    maxiEnv     _envSound;
    maxiOsc     _oscSound;
    maxiFilter  _filterSound;
    int         _soundDuration;
    bool        _bErase;
};
