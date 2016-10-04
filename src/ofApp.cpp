#include "ofApp.h"
#include <boost/range/algorithm_ext/erase.hpp>

//--------------------------------------------------------------
ofApp::~ofApp()
{
    _lineDefaultColor.removeListener(this,&ofApp::lineDefColModified);
    _lineMovingColor.removeListener(this,&ofApp::lineMovColModified);
    _circleDefaultColor.removeListener(this,&ofApp::circleColModified);
    _currentCircleColor.removeListener(this,&ofApp::currentCircleColModified);

}
//--------------------------------------------------------------
ofApp::ofApp():
    _nw("kaleidoSound","i-score","127.0.0.1",13579, 9998)
{}

//--------------------------------------------------------------
void ofApp::reset(bool &newval)
{
    // _lineNb.set(50);
    _vecSoundCircles.clear();

    _currCircle.reset(50);
    //  _currCircle.setColor(_currentCircleColor.get());
    //  _currCircle.setDashColor(_currentCircleColor.get());

    // reset line vector
    lock lM(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {
        l.reset();
    }

    if(_fbo.isAllocated())
    {
        _fbo.begin();
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        _fbo.end();
    }
}
//--------------------------------------------------------------
void ofApp::changeInput(bool& newval)
{
    // add the current circle to the circle vector
    _currCircle.setColor(_circleDefaultColor.get());
    _currCircle.setDashColor(_circleDefaultColor.get());
    lock lc(_circleMutex);
    _vecSoundCircles.push_back(_currCircle);
    _currCircle.reset(50);

    /*  if(newval) // change to hot hand input
    {}
    else // change to mouse input
    {}*/
}


//--------------------------------------------------------------
void ofApp::lineNbModified(int& newval)
{
    int oldSize = _vecSoundLines.size();
    lock lM(_lineMutex);
    if(oldSize != newval)
    {
        _vecSoundLines.resize(newval);
        //  lock lM(_lineMutex);

        if(oldSize < newval)
        {
            for(SoundLine& l : _vecSoundLines)
            {
                l.setDefaultColor(_lineDefaultColor);
                l.setMovingColor(_lineMovingColor);
            }

        }
    }


}

//--------------------------------------------------------------
void ofApp::lineDefColModified(ofColor& newval)
{
    lock lM(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {
        l.setDefaultColor(newval);
    }
}
//--------------------------------------------------------------
void ofApp::lineMovColModified(ofColor& newval)
{
    lock lM(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {
        l.setMovingColor(newval);
    }
}

//--------------------------------------------------------------
void ofApp::circleColModified(ofColor& newval)
{
    lock cM(_circleMutex);
    for(SoundCircle& c : _vecSoundCircles)
    {
        c.setColor(newval);
        c.setDashColor(newval);
    }
}
//--------------------------------------------------------------
void ofApp::currentCircleColModified(ofColor& newval)
{
    _currCircle.setColor(newval);
    _currCircle.setDashColor(newval);
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
    _gui.setup("Gui");
    _gui.setPosition(0 , 0);

    // add reset button
    _gui.add(_reset.setup(_nw.getSceneNode(),"reset",false));
    _reset.addListener(this,&ofApp::reset);

    // add input choosing button
    _gui.add(_inputHotHand.setup(_nw.getSceneNode(),"InputHotHand",false));//set("InputHotHand",false));
    _inputHotHand.addListener(this,&ofApp::changeInput);

    // adding the input from the hot hand
    //_gui.add(_valueHotHand.set("valueHotHand",ofVec3f(0),ofVec3f(0),ofVec3f(1)));
    _gui.add(_valueHotHand.setup(_nw.getSceneNode(),"valueHotHand",ofVec3f(0),ofVec3f(0),ofVec3f(1)));
    //_valueHotHand.addListener(this,&ofApp::inputFromHotHand);

    _gui.add(_valueHotHandX.setup(_nw.getSceneNode(),"valueHotHandX",0.,0,1));
    _valueHotHandX.addListener(this,&ofApp::inputFromHotHandX);
    _gui.add(_valueHotHandY.setup(_nw.getSceneNode(),"valueHotHandY",0.,0,1));
    _valueHotHandY.addListener(this,&ofApp::inputFromHotHandY);

    // separation Line x
    _gui.add(_sepLineX.setup(_nw.getSceneNode(),"SeparationLine",0.75,0.01,0.99));
    // separation Line x
    _gui.add(_sepLineWidth.setup(_nw.getSceneNode(),"SeparationWidth",20,0.1,50));


    /*
     *  Line Parameters Group
     */
    _lineParameters.setName("lineParameters");

    // line number
    _lineParameters.add(_lineNb.setup(_nw.getSceneNode(),"lineNumber",40,1,200));
    _lineNb.addListener(this,&ofApp::lineNbModified);

    // line width
    _lineParameters.add(_lineWidth.setup(_nw.getSceneNode(),"lineWidth",5,0.1,10));

    // line amplitude
    _lineParameters.add(_lineAmplitude.setup(_nw.getSceneNode(),"lineAmplitude",200,1,500));

    // line frequence
    _lineParameters.add(_lineFrequence.setup(_nw.getSceneNode(),"lineFrequence",10,1,20));

    // line colors
    _lineParameters.add(_lineDefaultColor.set
                        ("lineDefaultColor",
                         ofColor(255,255,255,255),
                         ofColor(0,0,0,0),
                         ofColor(255,255,255,255)));
    _lineDefaultColor.addListener(this,&ofApp::lineDefColModified);

    _lineParameters.add(_lineMovingColor.set
                        ("lineMovingColor",
                         ofColor(0,213,135,255),
                         ofColor(0,0,0,0),
                         ofColor(255,255,255,255)));
    _lineMovingColor.addListener(this,&ofApp::lineMovColModified);

    /*
     *  Circle Parameters Group
     */
    _circleParameters.setName("circleParameters");

    // filling circle
    // _circleParameters.add(_circleFill.set("fillCircle",true));//_reset.setup(_nw.getSceneNode(),"reset",false));

    // circle default color
    _circleParameters.add(_circleDefaultColor.set
                          ("circleColor",
                           ofColor(255,0,0,255),
                           ofColor(0,0,0,0),
                           ofColor(255,255,255,255)));
    _circleDefaultColor.addListener(this,&ofApp::circleColModified);


    // current circle color
    _circleParameters.add(_currentCircleColor.set
                          ("currentCircleColor",
                           ofColor(254,226,62,255),
                           ofColor(0,0,0,0),
                           ofColor(255,255,255,255)));
    _currentCircleColor.addListener(this,&ofApp::currentCircleColModified);

    // circle growing speed
    _circleParameters.add(_circleGrowingSpeed.set("circleGrowth",1,0,5));//_reset.setup(_nw.getSceneNode(),"reset",false));

    // draw all the circles (even if they do not emit any sound)
    _circleParameters.add(_drawAllCircles.set("history",false));//_reset.setup(_nw.getSceneNode(),"reset",false));


    /*
     *  Sound Parameters Group
     */
    _soundParameters.setName("soundParameters");
    _soundParameters.add(_freqMod.set("frequence",0.5,0,10));
    _soundParameters.add(_volumeLine.set("volumeLine",1.,0,1));
    _soundParameters.add(_volumeCircle.set("volumeCircle",0.2,0,1.));


    // add parameters' group to the gui
    _gui.add(_lineParameters);
    _gui.add(_circleParameters);
    _gui.add(_soundParameters);

    _gui.minimizeAll();

}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(255);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(100);
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    // window size
    _windowWidth = ofGetWindowWidth();
    _windowHeight = ofGetWindowHeight();

    //separation line default value
    _sepLineSelected = false;

    // allocate fbo
    if(!_fbo.isAllocated())
    {
        // allocate fbo
        _fbo.allocate(_windowWidth , _windowHeight,GL_RGBA);
    }

    // circle
    _currCircle.setup(ofPoint(0),0,50);

    // setting up Gui
    setupGui();

    // setting up line sound vector
    SoundLine l;
    l.setDefaultColor(_lineDefaultColor.get());
    l.setMovingColor(_lineMovingColor.get());

    lock lM(_lineMutex);
    for (int i = 0 ; i < _lineNb ; i++)
    {
        _vecSoundLines.push_back(l);
    }


    // ofxMaxim setup
    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
}

//--------------------------------------------------------------
void ofApp::update()
{
    //update current circle's radius using growing speed
    if(_currCircle.isReady())
    {
        bool bLimit = (_vecSoundCircles.size()%2 == 0 );
        _currCircle.updateRadius(_circleGrowingSpeed.get(), bLimit);
    }
    // erase unplaying sound circle
    if(_vecSoundCircles.size())
    {
        lock lc(_circleMutex);
        boost::range::remove_erase_if(_vecSoundCircles,
                                      [] (SoundCircle& sc) {
            return sc.isToBeRemoved();

        });
    }


    // update circle lifetime
    for(SoundCircle& c : _vecSoundCircles)
    {
        c.update(1);
    }


    // update line structure contained in the vector linesSound
    int lineX = 0;
    int offsetY = 0;
    float stepX = (float) _windowWidth / _lineNb ;
    float sepLineX = (float)_windowWidth*_sepLineX;

    lock l(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {
        if(!l.isMoving())
        {
            l.setMove(isLineTouchingCircle(lineX));
        }

        if(l.isMoving())
        {
            offsetY =(int)_lineAmplitude *sin(sqrt(_lineFrequence)*lineX/_windowWidth*ofGetElapsedTimef());

            if( lineX > sepLineX )// draw square
            {
                offsetY *= 3;
            }

        }

        l.update(lineX,offsetY);

        // move along the x axis by stepX
        lineX += (int)stepX;
    }
}

//--------------------------------------------------------------
bool ofApp::isLineTouchingCircle(float x)
{
    // check first for current hole
    if(_currCircle.isTouchingLine(x))
        return true;

    // check other circles
    lock c(_circleMutex);
    for (SoundCircle& c : _vecSoundCircles)
    {
        if(c.isTouchingLine(x))
            return true;
    }

    return false;
}
//--------------------------------------------------------------
void ofApp::drawInFbo()
{
    if(_vecSoundCircles.size() > 0 && _fbo.isAllocated())
    {
        _fbo.begin();

        //ofPushStyle();

        lock c(_circleMutex);
        for (SoundCircle& c : _vecSoundCircles)
        {
            if(c.getSoundDuration() <= 0 && !c.isToBeRemoved())
            {
                c.toBeRemoved();
                c.drawSoundCircle();

            }
        }
        //ofPopStyle();
        _fbo.end();
    }


}
//--------------------------------------------------------------
void ofApp::draw()
{

    // Draw separation line
    ofPushStyle();
    float sepLineX = _windowWidth*_sepLineX;
    ofSetColor(255,255,0);
    ofSetLineWidth(_sepLineWidth);
    ofDrawLine(ofPoint(sepLineX,0),ofPoint(sepLineX,_windowHeight));

    ofSetColor(255,100,0,200);
    ofDrawRectangle(sepLineX,0,0,_windowWidth - sepLineX,_windowHeight);
    ofPopStyle();

    /*
     * Draw lines
     * */

    ofPushStyle();

    // ofSetLineWidth();
    int lineX = 0;
    float stepX = (float) _windowWidth / _lineNb ;

    lock l(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {

        l.drawSoundLine(lineX,_windowHeight,_lineWidth,sepLineX);
        lineX += stepX;
    }


    ofPopStyle();

    /*
     * Draw circles
     * */

    // drawing all the circles in fbo and setting to be removed in update()
    drawInFbo();

    // display the texture in fbo
    if(_fbo.isAllocated() && _drawAllCircles)
        _fbo.draw(0,0);

    // draw every playing sound
    ofPushStyle();

    lock c(_circleMutex);
    for (SoundCircle& c : _vecSoundCircles)
    {
        if(c.getSoundDuration())
            c.drawSoundCircle();
    }

    if(_currCircle.isReady())   _currCircle.drawSoundCircle();

    ofPopStyle();

    /*
     * Draw UI
     * */

    _gui.draw();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
    float vl = _volumeLine.get();
    float vc = _volumeCircle.get();
    float sepLineX = _sepLineX.get();

    for(int i = 0 ; i < bufferSize * nChannels; i++) output[i] = 0;


    for (int i = 0; i < bufferSize; i++){

        // get lines's sound
        int lineX = 0;
        float stepX = (float) _windowWidth / _lineNb ;

        lock l(_lineMutex);
        for(SoundLine& l : _vecSoundLines)
        {
            float soundL =  5 * vl * l.getSound((float)lineX/_windowWidth,sepLineX);
            output[i * nChannels] += soundL;
            output[i * nChannels +1] += soundL;

            lineX += stepX;
        }

        //get circles' sound

        lock lc(_circleMutex);
        for(SoundCircle& c : _vecSoundCircles)
        {
            float soundC = 30 * vc * c.getSound();
            output[i * nChannels] += soundC ;
            output[i * nChannels +1] += soundC ;

        }

        // get current circle's sound
        float soundCurr =  30 * vc * _currCircle.getSound();

        output[i * nChannels] += soundCurr;
        output[i * nChannels +1] += soundCurr;

    }


    int n = _vecSoundLines.size() +1;//+ _vecSoundCircles.size() + 1;//+5; //get playing circle number
    if(n > 1)
        for(int i = 0 ; i < bufferSize * nChannels; i++) output[i] /= n;
}
//--------------------------------------------------------------
void ofApp::inputToCircle(int x, int y, float z)
{
    // getting input : current circle is ready to grown
    _currCircle.setReady(true);

    float err = 10;
    if(_currCircle.isSameCircle(x,y,err) || _vecSoundCircles.size() > 50)
    {
        //same SoundCircle, radius augments in the update()
    }
    else
    {
        // add the previous circle to the circle vector
        _currCircle.setColor(_circleDefaultColor.get());
        _currCircle.setDashColor(_circleDefaultColor.get());
        lock lc(_circleMutex);
        _vecSoundCircles.push_back(_currCircle);

        // "new" current circle
        _currCircle.setup(ofPoint((int)x,(int)y),0.2,50);
        _currCircle.setColor(_currentCircleColor.get());
        _currCircle.setDashColor(_currentCircleColor.get());
    }
}

//--------------------------------------------------------------
void ofApp::inputFromHotHand(ofVec3f& newval)
{

    if(!_inputHotHand)
    {
        return;
    }

    // remapping to the screen dimension
    int x = newval.x * _windowWidth;
    int y = newval.y * _windowHeight;

    inputToCircle(x, y, newval.z );
}
//--------------------------------------------------------------
void ofApp::inputFromHotHandX(float& newval)
{

    if(!_inputHotHand)
    {
        return;
    }

    // remapping to the screen dimension
    int x = newval * _windowWidth;

  // ofVec3f oldVal = _valueHotHand.get();
  // int y = oldVal.y * _windowHeight;

 //  oldVal.x = x;
  //  _valueHotHand.set(oldVal);
    inputToCircle(x, x, 0);//oldVal.z );

}
//--------------------------------------------------------------
void ofApp::inputFromHotHandY(float& newval)
{

    if(!_inputHotHand)
    {
        return;
    }

    // remapping to the screen dimension

    int y = newval * _windowHeight;
   // _inputHotHand.y = y;

    ofVec3f oldVal = _valueHotHand.get();
   // oldVal.y = y;
    // _valueHotHand.set(oldVal);
    int x = oldVal.y * _windowWidth;

  //  oldVal.x = x;
   //  _valueHotHand.set(oldVal);
     inputToCircle(x, y, oldVal.z );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLog ()<< _volumeCircle;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    if(_inputHotHand || _sepLineSelected)
    {
        return;
    }

    // updating current circle (and creating other if necessary)
    // according to the mouse movements
    inputToCircle(x,y);

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if(_sepLineSelected && x > _lineWidth && x < _windowWidth)
    {
        _sepLineX.set((float)x/_windowWidth);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if(_currCircle.isReady())
    {
        // add the current circle to the circle vector
        _currCircle.setColor(_circleDefaultColor.get());
        _currCircle.setDashColor(_circleDefaultColor.get());
        lock lc(_circleMutex);
        _vecSoundCircles.push_back(_currCircle);
        _currCircle.reset(50);
    }

    float xx = _sepLineX*_windowWidth - x;
    // pressed on the separation line
    if(abs(xx) < _sepLineWidth)
    {
        _sepLineSelected = true;
    }
    else
    {
        _sepLineSelected = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    _sepLineSelected = false;

    //_currHole.radius = ofGetElapsedTimef() - _currHole.radius;
    //_currHole.radius *= 10;
    // ofLog() << "hole radius " << _currCircle.radius;

    /*_circles.push_back(_currCircle);

    _currCircle.radius = 0;
*/
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    _windowWidth = w;
    _windowHeight = h;
}

