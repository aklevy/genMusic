#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp()
{
    _reset.removeListener(this,&ofApp::reset);
}

//--------------------------------------------------------------
void ofApp::reset()
{
   // _lineNb.set(50);
    _circles.clear();

    _currCircle.pos = ofPoint(0,0);
    _currCircle.radius = 0;
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
    //hole
    _currCircle.pos = ofPoint(0,0);
    _currCircle.radius = 0;

    // setting up Gui
    setupGui();

    // load shader

    //_shader.load("shader");
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
}

//--------------------------------------------------------------
ofColor ofApp::getLineColor(float x)
{
    // check first for current hole
    if ( _currCircle.pos.x - _currCircle.radius < x
         && _currCircle.pos.x + _currCircle.radius > x)
    {
        return _lineMovingColor;
    }

    // check other holes
    for (auto h : _circles)
    {
        if ( h.pos.x- h.radius < x
             && h.pos.x + h.radius > x)
        {
            return _lineMovingColor;
        }
    }

    return _lineDefaultColor;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //draw lines
    ofPushStyle();

    ofSetLineWidth(_lineWidth);
    //ofSetColor(255);

    int lineX = 0;
    float stepX = (float) ofGetWindowWidth() / _lineNb ;
    ofColor col(_lineDefaultColor.get());

    for (int i = 0 ; i < _lineNb ; i++)
    {
        ofPushMatrix();

        col = getLineColor(lineX);
        ofSetColor(col);
        if(col.g != 0)
        {
           ofTranslate(0,_lineAmplitude *sin(_lineFrequence*lineX/ofGetWindowWidth()*ofGetElapsedTimef()),0);
        }

        ofDrawLine(lineX,0,lineX,ofGetWindowHeight());


        lineX += (int)stepX;

        ofPopMatrix();
    }

    ofPopStyle();

    //draw holes
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
    ofLog() << "hole radius " << _currCircle.radius;

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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
