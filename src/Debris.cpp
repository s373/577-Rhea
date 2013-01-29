/*
 *  Debris.cpp
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Debris.h"

#include "world.h"
#include "Planet.h"

void Debris::setup(World *w, const ofPoint & loc, const ofPoint & velocidade){
	world = w;
	pos = loc;
	vel = velocidade;
	acc.set(0,0,0);

	rad = 10;
	lifetime = 1.0f;
	lifedec = ofRandom(1e-3,2e-3);
	linemult = 2.5;
}
void Debris::update(){
	lifetime -= lifedec;
	
	acc.set(0,0,0);
	for(int i=0; i < world->planets.size(); i++){
		Planet * p = world->planets[i];

		ofPoint delta;
		delta.x = p->pos.x - pos.x;		
		delta.y = p->pos.y - pos.y;		
		
		float dsquared = delta.x*delta.x + delta.y*delta.y;
		float rsquared = rad*rad + p->rad*p->rad;
		
		if(dsquared > p->radinfluencesquared){
			continue;
		}
		
		if(dsquared <= rsquared) { 			
			dsquared = -1*dsquared;			
		}
		
		float f = 100 * p->rad * rad / dsquared;
		
		delta *= f;
		
		acc += delta;
		
		
	}
	
	//calc blackhole grav
	{
		
		BlackHole & black = world->blackhole;
		
		ofPoint delta;
		delta.x = black.pos.x - pos.x;		
		delta.y = black.pos.y - pos.y;		
		
		float dsquared = delta.x*delta.x + delta.y*delta.y;
		float rsquared = rad*rad + black.rad*black.rad;
		
		
//		if(!black.mayenter && dsquared <= rsquared) { // && dsquared > 0){
		if(dsquared <= rsquared) { 			
			dsquared = -1*dsquared;
			
		}
		
		
		if(dsquared > black.radinfluencesquared){
			;
		} else {
		
		float f = black.rad * rad / dsquared;
		
		delta *= f;
		
		acc+= delta;
		
		}
		
	}
	
	
	
	
	
	acc.limit(5);

	vel += acc;
	pos += vel;
	
	
}
void Debris::draw(){
	
	ofSetColor(255, lifetime *200 + 50);
	ofLine( pos.x, pos.y, pos.x + vel.x*linemult, pos.y + vel.y*linemult );
	
}
