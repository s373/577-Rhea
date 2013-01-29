/*
 *  Goodies.h
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"


class World;
class Planet;

class Goodie {
public:
	
	float radfromplanet, angle;
//	float orbitspeed, orbit;

	ofPoint pos;
	float rad;
	
	float energy;
	bool active, good;
	
	float pulse,pulseinc; 
	
	World * world;
	Planet * planet;
	
	ofImage img;
	
	Goodie(){
		world = NULL;
		planet = NULL;		
	}
	void setup(World *w);
	void update();
	void draw();
	

};