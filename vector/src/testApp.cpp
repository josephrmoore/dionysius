#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    current_point.x = ofGetScreenWidth()/2;
    current_point.y = ofGetScreenHeight()/2;
    current_object = 0;
    current_sides = 0;
    current_radius = 10;
    current_z = current_object;
    current_color = ofColor(255,0,0);
    preview_object.create_geometry(-2, current_point, current_sides, current_radius, current_color, current_z, line);
    ofSetVerticalSync(true);
    step = 1;
    
    arduino = "";
	serial.setup(0, 9600); //open the first device
    
    j1_up = false;
    j1_down = false;
    j1_left = false;
    j1_right = false;
    j1_button = false;
    j2_up = false;
    j2_down = false;
    j2_left = false;
    j2_right = false;
    j2_button = false;
    delete_button = false;
    ok_button = false;
}

//--------------------------------------------------------------
void testApp::update(){
    preview_object.update_geometry(current_point, current_sides, current_radius, current_color);
    do {
        arduino = ofxGetSerialString(serial,';'); //read until end of line
        if(arduino != ""){
            output = arduino;
            values = ofSplitString(output, " ", true, true); // trues are ignoreempty & trim
            if(values.size()==16){
                current_point.x += ofToInt(values[0]);
                current_point.y += ofToInt(values[1]);
                if(ofToInt(values[8]) == 0 && j1_up == false){
                    // J1 UP
                    if(current_object < (objects.size()-1)){
                        current_object++;
                    } else {
                        current_object = 0;
                    }
                    j1_up = true;
                } else {
                    j1_up = false;
                }
                if(ofToInt(values[9]) == 0 && j1_down == false){
                    // J1 DOWN
                    if((current_object) > 0){
                        current_object--;
                    } else {
                        current_object = (objects.size()-1);
                    }
                    j1_down = true;
                } else {
                    j1_down = false;
                }
                if(ofToInt(values[10]) == 0 && j1_left == false){
                    // J1 LEFT
                    if((current_object) > 0){
                        current_object--;
                    } else {
                        current_object = (objects.size()-1);
                    }
                    j1_left = true;
                } else {
                    j1_left = false;
                }
                if(ofToInt(values[11]) == 0 && j1_right == false){
                    // J1 RIGHT
                    if(current_object < (objects.size()-1)){
                        current_object++;
                    } else {
                        current_object = 0;
                    }
                    j1_right = true;
                } else {
                    j1_right = false;
                }
                if(ofToInt(values[6]) == 1 && ok_button == false){
                    // OK button
                    geometry newgeo;
                    newgeo.create_geometry(objects.size(), current_point, current_sides, current_radius, current_color, current_z, line);
                    zs.push_back(objects.size());
                    objects.push_back(newgeo);
                    current_object = objects.size()-1;
                    // step--;
                    ok_button = true;
                } else {
                    ok_button = false;
                }
                if(values[2] == "1" && j2_up == false){
                    // J2 UP
                    float bri = current_color.getBrightness();
                    bri+=step;
                    current_color.setBrightness(bri);
                    j2_up = true;
                } else {
                    j2_up = false;
                }
                if(values[3] == "1" && j2_down == false){
                    // J2 DOWN
                    float bri = current_color.getBrightness();
                    bri-=step;
                    current_color.setBrightness(bri);
                    j2_down = true;
                } else {
                    j2_down = false;
                }
                if(values[4] == "1" && j2_left == false){
                    // J1 LEFT
                    float sat = current_color.getSaturation();
                    cout<<sat<<endl;

                    sat-=step;
                    current_color.setSaturation(sat);
                    cout<<sat<<endl;
                    sat = current_color.getSaturation();
                    cout<<sat<<endl;

                    j2_left = true;
                } else {
                    j2_left = false;
                }
                if(values[5] == "1" && j2_right == false){
                    // J2 RIGHT
                    float sat = current_color.getSaturation();
                    cout<<sat<<endl;
                    sat+=step;
                    current_color.setSaturation(sat);
                    j2_right = true;
                } else {
                    j2_right = false;
                }
                if(ofToInt(values[7]) == 0 && delete_button == false){
                    // DELETE
                    if(current_object>=0){
                        objects.erase(objects.begin() + current_object);
                        current_object--;
                    }
                    delete_button = true;
                } else {
                    delete_button = false;
                }
                current_color.setHue(ofMap(ofToInt(values[14]), 5, 685, 0, 255));
                current_sides = ofMap(ofToInt(values[13]), 5, 685, 0, 20);
                current_radius = ofMap(ofToInt(values[12]), 5, 685, 0, ofGetScreenWidth()/2);
                current_z = ofMap(ofToInt(values[15]), 5, 685, 0, objects.size());
            }
        }
        if (arduino == "") continue;
        //do something with str
    } while (arduino != "");
    if(current_sides != 1){
        if(line.getArea() != 0){
            line.clear();            
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    string info = "";
	info += "x:"+ofToString(current_point.x)+"\n"; 
	info += "y:"+ofToString(current_point.y)+"\n";
    info += "sides:"+ofToString(current_sides+1)+"\n";
    info += "size:"+ofToString(current_radius)+"\n";
    info += "color:"+ofToString(current_color)+"\n";
    info += "hue:"+ofToString(current_color.getHue())+"\n";
    info += "saturation:"+ofToString(current_color.getSaturation())+"\n";
    info += "brightness:"+ofToString(current_color.getBrightness())+"\n";
    info += "step:"+ofToString(step)+"\n";
    info += "z:"+ofToString(current_z)+"\n";
    info += "selected:"+ofToString(current_object)+"\n";
    info += "serial:" + output;
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
    if(objects.size()==0){
        preview_object.draw(false);
        ofSetColor(current_color);
        for(int j=1; j<line.size(); j++){
            this->line.draw();
            for(int i=0; i<current_radius/10;i++){
                ofPushMatrix();
                if(line[j].y-line[j-1].y>line[j].x-line[j-1].x){
                    ofTranslate(0,i);
                } else {
                    ofTranslate(i,0);                
                }
                if(line.size()>1){
                    ofLine(line[j-1],line[j]);            
                }
                ofPopMatrix();
            }
        }
    }
    for(int i=0;i<objects.size();i++){
        for(int j=0;j<zs.size();j++){
            if(zs[j]==i){
                if(current_z==zs[j]){
                    preview_object.draw(false);
                    ofSetColor(current_color);
                    for(int j=1; j<line.size(); j++){
                        this->line.draw();
                        for(int i=0; i<current_radius/10;i++){
                            ofPushMatrix();
                            if(line[j].y-line[j-1].y>line[j].x-line[j-1].x){
                                ofTranslate(0,i);
                            } else {
                                ofTranslate(i,0);                
                            }
                            if(line.size()>1){
                                ofLine(line[j-1],line[j]);            
                            }
                            ofPopMatrix();
                        }
                    }
                }
                if(objects[j].object == current_object){
                    objects[j].draw(true);
                } else {
                    objects[j].draw(false);
                }
            }
        }
        if(current_z==zs.size()){
            preview_object.draw(false);
            ofSetColor(current_color);
            for(int j=1; j<line.size(); j++){
                this->line.draw();
                for(int i=0; i<current_radius/10;i++){
                    ofPushMatrix();
                    if(line[j].y-line[j-1].y>line[j].x-line[j-1].x){
                        ofTranslate(0,i);
                    } else {
                        ofTranslate(i,0);                
                    }
                    if(line.size()>1){
                        ofLine(line[j-1],line[j]);            
                    }
                    ofPopMatrix();
                }
            }
        }

    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == '+'){
        step++;
    }
    if(key == '_'){
        step--;
    }
    if(key == OF_KEY_LEFT){
        float sat = current_color.getSaturation();
        if(sat>step){
            sat-=step;
            current_color.setSaturation(sat);
        }
    }
    if(key == OF_KEY_RIGHT){
        float sat = current_color.getSaturation();
        if(sat<255-step){
            sat+=step;
            current_color.setSaturation(sat);
        }
    }
    if(key == OF_KEY_UP){
        float bri = current_color.getBrightness();
        bri+=step;
        current_color.setBrightness(bri);
    }
    if(key == OF_KEY_DOWN){
        float bri = current_color.getBrightness();
        bri-=step;
        current_color.setBrightness(bri);
    }
    if(key == 'a'){
        if((current_object) > -1){
            current_object--;
        } else {
            current_object = (objects.size()-1);
        }
    }
    if(key == 'd'){
        if(current_object < (objects.size()-1)){
            current_object++;
        } else {
            current_object = 0;
        }
    }
    if(key == 'c'){
        line.close();
    }
    if(key == 'v'){
        if(current_sides==1){
            line.addVertex(current_point);
        }
    }
    if(key == 'w'){
        if(current_object < (objects.size()-1)){
            current_object++;
        } else {
            current_object = 0;
        }
    }
    if(key == 's'){
        if((current_object) > 0){
            current_object--;
        } else {
            current_object = (objects.size()-1);
        }
    }
    if(key == '1'){
        wheel_state = 1;  
    }
    if(key == '2'){
        wheel_state = 2;
    }
    if(key == '3'){
        wheel_state = 3;
    }
    if(key == '-'){
        if(wheel_state == 1){
            if(current_sides>0){
                current_sides-=1;
            }
        } else if (wheel_state == 2){
            if(current_radius>1){
                current_radius-=step;
            }
        } else if (wheel_state == 3){
            float hue = current_color.getHue();
            hue-=step;
            current_color.setHue(hue);
        }
    }
    if(key == '='){
        if(wheel_state == 1){
            current_sides+=1;
        } else if (wheel_state == 2){
            current_radius+=step;
        } else if (wheel_state == 3){
            float hue = current_color.getHue();
            hue+=step;
            current_color.setHue(hue);
        }
    }
    if(key == 'z'){
        if(current_z>0){
            current_z--;            
        }
    }
    if(key == 'x'){
        if(current_z<objects.size() && objects.size()>0){
            current_z++;
        }
    }
    if(key == ' ' || key == OF_KEY_RETURN){
        geometry newgeo;
        newgeo.create_geometry(objects.size(), current_point, current_sides, current_radius, current_color, current_z, line);
        for(int i=0;i<zs.size();i++){
            if(zs[i] >= current_z){
                zs[i]++;
            }
        }
        zs.push_back(current_z);
        objects.push_back(newgeo);
        line.clear();
    }
    if(key == OF_KEY_BACKSPACE){
        if(current_object>0){
            objects.erase(objects.begin() + current_object);
            zs.erase(zs.begin() + current_object);
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    current_point.x = x;
    current_point.y = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}