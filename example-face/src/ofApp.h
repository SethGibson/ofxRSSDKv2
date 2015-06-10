#ifndef __OFAPP_H__
#define __OFAPP_H__

#include "ofxRSSDKv2.h"

using namespace ofxRSSDK;

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();

private:
	RSDevicePtr mRSSDK;
	ofTexture	mTexRgb;
};
#endif