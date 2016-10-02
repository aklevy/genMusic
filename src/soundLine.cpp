#include "soundLine.h"

//--------------------------------------------------------------

SoundLine::SoundLine ()
    : _offsetY(0),
      _bPlay(false),
      _bMove(false)
{
    _linePlayingColor = ofColor(255,0,0);
    _envSound.setAttack(1);
    _envSound.setDecay(1000);
    _envSound.setSustain(1000);
    _envSound.setRelease(1);
}
//--------------------------------------------------------------

void SoundLine::reset()
{
    _bPlay = false;
    _bMove = false;
    _offsetY = 0;

    _lineColor = _lineDefaultColor;
    _oscSound.phaseReset(0);
}
//--------------------------------------------------------------

void SoundLine::update(float x, int y)
{
    if(_bMove)
    {
        _offsetY = y;
        // play sound ?
        if(abs(_offsetY) < 30 ) // hitting the upper screen limit
        {
            _bPlay = true;
            _lineColor = ofColor(255,0,0);
        }
        else
        {
            _bPlay = false;
            _lineColor = _lineMovingColor;
        }
    }
    else
    {
        _offsetY =  0;
        _bMove = false;
        _bPlay = false;
        _lineColor = _lineDefaultColor;
    }
}
//--------------------------------------------------------------

void SoundLine::drawSoundLine(float x, float winHeight, float lineWidth)
{
    glLineWidth(lineWidth);
    glColor4f(_lineColor.r/255.,_lineColor.g/255.,_lineColor.b/255.,_lineColor.a/255.);
    glBegin(GL_LINES);
    glVertex3f(x, _offsetY, 0.0);
    glVertex3f(x, winHeight + _offsetY, 0);
    glEnd();
}

//--------------------------------------------------------------

float SoundLine::getSound(float xMod)
{
    if(_bPlay)
    {

        /*
        float sawFreq = maxiMap::linexp(lineX,0, _windowWidth, 1000, 15000);
        float w = sawOsc.saw(sawFreq);

        w = dist.atanDist(w, 10);

        float filtFreq = maxiMap::linexp(lineX,0, _windowHeight, 1000, 15000);
        w = filt.hires(w, filtFreq, 0.4);
*/
        // w = 1000 * sin(i*400);//maxiMap::linexp(lineX,0, _windowWidth, 1000, 15000);

        // float w = sawOsc.pulse(55,0.6);
        float sound = _oscSound.sinewave(440+440*xMod);
       // sound += 0.1*_oscSound.sinewave(261*ofGetMouseY());

        //             w = sawOsc.pulse(110+w,0.2);//it's a pulse wave at 110hz with LFO modulation on the frequency, and width of 0.2
        //float adsrOut = env.adsr(w,1000,1000,1000,1000);
             float adsrOut = _envSound.adsr(sound, 1, 1, 1, 1, 1, _envSound.trigger);

        //  w = filt.lores(w,adsrOut*10000,10);
      //  sound *= adsrOut;

        _envSound.trigger = 1;

        return adsrOut;
    }
    return 0.;
}
