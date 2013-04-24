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
void geometry::create_geometry( int current_object, ofPoint current_point, int sides, int radius, ofColor color, int current_z){
    this->verticies = sides+1;
    this->radius = radius;
    this->color = color;
    this->centroid = current_point;
    this->object = current_object;
    this->z = current_object;
}
void geometry::update_geometry(ofPoint current_point, int sides, int radius, ofColor color){
    this->verticies = sides+1;
    this->radius = radius;
    this->color = color;
    this->centroid = current_point;
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
            ofCircle(this->centroid.x, this->centroid.y, this->radius+3, this->radius+3);
        } else if (this->verticies==2){
            ofLine(this->centroid.x, this->centroid.y, this->centroid.x+this->radius, this->centroid.y+this->radius);
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
        ofCircle(this->centroid.x, this->centroid.y, this->radius, this->radius);
    } else if (this->verticies==2){
        ofLine(this->centroid.x, this->centroid.y, this->centroid.x+this->radius, this->centroid.y+this->radius);
    } else {
        ofBeginShape();
        for(int i=0; i<this->verticies;i++){
            ofVertex(this->centroid.x + (float)(this->radius*sin((i+1) * ((2*PI) / this->verticies))), this->centroid.y + (float)(this->radius*-cos((i+1) * ((2*PI) / this->verticies))));
        }
        ofEndShape();
    }
}