/*
 *  Player.h
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"


class World;


class Player {
public:
	ofPoint pos,vel,acc, accPlanets, accHeading;
	float rad, angle, speed;
	float force, energy, colide;
	bool kup,kdown,kleft,kright, koliding, lives;
	
	int numCollected;
		
	World * world;

	float distanceTravelled;
	
	Player(){
		world = NULL;
		distanceTravelled = 0.0f;
	}
	
	void setup( World *w);
	void calcGrav(); void bound(); void hurt(); void heal();
	void findGoodies();
	void update();
	void draw();
	

};