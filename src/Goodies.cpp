/*
 *  Goodies.cpp
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Goodies.h"

#include "world.h"
#include "Planet.h"


string goodimgs[4] = {
		"greenrecycle.png", "ocean.png", "tree.png", "windturbine.png"
};
string badimgs[4] = {
	"industry.png", "petrol.png", "grey_car.png", "shopcart.png"
};

void Goodie::setup(World *w){
	world = w;
	planet = world->planets[(int)ofRandom(world->planets.size())];
	
	active = true;
	good = !planet->audioom;
	energy = 1;
	
	if(good){
		img.loadImage("PNG32pix/"+goodimgs[(int)ofRandom(4)]);
	} else {
		img.loadImage("PNG32pix/"+badimgs[(int)ofRandom(4)]);
	}
	
	rad = ofRandom(15, 50);
	rad = ofRandom(50, 150);
	
	//pos calc
	angle = ofRandom(TWO_PI);
	radfromplanet = ofRandom(planet->rad, planet->rad*2.1);	
	pos.x = cosf(angle)*radfromplanet + planet->pos.x; 
	pos.y = sinf(angle)*radfromplanet + planet->pos.y; 
	
	pulse = 0.0f;
	pulseinc = ofRandom(0.01,0.05);
}
void Goodie::update(){
	
}
void Goodie::draw(){

	pulse += pulseinc;
	if(pulse>1||pulse<0) pulseinc = -pulseinc;
	float al = pulse*255;
	if(good){
		ofSetColor(10,255,0, al);
	} else {
		ofSetColor(127, al);
	}

	ofCircle(pos.x, pos.y, rad);
	img.draw(pos.x-rad, pos.y-rad, rad*2, rad*2);
}
