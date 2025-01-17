#pragma once

#include "ofxExtras.h"
#include "geometry.h"
#include "ofxOsc.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw(); 

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void drawPreview();
        void edit();
        void placeObject();
        void devInfo();
        void deleteObject(int index);
        void print_r(vector <geometry> v);
        void print_r(vector <int> v);
        void animate(ofPoint p, int s, int r, ofColor c, int z, ofPolyline l);
        void audio(ofPoint p, int s, int r, ofColor c, int z, ofPolyline l);
    
        bool oskar;
        vector <geometry> objects;
        vector <int> zs;
        geometry preview_object, empty_object;
        int current_object;
        ofPoint current_point;
        ofColor current_color, current_bk, current_preview;
        int current_sides, current_radius, current_z, step, current_alpha;
        int wheel_state; // temporary for keyboard clone
        ofPolyline line;
    
        bool		bSendSerialMessage;			// a flag for sending serial
        char		bytesRead[3];				// data from serial, we will be trying to read 3
        char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
        int			nBytesRead;					// how much did we read?
        int			nTimesRead;					// how many times did we read?
        float		readTime;					// when did we last read?				
        string      arduino, output;
    
        ofSerial	serial;
        ofxOscSender sender;
    
        vector <string> values;
    
        bool j1_up, j1_down, j1_left, j1_right, j1_button, j2_up, j2_down, j2_left, j2_right, j2_button, delete_button, ok_button, info_on, vertex, close, flag_start, flag_stop;
};
