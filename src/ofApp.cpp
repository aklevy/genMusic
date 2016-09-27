#include "ofApp.h"
#include <boost/range/algorithm_ext/erase.hpp>

//--------------------------------------------------------------
ofApp::~ofApp()
{
    _reset.removeListener(this,&ofApp::reset);
    _lineNb.removeListener(this,&ofApp::lineNbModified);

}

//--------------------------------------------------------------
void ofApp::reset()
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
void ofApp::lineNbModified(int& newval)
{
    lock lM(_lineMutex);
    int oldSize = _vecSoundLines.size();
    if(oldSize != newval)
        _vecSoundLines.resize(newval);

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
    _gui.add(_reset.setup("Reset"));//_reset.setup(_nw.getSceneNode(),"reset",false));
    _reset.addListener(this,&ofApp::reset);


    /*
     *  Line Parameters Group
     */
    _lineParameters.setName("lineParameters");

    // line number
    _lineParameters.add(_lineNb.set("lineNumber",40,1,200));
    _lineNb.addListener(this,&ofApp::lineNbModified);

    // line width
    _lineParameters.add(_lineWidth.set("lineWidth",5,0,10));

    // line amplitude
    _lineParameters.add(_lineAmplitude.set("lineAmplitude",200,1,500));

    // line frequence
    _lineParameters.add(_lineFrequence.set("lineFrequence",10,1,20));

    // line colors
    _lineParameters.add(_lineDefaultColor.set
                        ("lineDefaultColor",
                         ofColor(255,255,255,255),
                         ofColor(0,0,0,0),
                         ofColor(255,255,255,255)));
    _lineDefaultColor.addListener(this,&ofApp::lineDefColModified);

    _lineParameters.add(_lineMovingColor.set
                        ("lineMovingColor",
                         ofColor(0,50,0,255),
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
    _circleParameters.add(_drawAllCircles.set("historic",false));//_reset.setup(_nw.getSceneNode(),"reset",false));


    /*
     *  Sound Parameters Group
     */
    _soundParameters.setName("soundParameters");
    _soundParameters.add(_freqMod.set("frequence",0.5,0,10));
    _soundParameters.add(_volumeLine.set("volumeLine",3.,0,5));
    _soundParameters.add(_volumeCircle.set("volumeCircle",0.3,0,5));


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
    bool bLimit = (_vecSoundCircles.size()%2 == 0 );
    _currCircle.updateRadius(_circleGrowingSpeed.get(), bLimit);

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

    lock l(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {
        if(!l.isMoving())
        {
            l.setMove(isLineMoving(lineX));
        }

        if(l.isMoving())
        {
            offsetY =(int)_lineAmplitude *sin(sqrt(_lineFrequence)*lineX/_windowWidth*ofGetElapsedTimef());
        }

        l.update(lineX,offsetY);

        // move along the x axis by stepX
        lineX += (int)stepX;
    }
}

//--------------------------------------------------------------
bool ofApp::isLineMoving(float x)
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
void ofApp::draw()
{
    //draw lines
    ofPushStyle();

    // ofSetLineWidth();
    int lineX = 0;
    float stepX = (float) _windowWidth / _lineNb ;

    lock l(_lineMutex);
    for(SoundLine& l : _vecSoundLines)
    {
        l.drawSoundLine(lineX,_windowHeight,_lineWidth);

        lineX += stepX;
    }


    ofPopStyle();

    /*
     * Draw circles
     * */

    // if there are no new circles, draw fbo texture
    //ofTexture fboTex = _fbo.getTexture();


    if(_vecSoundCircles.size() > 0 && _fbo.isAllocated())
        // draw the new circles in the texture
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

        //  _fbo.draw(0,0);

    }
    if(_fbo.isAllocated() && _drawAllCircles)
        _fbo.draw(0,0);
    /*_fbo.getTexture().draw(ofPoint(0,0,0),
                               ofPoint(_windowWidth,0,0),
                               ofPoint(0,_windowHeight,0),
                              ofPoint(_windowWidth,_windowHeight,0));
   */
    // draw playing sound
    ofPushStyle();

    lock c(_circleMutex);
    for (SoundCircle& c : _vecSoundCircles)
    {
        if(c.getSoundDuration())
            c.drawSoundCircle();
    }

    _currCircle.drawSoundCircle();

    ofPopStyle();

    // drawing gui
    _gui.draw();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{

    for(int i = 0 ; i < bufferSize * nChannels; i++) output[i] = 0;


    for (int i = 0; i < bufferSize; i++){

        // get lines's sound
        int lineX = 0;
        float stepX = (float) _windowWidth / _lineNb ;

        lock l(_lineMutex);
        for(SoundLine& l : _vecSoundLines)
        {
            float soundL = _volumeLine * l.getSound((float)lineX/_windowWidth);
            if(soundL )
            {
                output[i * nChannels] += soundL;
                output[i * nChannels +1] += soundL;
            }
            lineX += stepX;
        }

        //get circles' sound

        lock lc(_circleMutex);
        for(SoundCircle& c : _vecSoundCircles)
        {
            float soundC = _volumeCircle * c.getSound();
            if(soundC)
            {
                output[i * nChannels] += soundC ;
                output[i * nChannels +1] += soundC ;
            }
        }

        // get current circle's sound
        float soundCurr = _volumeCircle * _currCircle.getSound();
        if(soundCurr)
        {
            output[i * nChannels] += soundCurr;
            output[i * nChannels +1] += soundCurr;
        }
    }


    int n = _vecSoundLines.size() + _vecSoundCircles.size() + 1;//+5; //get playing circle number
    if(n > 1)
        for(int i = 0 ; i < bufferSize * nChannels; i++) output[i] /= n;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
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
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    ofLog() << _vecSoundCircles.size();
    /*_currCircle.pos = ofPoint(x,y);
    _currCircle.radius = 0.01;//ofGetElapsedTimef();

*/
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
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

