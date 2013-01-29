/*
 *  Player.cpp
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Player.h"

#include "world.h"
#include "Planet.h"
#include "BlackHole.h"
#include "Goodies.h"


void Player::setup( World *w){
	world = w;
	pos = world->getRndPointIn80World();
	vel.set(0,0,0);
	acc.set(0,0,0);
	accPlanets.set(0,0,0);
	accHeading.set(0,0,0);	
	
	rad = 50;
	angle = 0.0f;//ofRandom(TWO_PI);
	speed = colide = 0;
	
	force = energy = 1.0f;
	
	kup = kdown = kleft = kright = koliding = false;
	lives = true;
	
	numCollected = 0;
}

void Player::calcGrav(){
//	accPlanets.set(0,0,0);
	koliding = false;

//	cout << "world->planets.size()" << world->planets.size() << endl;
	
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
		
		if(dsquared <= rsquared) { // && dsquared > 0){
			koliding = true;
			dsquared = -1*dsquared;
			
		}
		
//		float f = 5.25 * p->rad * rad / dsquared;
		float f = p->rad * rad / dsquared;
			
		delta *= f;
			
		accPlanets += delta;
//		cout << "adding delta to acc planets " << ofToString(delta) << endl;
				
	}
	
	//calc blackhole grav
	{
		
		BlackHole & black = world->blackhole;
		
		ofPoint delta;
		delta.x = black.pos.x - pos.x;		
		delta.y = black.pos.y - pos.y;		
		
		float dsquared = delta.x*delta.x + delta.y*delta.y;
		float rsquared = rad*rad + black.rad*black.rad;
		
			
		if(!black.mayenter && dsquared <= rsquared) { // && dsquared > 0){
			koliding = true;
			dsquared = -1*dsquared;
			
		}
		
	
		if(dsquared > black.radinfluencesquared){
			return;
		}
		
		float f = black.rad * rad / dsquared;
		
		delta *= f;
		
		accPlanets += delta;
		
	}
	
}


void Player::update(){
	acc.set(0,0,0);
	accPlanets.set(0,0,0);
	accHeading.set(0,0,0);
	
	ofPoint ppos = pos;
	
	calcGrav();	

	if(koliding){
		colide += ofRandom(10);//0.01 + colide;
		if(ofRandom(1)<0.11){
			world->makeDebris(pos, speed);
		}
	} else {
		colide = 0;
	}
	
//	accPlanets.limit(20);
	
	acc += accPlanets;
	
//	float accPlanetsLen = accPlanets.length();
	
	acc.limit(15);
	
	float af = 0.05;
	if(kleft){
		angle-=af;
	}
	if(kright){
		angle+=af;
	}
	if(lives && kup){
		speed += 1.27;
		energy -= speed * 0.00001;
	}
	if(kdown){
		speed -= 0.05;
	}
	speed *= 0.965;
	speed = CLAMP(speed, -55, 55);
	
	accHeading.set( cosf(angle)*speed, sinf(angle)*speed, 0);
	
	//headingvel += (headingForce-headingvel) *0.073;
	
	acc += accHeading;//headingvel;//headingForce;
	
	vel += acc;
	
	vel.limit(20);
	pos+=vel;
	
	if(lives){
	float distt = ABS(pos.x-ppos.x) + ABS(pos.y -ppos.y);
	distanceTravelled += distt * 1e-4;
	}
	
	bound();
	
	findGoodies();
}

void Player::bound(){
	float w = world->worldDim.x;
	float h = world->worldDim.y;
	if(pos.x<world->worldBoundsMin.x) { pos.x += w; }
    if(pos.x>world->worldBoundsMax.x){ pos.x -= w; }
    if(pos.y<world->worldBoundsMin.y){ pos.y += h; }
    if(pos.y>world->worldBoundsMax.y){ pos.y -= h; }
	
}
void Player::findGoodies(){
	
	for(int i=0; i<world->goodies.size(); i++){
		Goodie * g = world->goodies[i];
		
		ofPoint delta = g->pos - pos;
		float dsquared = delta.squareLength();
		float rsquared = rad*rad + g->rad*g->rad;
		if( dsquared > rsquared){
			continue;
		} else
		{
			
			if(g->good){
				world->playGoodieSnd();
			} else {
				world->playExploSnd();
			}
				
			/// add energy to player or delete energy
			world->makeDebris(g->pos, speed);

			/// delete it
			delete g;
			world->goodies.erase(world->goodies.begin()+i);
			i--;
		}
		
	}
	
}

void Player::hurt(){
	energy-= ofRandom(5e-4,1e-2);
	if(energy<0){
		energy = 0;

		lives = false;
		
	}
	
}

void Player::heal(){
//	energy+= ofRandom(1e-2,2e-2);
	energy+= ofRandom(0.1,0.2);
	if(energy>1){
		energy = 1;
		
//		lives = false;//ha
		
	}
	
}




void Player::draw(){
	
	float a = 7.5f;//2.5f;//250.5;
	if(koliding)
		ofSetColor(255,0,0);
	else
		ofSetColor(0,255,0);
	
	
	ofPushMatrix();
	ofTranslate(pos);
	ofRotateZ(angle * RAD_TO_DEG);
//	ofTriangle(-rad*0.7, rad, rad*0.7, rad, 0, -rad);
	ofTriangle(-rad, rad*0.7, -rad, -rad*0.7, rad, 0);
	ofSetColor(0);
	ofDrawBitmapString("577 Rhea", 0,0);
	ofPopMatrix();
	
//	ofCircle(pos.x,pos.y, rad);
	ofSetColor(0, 100);
	//	ofLine(pos.x, pos.y, pos.x+vel.x*a, pos.y*vel.y*a);
	ofLine(pos.x, pos.y, pos.x+vel.x*a, pos.y+vel.y*a);

//	ofSetColor(120);

//	ofSetColor(255,0,0);
//	ofLine(pos.x, pos.y, pos.x+acc.x*a, pos.y+acc.y*a);
//	ofSetColor(255,128,0);
//	ofLine(pos.x, pos.y, pos.x+accHeading.x*a, pos.y+accHeading.y*a);
	ofSetColor(0,0,255, 100);
	ofLine(pos.x, pos.y, pos.x+accPlanets.x*a, pos.y+accPlanets.y*a);

	
	
	if(kup){
	
//		ofSetColor( ofRandom(100,255), ofRandom(10,50), ofRandom(255) );
		ofSetColor( ofNoise(ofGetFrameNum()*0.025)*255, 
				   ofNoise(ofGetFrameNum()*0.005)*255, 
				   ofNoise(ofGetFrameNum()*0.007)*255);

		int num = CLAMP(speed, 1, 5);
		for(int i=0; i<num; i++){
		ofLine(pos.x, pos.y, 
			   pos.x + accHeading.x * -10 + ofRandom(-speed,speed),
			   pos.y + accHeading.y * -10 + ofRandom(-speed,speed));
		}
		
	}
	
//	headingvel
	
}
