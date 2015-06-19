#include "ofMain.h"
#include "ofApp.h"

#include "ofAppGLFWWindow.h"

int main( ){

	ofAppGLFWWindow window;
	window.setNumSamples(0);

	ofSetupOpenGL(&window, 200, 200, OF_WINDOW);

	ofRunApp(new ofApp());

}
