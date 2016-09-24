#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::reset()
{
   // _lineNb.set(50);
    _holes.clear();

    _currHole.pos = ofPoint(0,0);
    _currHole.radius = 0;
}
//--------------------------------------------------------------
void ofApp::setupGui()
{
    _gui.setup("Gui");
    _gui.setPosition(ofGetWidth()/2 , 0);

    // add reset button
    _gui.add(_reset.setup("Reset"));//_reset.setup(_nw.getSceneNode(),"reset",false));
    _reset.addListener(this,&ofApp::reset);


    // line number
    _gui.add(_lineNb.set("lineNumber",50,1,200));

    // add bloody button
    _gui.add(_bloody.set("Bloody",false));//_reset.setup(_nw.getSceneNode(),"reset",false));

}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(255);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(100);
    //hole
    _currHole.pos = ofPoint(0,0);
    _currHole.radius = 0;

    // setting up Gui
    setupGui();

    // load shader

    _shader.load("shader");
}

//--------------------------------------------------------------
void ofApp::update()
{
    //update current hole
    if(_currHole.radius)
    {
        if(_holes.size() %2==0)
        {
            _currHole.radius += pow(0.5,_currHole.radius/2)*(_currHole.radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);
        }
        else
        {
            _currHole.radius += 0.2;
        }
        //_currHole.radius += 0.1/sqrt(_currHole.radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);
        //_currHole.radius += pow(0.5,_currHole.radius/2)*(_currHole.radius);//0.2;//10*(ofGetElapsedTimef() - _currHole.radius);

    }
}

//--------------------------------------------------------------
ofColor ofApp::getLineColor(float x)
{
    // check first for current hole
    if ( _currHole.pos.x - _currHole.radius < x
         && _currHole.pos.x + _currHole.radius > x)
    {
        return ofColor(255,0,0);
    }

    // check other holes
    for (auto h : _holes)
    {
        if ( h.pos.x- h.radius < x
             && h.pos.x + h.radius > x)
        {
            return ofColor(255,0,0);
        }
    }

    return ofColor(0);//255);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //draw lines
    ofPushStyle();
    //ofSetColor(255);

    float lineX = 0;
    float stepX = (float) ofGetWindowWidth() / _lineNb ;
    ofColor col(255);

    for (int i = 0 ; i < _lineNb ; i++)
    {
        col = getLineColor(lineX);
        ofSetColor(col);
        if(col != ofColor(0))
        {
            _shader.begin();

            _shader.setUniform1f("time", ofGetElapsedTimef());
        }

        ofDrawLine(lineX,0,lineX,ofGetWindowHeight());

        if(col != ofColor(0))
        {
            _shader.end();
        }

        lineX += stepX;
    }

    ofPopStyle();

    //draw holes
    ofPushStyle();
    ofSetColor(255,0,0);
    if(!_bloody)    ofNoFill();
    //ofBackgroundGradient(ofColor(0),ofColor(255,0,0),OF_GRADIENT_CIRCULAR);

    for (auto h : _holes)
    {

        ofDrawCircle(h.pos,h.radius);
    }

    if(_currHole.radius)
    {
        ofDrawCircle(_currHole.pos,_currHole.radius);
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
    if(abs(_currHole.pos.x - x) < err && abs(_currHole.pos.y - y) < err)
    {
        //same hole, radius augments in the update()
    }
    else
    {
        // create a new hole, adding the previous to the hole vector
        _holes.push_back(_currHole);
        _currHole.pos = ofPoint(x,y);
        _currHole.radius = 0.02;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    _currHole.pos = ofPoint(x,y);
    _currHole.radius = 0.01;//ofGetElapsedTimef();
    //_holes.push_back(ofPoint(x,y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    //_currHole.radius = ofGetElapsedTimef() - _currHole.radius;
    //_currHole.radius *= 10;
    ofLog() << "hole radius " << _currHole.radius;

    _holes.push_back(_currHole);

    _currHole.radius = 0;

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
