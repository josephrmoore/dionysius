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
    this->ap.setRepeatType(LOOP_BACK_AND_FORTH);
    this->ap.setCurve(EASE_IN_EASE_OUT);
    this->ap.animateTo(ofPoint(current_point.x+radius+40, current_point.y+radius+40));
    this->ac.setColor(color);
    this->ac.setRepeatType(LOOP_BACK_AND_FORTH);
    this->ac.setCurve(EASE_IN_EASE_OUT);
    this->ac.animateTo(ofColor(color.r-100, color.g+100, color.b+100));
    this->af.setRepeatType(LOOP_BACK_AND_FORTH);
    this->af.setCurve(EASE_IN_EASE_OUT);
    this->af.animateTo(radius+20);
}
void geometry::update_geometry(ofPoint current_point, int sides, int radius, ofColor color, int current_z, ofPolyline line, bool ani){
    this->verticies = sides+1;
    this->radius = radius;
    this->color = color;
    this->z = current_z;
    this->line = line;
    this->ap.update(1.0f/60.0f);
    this->af.update(1.0f/60.0f);
    this->ac.update(1.0f/60.0f);
    if(ani){
        this->centroid = this->ap.getCurrentPosition(); 
        this->radius = this->af.val(); 
        this->color = this->ac.getCurrentColor();
        cout<<this->ac.getCurrentColor()<<endl;
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