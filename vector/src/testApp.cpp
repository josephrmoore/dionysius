#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    current_point.x = ofGetScreenWidth()/2;
    current_point.y = ofGetScreenHeight()/2;
    current_object = -1;
    current_sides = 2;
    current_radius = 10;
    current_z = 0;
    current_alpha = 255;
    current_color = ofColor(255,0,0);
    current_bk = ofColor(255,255,255);
    current_preview = current_color;
    preview_object.create_geometry(-2, current_point, current_sides, current_radius, current_color, current_z, line);
    ofSetVerticalSync(true);
    step = 5;
    info_on = true;
    arduino = "";
	serial.setup(0, 9600); //open the first device
    oskar = false;
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
    vertex = false;
    close = false;
}

//--------------------------------------------------------------
void testApp::update(){
    current_color = ofColor(current_color.r, current_color.g, current_color.b, current_alpha);
    if(current_sides==0){
        current_bk = current_color;
        current_preview = ofColor(0,0,0);
    } else {
        if(current_color.r == current_bk.r && current_color.g == current_bk.g && current_color.b == current_bk.b){
            current_preview = ofColor(0,0,0);
        } else {
            current_preview = current_color;
        }
    }
    preview_object.update_geometry(current_point, current_sides, current_radius, current_preview, current_z, line, false);
    for(int i=0; i<objects.size(); i++){
        objects[i].update_geometry(objects[i].centroid, objects[i].verticies-1, objects[i].radius, objects[i].color, objects[i].z, objects[i].line, oskar);
    }
    ofBackground(current_bk);
    do {
        arduino = ofxGetSerialString(serial,';'); //read until end of line
        if(arduino != ""){
            output = arduino;
            values = ofSplitString(output, " ", true, true); // trues are ignoreempty & trim
            if(values.size()==20){
                current_point.x += ofToInt(values[0]);
                if(current_point.x > ofGetScreenWidth()+current_radius){
                    current_point.x = ofGetScreenWidth()+current_radius;
                }
                if(current_point.x < -current_radius-5){
                    current_point.x = -current_radius-5;
                }
                current_point.y += -ofToInt(values[1]);
                if(current_point.y > ofGetScreenHeight()+current_radius){
                    current_point.y = ofGetScreenHeight()+current_radius;
                }
                if(current_point.y < -current_radius-5){
                    current_point.y = -current_radius-5;
                }
                if(ofToInt(values[2]) == 1 && j1_up == false){
                    // J1 UP
                    if(current_object < (objects.size()-1) && objects.size()>0){
                        current_object++;
                    } else {
                        current_object = 0;
                    }
                    j1_up = true;
                } else {
                    j1_up = false;
                }
                if(ofToInt(values[3]) == 1 && j1_down == false){
                    // J1 DOWN
                    if((current_object) > -1 && objects.size()>0){
                        current_object--;
                    } else {
                        current_object = (objects.size()-1);
                    }
                    j1_down = true;
                } else {
                    j1_down = false;
                }
                if(ofToInt(values[4]) == 1 && j1_left == false){
                    // J1 LEFT
                    if((current_object) > -1 && objects.size()>0){
                        current_object--;
                    } else {
                        current_object = (objects.size()-1);
                    }
                    j1_left = true;
                } else {
                    j1_left = false;
                }
                if(ofToInt(values[5]) == 1 && j1_right == false){
                    // J1 RIGHT
                    if(current_object < (objects.size()-1) && objects.size()>0){
                        current_object++;
                    } else {
                        current_object = 0;
                    }
                    j1_right = true;
                } else {
                    j1_right = false;
                }
                if(ofToInt(values[6]) == 0 && ok_button == false){
                    // OK button
                    if(current_sides!=0){
                        if(current_object<0){
                            if(current_sides!=0){
                                placeObject();
                            }
                        } else {
                            edit();
                        }
                    }
                    ok_button = true;
                } else {
                    ok_button = false;
                }
                if(ofToInt(values[7]) == 0 && delete_button == false){
                    // DELETE
                    if(current_object>=0){
                        deleteObject(current_object);            
                    }
                    delete_button = true;
                } else {
                    delete_button = false;
                }
                if(values[8] == "0" && j2_up == false){                    
                    // J2 UP
                    float bri = current_color.getBrightness();
                    bri+=step;
                    current_color.setBrightness(bri);
                    j2_up = true;
                } else {
                    j2_up = false;
                }
                if(values[9] == "0" && j2_down == false){
                    // J2 DOWN
                    float bri = current_color.getBrightness();
                    bri-=step;
                    current_color.setBrightness(bri);
                    j2_down = true;
                } else {
                    j2_down = false;
                }
                if(values[10] == "0" && j2_left == false){
                    // J1 LEFT
                    float sat = current_color.getSaturation();
                    if(sat>step){
                        sat-=(2*step);
                        current_color.setSaturation(sat);
                    }
                    j2_left = true;
                } else {
                    j2_left = false;
                }
                if(values[11] == "0" && j2_right == false){
                    // J2 RIGHT
                    float sat = current_color.getSaturation();
                    if(sat<(255-step)){
                        sat+=(2*step);
                        current_color.setSaturation(sat);
                    }
                    j2_right = true;
                } else {
                    j2_right = false;
                }
                current_sides = (int)(ofMap(ofToInt(values[12]), 58, 1006, 0, 20));
                current_radius = (int)(ofMap(ofToInt(values[13]), 0, 1020, 0, ofGetScreenWidth()/2));
                current_color.setHue(ofMap(ofToInt(values[14]), 58, 1021, 0, 255));
                current_z = (int)(ofMap(ofToInt(values[15]), 58, 1008, 0, objects.size()));
                current_alpha = (int)(ofMap(ofToInt(values[16]), 58, 1020, 0, 255));
                if(ofToInt(values[17]) == 0 && delete_button == false){
                    // CLOSE SHAPE
                    line.close();
                    close = true;
                } else {
                    close = false;
                }
                if(ofToInt(values[18]) == 0 && delete_button == false){
                    // VERTEX
                    if(current_sides==1){
                        line.addVertex(current_point);
                    }
                    vertex = true;
                } else {
                    vertex = false;
                }
                if(ofToInt(values[19]) == 0){
                    // OSKAR MODE
                    oskar = false;
                } else {
                    oskar = true;
                }
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
    // if preview is on the bottom
    if(objects.size()==0){
        drawPreview();
    }
    // draw the objects in z-index order, drawing preview where it belongs in the stack
    for(int i=0;i<objects.size();i++){
        for(int j=0;j<zs.size();j++){
            if(zs[j]==i){
                if(current_z==zs[j]){
                    drawPreview();
                }
                if(objects[j].z == current_object){
                    objects[j].draw(true);
                } else {
                    objects[j].draw(false);
                }
            }
        }
        // if preview is on top
        if(current_z==zs.size()){
            drawPreview();
        }
    }
    if(current_sides == 1){
        ofLine(current_point.x-5, current_point.y-5, current_point.x+5, current_point.y+5);
        ofLine(current_point.x-5, current_point.y+5, current_point.x+5, current_point.y-5);
    }
    devInfo();
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
            sat-=(2*step);
            current_color.setSaturation(sat);
        }
    }
    if(key == OF_KEY_RIGHT){
        float sat = current_color.getSaturation();
        if(sat<(255-step)){
            sat+=(2*step);
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
    if(key == 'k'){
        if(current_alpha>0){
            current_alpha-=step;
        }
    }
    if(key == 'l'){
        if(current_alpha<255){
            current_alpha+=step;
        }
    }
    if(key == 'i'){
        info_on = !info_on;
    }
    if(key == 'c'){
        line.close();
    }
    if(key == 'v'){
        if(current_sides==1){
            line.addVertex(current_point);
        }
    }
    if(key == 'a'){
        if((current_object) > -1 && objects.size()>0){
            current_object--;
        } else {
            current_object = (objects.size()-1);
        }
    }
    if(key == 'd'){
        if(current_object < (objects.size()-1) && objects.size()>0){
            current_object++;
        } else {
            current_object = 0;
        }
    }
    if(key == 'w'){
        if(current_object < (objects.size()-1) && objects.size()>0){
            current_object++;
        } else {
            current_object = 0;
        }
    }
    if(key == 's'){
        if((current_object) > 0 && objects.size()>0){
            current_object--;
        } else {
            current_object = (objects.size()-1);
        }
    }
    if(key == 'o'){
        oskar = !oskar;
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
        if(current_sides!=0){
            if(current_object<0){
                if(current_sides!=0){
                    placeObject();
                }
            } else {
                edit();
            }
        }

    }
    if(key == OF_KEY_BACKSPACE){
        if(current_object>=0){
            deleteObject(current_object);            
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if(arduino == ""){
        current_point.x = x;
        current_point.y = y;
    }
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

//--------------------------------------------------------------
void testApp::drawPreview(){
    ofSetColor(current_preview);
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

//--------------------------------------------------------------
void testApp::edit(){
    if(objects.size()>0 && current_object >= 0){
        current_sides = objects[current_object].verticies-1;
        current_radius = objects[current_object].radius;
        current_color = objects[current_object].color;
        current_z = objects[current_object].z;
        line = objects[current_object].line;
        zs[current_object] = current_z;
        deleteObject(current_object);
    }
    current_object = -1;
}

//--------------------------------------------------------------
void testApp::placeObject(){
    geometry newgeo;
    newgeo.create_geometry(objects.size(), current_point, current_sides, current_radius, current_color, current_z, line);
    for(int i=0;i<zs.size();i++){
        objects[i].z = zs[i];
        if(zs[i] >= current_z){
            zs[i]++;
        }
    }
    for(int i=0;i<zs.size();i++){
        if(objects.size()==zs.size()){
            objects[i].z = zs[i];            
        } else {
            cout<<"ERROR: zs & objects not the same length"<<endl;
        }
    }
    zs.push_back(current_z);
    objects.push_back(newgeo);
    line.clear();
    current_z++;
    if(oskar){
        animate(current_point, current_sides, current_radius, current_color, current_z, line);
        audio(current_point, current_sides, current_radius, current_color, current_z, line);
    }
}

//--------------------------------------------------------------
void testApp::devInfo(){
    if(info_on){
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
        info += "alpha:"+ofToString(current_alpha)+"\n";
        info += "selected:"+ofToString(current_object)+"\n";
        info += "serial:" + output;
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info, 30, 30);
    }
}

//--------------------------------------------------------------
void testApp::deleteObject(int index){
    if(zs[index]>=0 && objects.size()>0){
        if(zs[index]==0){
            for(int i=0;i<zs.size();i++){
                zs[i]-=1;
                objects[i].z = zs[i];
            }
        } else if (zs[index]!= objects.size()-1){
            for(int i=0;i<zs.size();i++){
                if(zs[i]>zs[index]){
                    zs[i]-=1;
                    objects[i].z = zs[i];
                }
            }
        }
        objects.erase((objects.begin() + index));
        objects.resize(objects.size());
        zs.resize(zs.size());
        zs.erase(zs.begin() + index);
        current_object = -1;
        current_z = objects.size();
    }
}

//--------------------------------------------------------------
void testApp::print_r(vector <geometry> v){
    if(v.size()>0){
        cout<<"==========================================="<<endl;
        for(int i=0; i<v.size(); i++){
            cout<<"Geometry #"<<i<<":"<<endl;
            cout<<"  id: "<<v[i].object<<endl;
            cout<<"  verticies: "<<v[i].verticies<<endl;
            cout<<"  hue: "<<v[i].color.getHue()<<endl;
            cout<<"  saturation: "<<v[i].color.getSaturation()<<endl;
            cout<<"  brightness: "<<v[i].color.getBrightness()<<endl;
            cout<<"  radius :"<<v[i].radius<<endl;
            cout<<"  z-index: "<<v[i].z<<endl;
            cout<<"  position: "<<v[i].centroid.x<<", "<<v[i].centroid.y<<endl;
            cout<<"  line segments: "<<v[i].line.size()<<endl;
            for(int j=0; j<v[i].line.size(); j++){
                cout<<"    point #"<<j<<": "<<v[i].line[j].x<<", "<<v[i].line[j].y<<endl;
            }
            cout<<""<<endl;
        }
        cout<<"==========================================="<<endl;
    }
}

//--------------------------------------------------------------
void testApp::print_r(vector <int> v){
    if(v.size()>0){
        cout<<"==========================================="<<endl;
        for(int i=0; i<v.size(); i++){
            cout<<"Integer #"<<i<<":"<<endl;
            cout<<"  z-index: "<<v[i]<<endl;
            cout<<""<<endl;
        }
        cout<<"==========================================="<<endl;
    }
}

//--------------------------------------------------------------
void testApp::animate(ofPoint p, int s, int r, ofColor c, int z, ofPolyline l){
    cout<<"animated!"<<endl;
}

//--------------------------------------------------------------
void testApp::audio(ofPoint p, int s, int r, ofColor c, int z, ofPolyline l){
    cout<<"audioed!"<<endl;
}