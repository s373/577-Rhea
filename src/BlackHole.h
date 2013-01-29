/*
 *  BlackHole.h
 *  577Rhea
 *
 *  Created by andré sier on 20130127.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once


#include "ofMain.h"

class World;
class Player;

class BlackHole {
public:
	ofPoint pos;
	
	float rad, diameter, radmin, radmax;
	float radinfluence, radinfluencesquared;
	float rot, rotspeed;
	float pulse, pulsespeed;
	int hiblink; bool white;
	bool mayenter;
	World *world;
	
	int	  audioom;
	float phase, phaseinc, frequency, volume;
	float source, target, delta;

	
	BlackHole(){
		world = NULL;
	}
	void setup( World *w, const ofPoint & loc, const float rad, const bool aom);
	void setup( World *w);
	
	void calcFrequency();
	void calcVolume(const Player *player);	
	void setPhase(const float newphase){
		phase = newphase;
	}
	void update();
	void draw();
	
	
	
};