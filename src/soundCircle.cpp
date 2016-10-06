#include "soundCircle.h"

//--------------------------------------------------------------

SoundCircle::SoundCircle ()
    : _position(ofPoint(0,0)),
      _radius(0.),
      _soundDuration(50),
      _dashWidth(3),
      _bErase(false),
      _bReady(false)
{

    _envSound.setAttack(1);
    _envSound.setDecay(10);
    _envSound.setSustain(1000);
    _envSound.setRelease(5000);

    _envSound.holdcount=0;
    _envSound.decayphase=0;
    _envSound.sustainphase=0;
    _envSound.releasephase=0;
    _envSound.attackphase=1;
    _envSound.amplitude = 1.;
}
//--------------------------------------------------------------

void SoundCircle::reset(int soundDur)
{

    _bReady = false;
    setup(ofPoint(0,0),0.,soundDur);


}

//--------------------------------------------------------------

void SoundCircle::setup(ofPoint pos, float rad, int soundDur)
{
    _position = pos;
    _radius = rad;
    _oscSound.phaseReset(0);
    _soundDuration = soundDur;
    _bErase = false;


    _envSound.setAttack(1);
    _envSound.setDecay(10);
    _envSound.setSustain(1000);
    _envSound.setRelease(5000);

    _envSound.holdcount=0;
    _envSound.decayphase=0;
    _envSound.sustainphase=0;
    _envSound.releasephase=0;
    _envSound.attackphase=1;
    _envSound.amplitude = 1.;
    _triggered = 1;
}

//--------------------------------------------------------------

void SoundCircle::update(float dt)
{
    // decrease sound duration
    if(_soundDuration > 0)
        _soundDuration -= dt;
}

//--------------------------------------------------------------

void SoundCircle::updateRadius(float speed, bool bLimit)
{
    if(!_radius)
        return;

    if(bLimit)
    {
        _radius += speed*pow(0.5,_radius/2)*(_radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);
    }
    else
    {

        _radius += speed*(_radius>50?-1:1)*0.2;
    }
    _envSound.setRelease(1000+2000*_radius);

}
//--------------------------------------------------------------

void SoundCircle::drawDashedCircle()
{

    float radiusDash = _radius*(1./_soundDuration + 2.);
    ofPushMatrix();

    ofTranslate(_position);

    // setting up dash style
    glLineStipple(3, (GLushort)0x0C0F);//(short) 0x0C0F);
    glEnable(GL_LINE_STIPPLE);

    glLineWidth(_dashWidth);

    glColor3f(_dashCircleColor.r/255., _dashCircleColor.g/255., _dashCircleColor.b/255.);

    glBegin(GL_LINE_LOOP);

    for (int i=0; i < 360; i++)
    {
        float degInRad = i*2*PI/360;
        glVertex2f(cos(degInRad)*radiusDash,sin(degInRad)*radiusDash);
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);

    ofPopMatrix();

}

//--------------------------------------------------------------

void SoundCircle::drawSoundCircle()
{
    if(!_radius)
        return;
    //ofPushStyle();
    ofSetLineWidth(2);
    ofFill();

    ofSetColor(_circleColor);

    //draw filled circle
    ofDrawCircle(_position,_radius);

    // if there is sound, draz dashed circle around the soundcircle
    if(_soundDuration)
    {
        drawDashedCircle();
    }
    //ofPopStyle();

}
//--------------------------------------------------------------

float SoundCircle::getSound()
{
    if(_soundDuration > 0 && _radius != 0)
    {
        // float w = sawOsc.pulse(55,0.6);
        float modR = _radius/4;//150 + _radius * 100;
        float freq = 100;//220;

        float pulse = _oscSound.sinewave(freq + freq*modR);//440+440*0.2);
        //pulse += _radius/20*_oscSound.noise();
       // _filterSound.lopass(pulse,_position.x/2000);

       /* float soundSin = _oscSound.sinewave(82+_position.x+0.2);*/



       // soundSin += modR*_oscSound.sinewave(65+_position.y);
      //  soundSin += 2*modR*_oscSound.sinewave(32+0.2);

       // soundSin += _oscSound.sinewave(65*modR+0.2);
        //soundSin += _oscSound.saw(_position.x);
        //  soundSin += _oscSound.sinewave(97*modY+0.5);
//        float pulse = _oscSound.pulse(soundSin,0.6);
        //             w = sawOsc.pulse(110+w,0.2);//it's a pulse wave at 110hz with LFO modulation on the frequency, and width of 0.2
        //float adsrOut = env.adsr(w,1000,1000,1000,1000);

        float adsrOut = _envSound.adsr(pulse, _triggered);

        _triggered = 0;

        //  w = filt.lores(w,adsrOut*10000,10);
      //  pulse *= adsrOut;

        //w = sawOsc.pulse(_freqMod.get()+w,0.2);//it's a pulse wave at 110hz with LFO modulation on the frequency, and width of 0.2
        return adsrOut;
    }
    return 0.;
}
//--------------------------------------------------------------

bool SoundCircle::isTouchingLine(float x)
{
    if ( _position.x - _radius < x
         && _position.x + _radius > x)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------

bool SoundCircle::isSameCircle(float x, float y, float err)
{

    float xx = (_position.x - x) * (_position.x - x) ;
    float yy = (_position.y - y) * (_position.y - y) ;
    float dist = sqrt(xx + yy );

    if(dist < err)//if(abs(_position.x - x) < err && abs(_position.y - y) < err)
    {
       return true;
    }
    return false;
}

