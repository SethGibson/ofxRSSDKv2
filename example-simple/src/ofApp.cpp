#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowShape(1280, 480);
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
		mTexDepth.allocate(mRSSDK->getDepthWidth(), mRSSDK->getDepthHeight(), GL_RGBA);
		mRSSDK->start();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	mRSSDK->update();
	mTexRgb.loadData(mRSSDK->getRgbFrame());
	mTexDepth.loadData(mRSSDK->getDepth8uFrame());
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(ofColor::black);
	mTexRgb.draw(0, 0);
	ofDrawBitmapString("Raw Color", ofPoint(10, 20));
	ofPushMatrix();
	ofTranslate(640, 0);
	mTexDepth.draw(0,0,640,480);
	ofDrawBitmapString("Depth Pixels", ofPoint(10, 20));
	ofPopMatrix();
}


void ofApp::exit()
{
	mRSSDK->stop();
}
