/*
 *  Planet.h
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

//#include "ofx3DModelLoader.h"
//#include "ofxAssimpModelLoader.h"
//#include "ofVboMesh.h"

class World;
class Player;

class Planet {
public:
	
	ofPoint pos;
	
	float rad, diameter, radmin, radmax;
	float radinfluence, radinfluencesquared;
	float rot, rotspeed, scopr;

	World *world;

	int	  audioom;
	float phase, phaseinc, frequency, volume;
	float source, target, delta;

	
//	ofx3DModelLoader model;
//	//ofxAssimpModelLoader model;
//	float animationTime, modelscale;	
//	ofVboMesh mesh;
	
	
	
	
	Planet(){
		world = NULL;
	}
	void setup( World *w, const ofPoint & loc, const float rad, const bool aom, const float scopr);
	void setup( World *w, const float scopr);

	void calcFrequency();
	void calcVolume(const Player *player);	
	void setPhase(const float newphase){
		phase = newphase;
	}
	void update();
	void draw();
	
};