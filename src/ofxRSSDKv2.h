#ifndef __OFX_RSSDK_H__
#define __OFX_RSSDK_H__
#ifdef _DEBUG
#pragma comment(lib, "libpxcmd_d.lib")
#else
#pragma comment(lib, "libpxcmd.lib")
#endif
#include <memory>
#include "ofMain.h"
#include "pxcsensemanager.h"
#include "pxcprojection.h"

using namespace std;

namespace ofxRSSDK
{
	class RSDevice;
	typedef unique_ptr<RSDevice> RSDevicePtr;
	typedef shared_ptr<RSDevice> RSDeviceRef;

	enum DepthRes
	{
		R200_SD,	// 480x360
		R200_VGA,	// 628x468
		F200_VGA,	// 640x480 
		QVGA		// 320x240
	};

	enum RGBRes
	{
		VGA,
		HD720,
		HD1080
	};

	enum AlignMode
	{
		ALIGN_FRAME,
		ALIGN_UVS_ONLY
	};

	enum CloudRes
	{
		FULL_RES=1,
		HALF_RES=2,
		Q_RES=4
	};

	class RSDevice
	{
	protected:
		RSDevice();
	public:
		~RSDevice();
		static RSDevicePtr createUniquePtr() { return RSDevicePtr(new RSDevice()); }
		static RSDeviceRef createSharedPtr() { return RSDeviceRef(new RSDevice()); }
		bool init();
		bool initRgb(const RGBRes& pSize, const float& pFPS);
		bool initDepth(const DepthRes& pSize, const float& pFPS, bool pAsColor);
		
		void enableAlignedImages(bool pState = true, AlignMode pMode = AlignMode::ALIGN_UVS_ONLY) { mShouldAlign = pState; mAlignMode = pMode; }
		void enablePointCloud(CloudRes pCloudRes) { mCloudRes=pCloudRes; mShouldGetPointCloud=true;}
		void setPointCloudRange(float pMin, float pMax);

		bool start();
		bool update();
		bool stop();

		const ofPixels&	getRgbFrame();
		const ofShortPixels&	getDepthFrame();
		const ofPixels&	getDepth8uFrame();
		const ofPixels&	getColorMappedToDepthFrame();
		const ofPixels&	getDepthMappedToColorFrame();
		vector<ofVec3f> getPointCloud();
		//Nomenclature Notes:
		//	"Space" denotes a 3d coordinate
		//	"Image" denotes an image space point ((0, width), (0,height), (image depth))
		//	"Coords" denotes texture space (U,V) coordinates
		//  "Frame" denotes a full Surface

		//get a camera space point from a depth image point
		const ofPoint		getDepthSpacePoint(float pImageX, float pImageY, float pImageZ);
		const ofPoint		getDepthSpacePoint(int pImageX, int pImageY, uint16_t pImageZ);
		const ofPoint		getDepthSpacePoint(ofPoint pImageCoords);

		//get a ofColor object from a depth image point
		const ofColor		getColorFromDepthImage(float pImageX, float pImageY, float pImageZ);
		const ofColor		getColorFromDepthImage(int pImageX, int pImageY, uint16_t pImageZ);
		const ofColor		getColorFromDepthImage(ofPoint pImageCoords);

		//get a ofColor object from a depth camera space point
		const ofColor		getColorFromDepthSpace(float pCameraX, float pCameraY, float pCameraZ);
		const ofColor		getColorFromDepthSpace(ofPoint pCameraPoint);

		//get ofColor space UVs from a depth image point
		const ofVec2f		getColorCoordsFromDepthImage(float pImageX, float pImageY, float pImageZ);
		const ofVec2f		getColorCoordsFromDepthImage(int pImageX, int pImageY, uint16_t pImageZ);
		const ofVec2f		getColorCoordsFromDepthImage(ofPoint pImageCoords);

		//get ofColor space UVs from a depth space point
		const ofVec2f		getColorCoordsFromDepthSpace(float pCameraX, float pCameraY, float pCameraZ);
		const ofVec2f		getColorCoordsFromDepthSpace(ofPoint pCameraPoint);

		const ofVec2f&	getDepthSize() { return mDepthSize;  }
		const int		getDepthWidth() { return mDepthSize.x;  }
		const int		getDepthHeight() { return mDepthSize.y; }

		const ofVec2f&	getRgbSize() { return mRgbSize; }
		const int		getRgbWidth() { return mRgbSize.x; }
		const int		getRgbHeight() { return mRgbSize.y; }

	private:
		void			updatePointCloud();

		bool			mIsInit,
						mIsRunning,
						mHasRgb,
						mHasDepth,
						mShouldAlign,
						mShouldGetDepthAsColor,
						mShouldGetPointCloud;

		AlignMode		mAlignMode;
		CloudRes		mCloudRes;

		ofVec2f			mPointCloudRange;
		ofVec2f			mDepthSize;
		ofVec2f			mRgbSize;
		ofPixels		mRgbFrame;
		ofPixels		mDepth8uFrame;
		ofPixels		mColorToDepthFrame;
		ofPixels		mDepthToColorFrame;
		ofShortPixels		mDepthFrame;

		PXCSenseManager		*mSenseMgr;
		PXCProjection		*mCoordinateMapper;
		PXCCapture::Sample	*mCurrentSample;

		vector<PXCPoint3DF32>	mInPoints3D;
		vector<PXCPoint3DF32>	mOutPoints3D;
		vector<PXCPointF32>		mOutPoints2D;
		vector<ofVec3f>			mPointCloud;
		uint16_t				*mRawDepth;
	};
};
#endif
