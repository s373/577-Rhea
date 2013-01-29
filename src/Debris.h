/*
 *  Debris.h
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h"

class World;


class Debris {
public:
	
	ofPoint pos, vel, acc;
	float linemult;
	float lifetime, lifedec, rad;
	
	World *world;
	
	Debris(){
		world = NULL;
	}
	
	void setup(World *w, const ofPoint & loc, const ofPoint & velocidade);
	void update();
	void draw();
	

};