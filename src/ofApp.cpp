#include "ofApp.h"

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
    _circles.clear();

    _currCircle.pos = ofPoint(0,0);
    _currCircle.radius = 0;

    // reset line vector
    for(auto& l : _linesSound)
    {
        l.bPlay = false;
        l.bMove = false;
        l.offsetY = 0;

        l.color = _lineDefaultColor;
    }

}
//--------------------------------------------------------------
void ofApp::lineNbModified(int& newval)
{
    int oldSize = _linesSound.size();
    if(oldSize != newval)
        _linesSound.resize(newval);

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
                         ofColor(0,0,50,255),
                         ofColor(0,0,0,0),
                         ofColor(255,255,255,255)));
    _lineParameters.add(_lineMovingColor.set
                        ("lineMovingColor",
                         ofColor(0,50,0,255),
                         ofColor(0,0,0,0),
                         ofColor(255,255,255,255)));

    /*
     *  Circle Parameters Group
     */
    _circleParameters.setName("circleParameters");

    // filling circle
    _circleParameters.add(_circleFill.set("fillCircle",true));//_reset.setup(_nw.getSceneNode(),"reset",false));

    // circle default color
    _circleParameters.add(_circleDefaultColor.set
                          ("circleColor",
                           ofColor(255,0,0,255),
                           ofColor(0,0,0,0),
                           ofColor(255,255,255,255)));

    // circle growing speed
    _circleParameters.add(_circleGrowingSpeed.set("circleGrowth",1,0,5));//_reset.setup(_nw.getSceneNode(),"reset",false));


    // add parameters' group to the gui
    _gui.add(_lineParameters);
    _gui.add(_circleParameters);
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

    // circle
    _currCircle.pos = ofPoint(0,0);
    _currCircle.radius = 0;

    // setting up Gui
    setupGui();

    // setting up line sound vector
    line l;
    l.bMove = l.bPlay = false;
    l.offsetY = 0;
    l.color = _lineDefaultColor.get();

    for (int i = 0 ; i < _lineNb ; i++)
    {
        _linesSound.push_back(l);
    }


    // ofxMaxim setup
    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

    //mySample.load(ofToDataPath("sounds/beat.wav"));
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
}

//--------------------------------------------------------------
void ofApp::update()
{
    //update current hole
    if(_currCircle.radius)
    {
        if(_circles.size() %2==0)
        {
            _currCircle.radius += _circleGrowingSpeed*pow(0.5,_currCircle.radius/2)*(_currCircle.radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);
        }
        else
        {

            _currCircle.radius += _circleGrowingSpeed*(_currCircle.radius>30?-1:1)*0.2;
        }
        //_currHole.radius += 0.1/sqrt(_currHole.radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);
        //_currHole.radius += pow(0.5,_currHole.radius/2)*(_currHole.radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);
    }

    // update line structure contained in the vector linesSound
    int lineX = 0;
    float stepX = (float) _windowWidth / _lineNb ;

    for(line& l : _linesSound)
    {
        if(!l.bMove)
            l.bMove = isLineMoving(lineX);

        if(l.bMove)
        {
            l.bMove = true;
            l.offsetY = (int)_lineAmplitude *sin(_lineFrequence*lineX/_windowWidth*ofGetElapsedTimef());
            // play sound ?
            if(abs(l.offsetY) < 10 ) // hitting the upper screen limit
            {
                l.bPlay = true;
                l.color = ofColor(255,0,0);
            }
            else
            {
                l.bPlay = false;
                l.color = _lineMovingColor;
            }
        }
        else
        {
            l.offsetY =  0;
            l.bMove = false;
            l.bPlay = false;
            l.color = _lineDefaultColor;
        }

        // move along the x axis by stepX
        lineX += (int)stepX;

    }
}

//--------------------------------------------------------------
bool ofApp::isLineMoving(float x)
{
    // check first for current hole
    if ( _currCircle.pos.x - _currCircle.radius < x
         && _currCircle.pos.x + _currCircle.radius > x)
    {
        return true;
    }

    // check other holes
    for (auto& h : _circles)
    {
        if ( h.pos.x- h.radius < x
             && h.pos.x + h.radius > x)
        {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //draw lines
    ofPushStyle();

    //ofSetLineWidth(_lineWidth);
    int lineX = 0;
    float stepX = (float) _windowWidth / _lineNb ;

    for(line& l : _linesSound)
    {

        glLineWidth(_lineWidth);
        glColor3f(l.color.r/255., l.color.g/255., l.color.b/255.);
        glBegin(GL_LINES);
        glVertex3f(lineX, l.offsetY, 0.0);
        glVertex3f(lineX, _windowHeight + l.offsetY, 0);
        glEnd();
       /*
          ofSetColor(l.color);
        ofDrawLine(lineX, l.offsetY , lineX, h + l.offsetY);
        */
        lineX += stepX;
    }


    ofPopStyle();

    //draw circles
    ofPushStyle();
    ofEnableSmoothing();

    ofSetColor(_circleDefaultColor);
    ofSetLineWidth(2);
    if(!_circleFill)    ofNoFill();
    //ofBackgroundGradient(ofColor(0),ofColor(255,0,0),OF_GRADIENT_CIRCULAR);

    for (auto h : _circles)
    {

        //   ofFill();
        ofDrawCircle(h.pos,h.radius);
        // ofNoFill();
        //ofDrawCircle(h.pos,h.radius);

    }

    if(_currCircle.radius)
    {
        //ofFill();
        ofDrawCircle(_currCircle.pos,_currCircle.radius);
        //ofNoFill();
        //ofDrawCircle(_currHole.pos,_currHole.radius);

    }

    ofPopStyle();

    // drawing gui
    _gui.draw();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{


    for (int i = 0; i < bufferSize; i++){


        //make 'wave' equal something noisy
        //if(_sound)
        int lineX = 0;
        //int lineOffsetY = 0;
        float stepX = (float) _windowWidth / _lineNb ;


        for(line l : _linesSound)
        {
            if(l.bPlay)
            {


                float sawFreq = maxiMap::linexp(lineX,0, _windowWidth, 1000, 15000);
                float w = sawOsc.saw(sawFreq);

                w = dist.atanDist(w, 10);

                float filtFreq = maxiMap::linexp(lineX,0, _windowHeight, 1000, 15000);
                w = filt.hires(w, filtFreq, 0.4);


                output[i] += w;
                output[i+1] += w;
            }
            lineX += stepX;
        }
        //double wave = mySample.play(0.5);


        //          output[i*nChannels    ] = wave; /* You may end up with lots of outputs. add them here */
        //        output[i*nChannels + 1] = wave;


    }


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
    float err = 5;
    if(abs(_currCircle.pos.x - x) < err && abs(_currCircle.pos.y - y) < err)
    {
        //same hole, radius augments in the update()
    }
    else
    {
        // create a new hole, adding the previous to the hole vector
        _circles.push_back(_currCircle);
        _currCircle.pos = ofPoint((int)x,(int)y);
        _currCircle.radius = 0.02;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    _currCircle.pos = ofPoint(x,y);
    _currCircle.radius = 0.01;//ofGetElapsedTimef();
    //_holes.push_back(ofPoint(x,y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    //_currHole.radius = ofGetElapsedTimef() - _currHole.radius;
    //_currHole.radius *= 10;
   // ofLog() << "hole radius " << _currCircle.radius;

    _circles.push_back(_currCircle);

    _currCircle.radius = 0;

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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
