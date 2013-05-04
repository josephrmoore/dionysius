//
//  Circle.h
//  vector
//
//  Created by Joseph Moore on 4/6/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"

class geometry{
    
public:
    
    int object, verticies, radius, x, y, z;
    ofColor color;
    ofPoint centroid;
    ofPolyline line;
    
    geometry();
    
    void updatePoint(ofPoint location);
    void updateLine(ofPoint terminal1, ofPoint terminal2, int width, ofColor color);
    void updatePolygon(ofPoint centroid, int sides, int radius, ofColor color);
    void updateCircle(ofPoint center, int radius, ofColor color);

    void create_geometry(int current_object, ofPoint current_point, int sides, int radius, ofColor color, int current_z, ofPolyline line);
    void update_geometry(ofPoint current_point, int sides, int radius, ofColor color, int current_z, ofPolyline line);
    void retrieve_geometry(int index);
    void delete_geometry(int index);
    void draw(bool outline);
    void drawOutline();
};