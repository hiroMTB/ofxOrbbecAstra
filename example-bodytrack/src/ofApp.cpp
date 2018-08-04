//
//  ofApp.cpp
//  example
//
//  Created by Pierre Proske on 12/04/17.
//
//

#include "ofApp.h"

void ofApp::setup(){
	ofBackground(0);
    ofSetWindowShape(1280, 960);
	ofSetVerticalSync(true);


	astra.setup();
   // astra.setLicenseString("<INSERT LICENSE KEY HERE>");

	astra.initColorStream();
	astra.initDepthStream();
    astra.initBodyStream();

}

void ofApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	astra.update();

}

void ofApp::draw(){

	ofSetColor(255);
    astra.drawDepth(0, 0, 640,480);
	astra.draw(640, 0, 640, 480);

	for (int b = 0; b<astra.getNumBodies(); b++){
        vector<astra::Joint>& joints = astra.getJointPositions(b);
        for(int i = 0; i < joints.size(); i++)
        {
			astra::Joint j = joints[i];
			astra::Vector3f v = j.world_position();
			astra::Vector2f v2 = j.depth_position();

			ofFill();
			ofSetColor(0, 255, 0);
			ofDrawCircle(v2.x, v2.y, 5);
        }
    }

	ofDrawBitmapStringHighlight("fps " + ofToString(ofGetFrameRate(), 0), 20, 20);
}

void ofApp::keyPressed(int key){

}
