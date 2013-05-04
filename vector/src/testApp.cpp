#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    current_point.x = ofGetScreenWidth()/2;
    current_point.y = ofGetScreenHeight()/2;
    current_object = -1;
    current_sides = 2;
    current_radius = 10;
    current_z = 0;
    current_color = ofColor(255,0,0);
    current_bk = ofColor(255,255,255);
    current_preview = current_color;
    preview_object.create_geometry(-2, current_point, current_sides, current_radius, current_color, current_z, line);
    ofSetVerticalSync(true);
    step = 1;
    info_on = true;
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
    preview_object.update_geometry(current_point, current_sides, current_radius, current_color, current_z, line);
    if(current_sides==0){
        current_bk = current_color;
        current_preview = ofColor(0,0,0);
    } else {
//        cout<<"else"<<endl;
        if(current_color.r == current_bk.r && current_color.g == current_bk.g && current_color.b == current_bk.b){
//            cout<<"2if"<<endl;
            current_preview = ofColor(0,0,0);
        } else {
//            cout<<"2else"<<endl;
            current_preview = current_color;
        }
    }
    ofBackground(current_bk);
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
                    placeObject();
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
                    sat-=(2*step);
                    current_color.setSaturation(sat);
                    sat = current_color.getSaturation();
                    j2_left = true;
                } else {
                    j2_left = false;
                }
                if(values[5] == "1" && j2_right == false){
                    // J2 RIGHT
                    float sat = current_color.getSaturation();
                    sat+=(2*step);
                    current_color.setSaturation(sat);
                    j2_right = true;
                } else {
                    j2_right = false;
                }
                if(ofToInt(values[7]) == 0 && delete_button == false){
                    // DELETE
                    if(current_object>0){
                        objects.erase(objects.begin() + current_object);
                        zs.erase(zs.begin() + current_object);
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
        cout<<sat<<" "<<step<<endl;
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
        cout<<objects.size()<<endl;

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
    current_sides = objects[current_object].verticies-1;
    current_radius = objects[current_object].radius;
    current_color = objects[current_object].color;
    current_z = objects[current_object].z;
    line = objects[current_object].line;
    zs[current_object] = current_z;
    objects.erase((objects.begin() + current_object));
    zs.erase(zs.begin() + current_object);
    current_object = -1;
}

//--------------------------------------------------------------
void testApp::placeObject(){
    cout<<"PLACED BEFORE!!!!!!"<<endl;
    print_r(objects);
    print_r(zs);
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
    cout<<"PLACED AFTER!!!!!!"<<endl;
    print_r(objects);
    print_r(zs);
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
        info += "selected:"+ofToString(current_object)+"\n";
        info += "serial:" + output;
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info, 30, 30);
    }
}


void testApp::deleteObject(int index){
    cout<<"DELETE BEFORE!!!!!!"<<endl;
    print_r(objects);
    print_r(zs);
    cout<<"zs[index]: "<<zs[index]<<endl;
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
        zs.erase(zs.begin() + index);
        current_object = -1;
        current_z = objects.size();
    }
    cout<<"DELETE AFTER!!!!!!"<<endl;
    print_r(objects);
    print_r(zs);
    //        cout<<objects.size()<<endl;
}

void testApp::print_r(vector <geometry> v){
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

void testApp::print_r(vector <int> v){
    cout<<"==========================================="<<endl;
    for(int i=0; i<v.size(); i++){
        cout<<"Integer #"<<i<<":"<<endl;
        cout<<"  z-index: "<<v[i]<<endl;
        cout<<""<<endl;
    }
    cout<<"==========================================="<<endl;
}