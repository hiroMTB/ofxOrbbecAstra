//
//  ofxOrbbecAstra.h
//  ofxOrbbecAstra
//
//  Created by Matt Felsen on 10/24/15.
//
//

#pragma once

#include "ofMain.h"
#include <astra/astra.hpp>
#include <astra/capi/astra.h>

class ofxOrbbecAstra : public astra::FrameListener {

public:

	static void terminate();

	ofxOrbbecAstra();
	~ofxOrbbecAstra();  

	// For multiple cameras, use "device/sensor0",
	// "device/sensor1", etc. Otherwise, leave blank.
	bool setup();
	bool setup(const string& uri);
    void setLicenseString(const string& license);
    
	void enableDepthImage(bool enable);
	void enableRegistration(bool useRegistration);
	void setDepthClipping(unsigned short near, unsigned short far);

	void initColorStream();
	void initDepthStream();
	void initPointStream();
	void initHandStream();
    void initBodyStream();
	void initVideoGrabber(int deviceID = 0);

    void startColorStream();
    void stopColorStream();
    void startDepthStream();
    void stopDepthStream();
    void startPointStream();
    void stopPointStream();

	void update();
	bool isFrameNew();

	void draw(float x = 0, float y = 0, float w = 0, float h = 0);
	void drawDepth(float x = 0, float y = 0, float w = 0, float h = 0);

    vector<ofVec3f> & getCachedCoords(){
        return cachedCoords;
    }
    
#ifndef TARGET_OSX
    ofVec2f getJointPosition(int body_id, int joint_id);
    vector<astra::Joint>& getJointPositions(int body_id);
    int getNumBodies();
    int getNumJoints(int body_id);
    astra::JointType getJointType(int body_id, int joint_id);
    string getJointName(astra::JointType id);
	string getSerialNumber();
#endif
    
	ofVec3f getWorldCoordinateAt(int x, int y);    

	unsigned short getNearClip();
	unsigned short getFarClip();

	ofShortPixels& getRawDepth();
	ofImage& getDepthImage();
	ofImage& getColorImage();

    map<int32_t,ofVec2f>& getHandsDepth();
    map<int32_t,ofVec3f>& getHandsWorld();

protected:

	virtual void on_frame_ready(astra::StreamReader& reader,
		astra::Frame& frame) override;

	void updateDepthLookupTable();

    shared_ptr<astra::StreamSet> streamset;
    astra::StreamReader reader;

	int width;
	int height;
	bool bSetup;
	bool bIsFrameNew;
	bool bDepthImageEnabled;
	unsigned short nearClip;
	unsigned short farClip;
	int maxDepth;

	ofShortPixels depthPixels;
	ofImage depthImage;
	ofImage colorImage;

	// Hack for Astra Pro cameras which only expose color via a webcam/UVC
	// stream, not through the SDK
	bool bUseVideoGrabber;
	shared_ptr<ofVideoGrabber> grabber;

	vector<char> depthLookupTable;
	vector<ofVec3f> cachedCoords;

    map<int32_t,ofVec2f> handMapDepth;
    map<int32_t,ofVec3f> handMapWorld;

#ifndef TARGET_OSX
    vector<vector<astra::Joint>> joints;
    size_t numBodies;
#endif

};
