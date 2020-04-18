//
//  ofxOrbbecAstra.h
//  ofxOrbbecAstra
//
//  Created by Matt Felsen on 10/24/15.
//
//

#include "ofxOrbbecAstra.h"

ofxOrbbecAstra::ofxOrbbecAstra() {
	width = 640;
	height = 480;
	nearClip = 300;
	farClip = 1800;
	maxDepth = 8000;

	bSetup = false;
	bIsFrameNew = false;
	bDepthImageEnabled = true;
}

ofxOrbbecAstra::~ofxOrbbecAstra() {
	astra::terminate();
}

void ofxOrbbecAstra::setLicenseString(const string& license) {
    #ifndef TARGET_OSX
    orbbec_body_tracking_set_license(license.c_str());
    #endif
}

void ofxOrbbecAstra::setup() {
	setup("device/default");
}

void ofxOrbbecAstra::setup(const string& uri) {
	colorImage.allocate(width, height, OF_IMAGE_COLOR);
	depthImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
	depthPixels.allocate(width, height, OF_IMAGE_GRAYSCALE);
	cachedCoords.resize(width * height);
	updateDepthLookupTable();

	astra_status_t status = astra::initialize();
	if (status != ASTRA_STATUS_SUCCESS) {
		ofLogError() << "Failed to initialize Astra camera, status id:  " << status;
	}
    #ifndef TARGET_OSX
	else {
		astra_version_info_t info;
		astra_status_t s = astra_version(&info);
		ofLogNotice() << "Astra SDK successfully initialized.";
		ofLogNotice() << "Astra SDK version: " << info.friendlyVersionString;
	}
    #endif
    
    streamset = shared_ptr<astra::StreamSet>(new astra::StreamSet(uri.c_str()));
    reader = streamset->create_reader();

	bSetup = true;
	reader.add_listener(*this);

}

void ofxOrbbecAstra::enableDepthImage(bool enable) {
	bDepthImageEnabled = enable;
}

void ofxOrbbecAstra::enableRegistration(bool useRegistration) {
	if (!bSetup) {
		ofLogWarning("ofxOrbbecAstra") << "Must call setup() before setRegistration()";
		return;
	}

	reader.stream<astra::DepthStream>().enable_registration(useRegistration);
}

void ofxOrbbecAstra::setDepthClipping(unsigned short _near, unsigned short _far) {
	nearClip = _near;
	farClip = _far;
	updateDepthLookupTable();
}

void ofxOrbbecAstra::initColorStream() {
	if (!bSetup) {
		ofLogWarning("ofxOrbbecAstra") << "Must call setup() before initColorStream()";
		return;
	}

	astra::ImageStreamMode colorMode;
	auto colorStream = reader.stream<astra::ColorStream>();

	colorMode.set_width(width);
	colorMode.set_height(height);
	colorMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_RGB888);
	colorMode.set_fps(30);

	colorStream.set_mode(colorMode);
	colorStream.start();
}

void ofxOrbbecAstra::initDepthStream() {
	if (!bSetup) {
		ofLogWarning("ofxOrbbecAstra") << "Must call setup() before initDepthStream()";
		return;
	}

	astra::ImageStreamMode depthMode;
	auto depthStream = reader.stream<astra::DepthStream>();

	depthMode.set_width(width);
	depthMode.set_height(height);
	depthMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_DEPTH_MM);
	depthMode.set_fps(30);

	depthStream.set_mode(depthMode);
	depthStream.start();
}

void ofxOrbbecAstra::initPointStream() {
	if (!bSetup) {
		ofLogWarning("ofxOrbbecAstra") << "Must call setup() before initPointStream()";
		return;
	}

	reader.stream<astra::PointStream>().start();
}

void ofxOrbbecAstra::initHandStream() {
	if (!bSetup) {
		ofLogWarning("ofxOrbbecAstra") << "Must call setup() before initHandStream()";
		return;
	}

	reader.stream<astra::HandStream>().start();
}


void ofxOrbbecAstra::initBodyStream() {
    if (!bSetup) {
        ofLogWarning("ofxOrbbecAstra") << "Must call setup() before initBodyStream()";
        return;
    }
#ifndef TARGET_OSX
   reader.stream<astra::BodyStream>().start();
#endif
}



void ofxOrbbecAstra::initVideoGrabber(int deviceID) {
	bUseVideoGrabber = true;

	grabber = make_shared<ofVideoGrabber>();
	grabber->setDeviceID(deviceID);
	grabber->setup(width, height);
}

void ofxOrbbecAstra::startColorStream(){
    auto colorStream = reader.stream<astra::ColorStream>();
    colorStream.start();
}

void ofxOrbbecAstra::stopColorStream(){
    auto colorStream = reader.stream<astra::ColorStream>();
    colorStream.stop();
}

void ofxOrbbecAstra::startDepthStream(){
    auto depthStream = reader.stream<astra::DepthStream>();
    depthStream.start();
}

void ofxOrbbecAstra::stopDepthStream(){
    auto depthStream = reader.stream<astra::DepthStream>();
    depthStream.stop();
}

void ofxOrbbecAstra::startPointStream(){
    auto pointStream = reader.stream<astra::PointStream>();
    pointStream.start();
}

void ofxOrbbecAstra::stopPointStream(){
    auto pointStream = reader.stream<astra::PointStream>();
    pointStream.stop();
}

void ofxOrbbecAstra::update(){
	// See on_frame_ready() for more processing
	bIsFrameNew = false;
#ifndef TARGET_OSX
    astra_update();
#else
    astra_temp_update();
#endif

	if (bUseVideoGrabber && grabber) {
		grabber->update();
		if (grabber->isFrameNew()) {
			colorImage.setFromPixels(grabber->getPixels());
			colorImage.mirror(false, true);
			colorImage.update();
		}
	}
}

void ofxOrbbecAstra::draw(float x, float y, float w, float h){
	if (!w) w = width;
	if (!h) h = height;
	colorImage.draw(x, y, w, h);
}

void ofxOrbbecAstra::drawDepth(float x, float y, float w, float h){
	if (!w) w = width;
	if (!h) h = height;
	depthImage.draw(x, y, w, h);
}

bool ofxOrbbecAstra::isFrameNew() {
	return bIsFrameNew;
}

void ofxOrbbecAstra::on_frame_ready(astra::StreamReader& reader,
									astra::Frame& frame)
{
	bIsFrameNew = true;

	auto colorFrame = frame.get<astra::ColorFrame>();
	auto depthFrame = frame.get<astra::DepthFrame>();
	auto pointFrame = frame.get<astra::PointFrame>();
	auto handFrame = frame.get<astra::HandFrame>();
#ifndef TARGET_OSX
    auto bodyFrame = frame.get<astra::BodyFrame>();
#endif
    
	if (colorFrame.is_valid()) {
		colorFrame.copy_to((astra::RgbPixel*) colorImage.getPixels().getData());
		colorImage.update();
	}

	if (depthFrame.is_valid()) {
		depthFrame.copy_to((short*) depthPixels.getData());

		if (bDepthImageEnabled) {
			// TODO do this with a shader so it's fast?
			for (int i = 0; i < depthPixels.size(); i++) {
				short depth = depthPixels.getColor(i).r;
				depth = ofClamp(depth, 0, maxDepth-1);
				float val = depthLookupTable[depth];
				depthImage.setColor(i, ofColor(val));
			}
			depthImage.update();
		}
	}

	if (pointFrame.is_valid()) {
		pointFrame.copy_to((astra::Vector3f*) cachedCoords.data());
	}

	if (handFrame.is_valid()) {
		handMapDepth.clear();
		handMapWorld.clear();
		auto& list = handFrame.handpoints();

		for (auto& handPoint : list) {
			const auto& id = handPoint.tracking_id();

			if (handPoint.status() == HAND_STATUS_TRACKING) {
				const auto& depthPos = handPoint.depth_position();
				const auto& worldPos = handPoint.world_position();

				handMapDepth[id] = ofVec2f(depthPos.x, depthPos.y);
				handMapWorld[id] = ofVec3f(worldPos.x, worldPos.y, worldPos.z);
			} else {
				handMapDepth.erase(id);
				handMapWorld.erase(id);
			}
		}
	}

#ifndef TARGET_OSX
    if (bodyFrame.is_valid()) {
        joints.clear();

        const auto& bodies = bodyFrame.bodies();
        numBodies = bodies.size();

        size_t id = 0;
        for (auto& body : bodies)
        {
            //const auto& id = body.id();
            vector<astra::Joint> newjoints;
            joints.push_back(newjoints);
            for(auto& joint : body.joints())
            {
                joints[id].push_back(joint);
            }
            id++;
        }
    } else {
        numBodies = 0;
    }
#endif
}

void ofxOrbbecAstra::updateDepthLookupTable() {
	// From product specs, range is 8m
	depthLookupTable.resize(maxDepth);

	// Depth values of 0 should be discarded, so set the LUT value to 0 as well
	depthLookupTable[0] = 0;

	// Set the rest
	for (int i = 1; i < maxDepth; i++) {
		depthLookupTable[i] = ofMap(i, nearClip, farClip, 255, 0, true);
	}

}

ofVec3f ofxOrbbecAstra::getWorldCoordinateAt(int x, int y) {
	return cachedCoords[int(y) * width + int(x)];
}

#ifndef TARGET_OSX
int ofxOrbbecAstra::getNumBodies() {
    if(numBodies == 0) return 0;
    return numBodies;
}

int ofxOrbbecAstra::getNumJoints(int body_id) {
    if(numBodies == 0) return 0;
    if(body_id > (numBodies-1)) return 0;
    return joints.at(body_id).size();
}

vector<astra::Joint>& ofxOrbbecAstra::getJointPositions(int body_id) {
    return joints.at(body_id);
}

ofVec2f ofxOrbbecAstra::getJointPosition(int body_id, int joint_id) {
    return ofVec2f(joints[body_id][joint_id].depth_position().x,joints[body_id][joint_id].depth_position().y);
}

astra::JointType ofxOrbbecAstra::getJointType(int body_id, int joint_id) {
    return joints[body_id][joint_id].type();
}

string ofxOrbbecAstra::getJointName(astra::JointType id) {
    switch(static_cast<int>(id))
    {
        case 0:
            return "Head";
        case 1:
            return "ShoulderSpine";
        case 2:
            return "LeftShoulder";
        case 3:
            return "LeftElbow";
        case 4:
            return "LeftHand";
        case 5:
            return "RightShoulder";
        case 6:
            return "RightElbow";
        case 7:
            return "RightHand";
        case 8:
            return "MidSpine";
        case 9:
            return "BaseSpine";
        case 10:
            return "LeftHip";
        case 11:
            return "LeftKnee";
        case 12:
            return "LeftFoot";
        case 13:
            return "RightHip";
        case 14:
            return "RightKnee";
        case 15:
            return "RightFoot";
        case 16:
            return "LeftWrist";
        case 17:
            return "RightWrist";
        case 18:
            return "Neck";
        case 255:
        default:
            return "Unknown";
    }
}

#endif

unsigned short ofxOrbbecAstra::getNearClip() {
	return nearClip;
}

unsigned short ofxOrbbecAstra::getFarClip() {
	return farClip;
}

ofShortPixels& ofxOrbbecAstra::getRawDepth() {
	return depthPixels;
}

ofImage& ofxOrbbecAstra::getDepthImage() {
	return depthImage;
}

ofImage& ofxOrbbecAstra::getColorImage() {
	return colorImage;
}

map<int32_t,ofVec2f>& ofxOrbbecAstra::getHandsDepth() {
	return handMapDepth;
}

map<int32_t,ofVec3f>& ofxOrbbecAstra::getHandsWorld() {
	return handMapWorld;
}
