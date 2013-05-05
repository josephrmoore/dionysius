//
//  geometry.cpp
//  vector
//
//  Created by Joseph Moore on 4/6/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "geometry.h"

geometry::geometry(){

}
//------------------------------------------------------------------
void geometry::create_geometry( int current_object, ofPoint current_point, int sides, int radius, ofColor color, int current_z, ofPolyline line){
    this->verticies = sides+1;
    this->radius = radius;
    this->color = color;
    this->centroid = current_point;
    this->object = current_object;
    this->z = current_z;
    this->line = line;
    this->ap.setPosition(current_point);
    this->ac.setColor(color);
    this->pointAni();
    this->colorAni();
    this->floatAni();
}
void geometry::update_geometry(ofPoint current_point, int sides, int radius, ofColor color, int current_z, ofPolyline line, bool ani){
    this->verticies = sides+1;
    this->radius = radius;
    this->color = color;
    this->z = current_z;
    this->line = line;
    if(ani){
        this->ap.update(1.0f/60.0f);
        this->af.update(1.0f/60.0f);
        this->ac.update(1.0f/60.0f);
        this->centroid = this->ap.getCurrentPosition(); 
        this->radius = this->af.val(); 
        this->color = this->ac.getCurrentColor();
    } else {
        this->centroid = current_point;
        this->radius = radius; 
        this->color = color;
    }
}
//------------------------------------------------------------------
void geometry::delete_geometry(int index){
    
}
//------------------------------------------------------------------
void geometry::retrieve_geometry(int index){
    
}

//------------------------------------------------------------------
void geometry::updatePoint(ofPoint location){

}
//------------------------------------------------------------------
void geometry::updateLine(ofPoint terminal1, ofPoint terminal2, int width, ofColor color){

}
//------------------------------------------------------------------
void geometry::updatePolygon(ofPoint centroid, int sides, int radius, ofColor color){

}
//------------------------------------------------------------------
void geometry::updateCircle(ofPoint center, int radius, ofColor color){

}
//------------------------------------------------------------------
void geometry::drawOutline(){
    
}
//------------------------------------------------------------------
void geometry::draw(bool outline){
    if(outline){
        ofSetColor(ofColor(0));
        ofFill();
        if(this->verticies==1){
            // do nothing
        } else if (this->verticies==2){
            this->line.draw();
        } else {
            ofBeginShape();
            for(int i=0; i<this->verticies;i++){
                ofVertex(this->centroid.x + (float)((this->radius+6)*sin((i+1) * ((2*PI) / this->verticies))), this->centroid.y + (float)((this->radius+6)*-cos((i+1) * ((2*PI) / this->verticies))));
            }
            ofEndShape();
        }
    }
    ofSetColor(this->color);
    ofFill();
    if(this->verticies==1){
        // do nothing
    } else if (this->verticies==2){
        this->line.draw();
        for(int j=1; j<this->line.size(); j++){
            for(int i=0; i<this->radius/10;i++){
                ofPushMatrix();
                if(this->line[j].y-this->line[j-1].y>this->line[j].x-this->line[j-1].x){
                    ofTranslate(0,i);
                } else {
                    ofTranslate(i,0);
                }
                if(this->line.size()>1){
                    ofLine(this->line[j-1],this->line[j]);
                }
                ofPopMatrix();
            }
        }
    } else {
        
        ofBeginShape();
        for(int i=0; i<this->verticies;i++){
            ofVertex(this->centroid.x + (float)(this->radius*sin((i+1) * ((2*PI) / this->verticies))), this->centroid.y + (float)(this->radius*-cos((i+1) * ((2*PI) / this->verticies))));
        }
        ofEndShape();
    }
}

void geometry::pointAni(){
    // sides = type of bounce:point, amount:color, hue:color
    int bounce_point = (int)ofMap(this->verticies, 1, 21, 0, 26);
    // size = speed:point, distance:point, hue:color
    this->frame_point = (int)ofMap(this->radius, 0, ofGetScreenWidth(), 60, 10);
    int dist_x = (int)ofMap(this->radius, 0, ofGetScreenWidth(), ofGetScreenWidth()/2, 20);
    int dist_y = (int)ofMap(this->radius, 0, ofGetScreenWidth(), 20, ofGetScreenWidth()/2);
    int hue = (int)ofMap(this->radius, 0, ofGetScreenWidth(), 0, 255);
    // color:
    //   hue = type of bounce:float
    int bounce_color = (int)ofMap(this->color.getHue(), 0, 255, 0, 26);
    //   brightness = speed:float, amount:color
    //   saturation = size:float, speed:color
    // pos x = final point:point, saturation:color
    // pos y = final point:point, brightness:color
    // z-index = type of bounce:color, distance:point, size:float
    int bounce_float = (int)ofMap(this->z, 0, this->object, 0, 26);
    // alpha = speed:color, speed:point, speed:float
    
    ofPoint p = ofPoint(0,0);
    this->ap.setRepeatType(PLAY_ONCE);
    this->ap.setCurve(AnimCurve(bounce_point));
    this->ap.animateTo(p);
}

void geometry::colorAni(){
    int bounce_color = (int)ofMap(this->color.getHue(), 0, 255, 0, 26);
    ofColor c = ofColor(0,0,0);
    this->ac.setRepeatType(PLAY_ONCE);
    this->ac.setCurve(AnimCurve(bounce_color));
    this->ac.animateTo(c);
}

void geometry::floatAni(){
    int bounce_float = (int)ofMap(this->z, 0, this->object, 0, 26);
    float f = 100;
    this->af.setRepeatType(PLAY_ONCE);
    this->af.setCurve(AnimCurve(bounce_float));
    this->af.animateTo(f);
}