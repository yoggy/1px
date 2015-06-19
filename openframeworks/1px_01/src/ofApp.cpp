#include "ofApp.h"

ofMesh mesh;

void ofApp::setup() 
{
	ofSetFrameRate(4);
	ofDisableSmoothing();

	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.addVertex(ofPoint(100, 100));
}

void ofApp::draw()
{
	ofBackground(0, 0, 0);

	ofColor c;
	switch (ofGetFrameNum() % 4) {
	case 0:
		c = ofColor(255, 0, 0);
		break;
	case 1:
		c = ofColor(0, 255, 0);
		break;
	case 2:
		c = ofColor(0, 0, 255);
		break;
	default:
		c = ofColor(0, 0, 0);
		break;
	}

	ofSetColor(c);
	mesh.draw();
}
