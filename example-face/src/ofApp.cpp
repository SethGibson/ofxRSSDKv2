#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowShape(640, 480);
	mRSSDK = RSDevice::createUniquePtr();	
	
	if(!mRSSDK->init())
	{
		ofLogError("Unable to create ofxRSSDK object");
		exit();
	}
	else
	{
		mRSSDK->initDepth(DepthRes::R200_SD, 30, true);
		mRSSDK->initRgb(RGBRes::VGA, 30);
		mTexRgb.allocate(mRSSDK->getRgbWidth(), mRSSDK->getRgbHeight(),GL_RGBA);
		mRSSDK->start();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	mRSSDK->update();
	mTexRgb.loadData(mRSSDK->getRgbFrame(), GL_BGRA);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(ofColor::black);
	mTexRgb.draw(0, 0);
	ofDrawBitmapString("Raw Color", ofPoint(10, 20));
}


void ofApp::exit()
{
	mRSSDK->stop();
}
