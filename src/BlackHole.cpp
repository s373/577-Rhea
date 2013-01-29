/*
 *  BlackHole.cpp
 *  577Rhea
 *
 *  Created by andré sier on 20130127.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "BlackHole.h"


#include "world.h"
#include "Player.h"

void BlackHole::setup( World *w){
	world = w;
	ofPoint loc = w->getRndPointIn80World();
	radmin = 370;
	radmax = 750;
	
	hiblink = 30;
	white = mayenter = false;
	pulse = 0;
	pulsespeed = ofRandom(5,15);
	
	float r = ofRandom(radmin,radmax);
	
	bool aom = false;
	setup(w, loc, r, aom);
	
}
void BlackHole::setup( World *w, const ofPoint & loc, const float rad, const bool aom){
	radmin = 370;
	radmax = 750;
	rot = 0;
	rotspeed = ofRandom(1e-1, 2);
	
	world = w;
	pos = loc;
	this->rad = rad;
	audioom = aom;
	
	diameter = 2*rad;
	radinfluence = 2.5*rad;//25*rad;
	radinfluencesquared = radinfluence*radinfluence;
	
	volume = phase = phaseinc = frequency = 0.0f;
	calcFrequency();
	
}

void BlackHole::calcFrequency( ){
	//	frequency = ofMap(rad, radmin,radmax, 100, 5000);
	if(!audioom){
		frequency = ofMap(rad, radmin,radmax, 1050, 70);
		phaseinc = frequency/44100.0f*TWO_PI;
	} else {
		frequency = ofMap(rad, radmin,radmax, 100, 500);
		phaseinc = frequency/44100.0f;			
	}
}


void BlackHole::calcVolume(const Player *player){
	float maxvol = 1;
	float distsq = pos.distanceSquared(player->pos);
	volume = ofMap(distsq, 0, radinfluencesquared, maxvol, 0.01, true);
}


void BlackHole::update(){
	
}

void BlackHole::draw(){
	
	
	rot+=rotspeed;
	
	pulse += pulsespeed;
	if(pulse > radinfluence){
		pulse = 0;
	}

	if(ofGetFrameNum()%hiblink==0){
		white^=true;
	}
	
	
	if(white) ofSetColor(255);
	else ofSetColor(0);
	
	ofPushMatrix();
	ofTranslate(pos);
	ofRotateZ(rot);
	ofNoFill();
	ofCircle(0,0, radinfluence);
	ofFill();
	if(mayenter)
		ofCircle(0,0, rad+pulse);
	else
		ofCircle(0,0, rad);
	ofPopMatrix();
	
	
	
//	ofSetColor(128, 255, 0);
//	ofDrawBitmapString(ofToString(volume), pos.x, pos.y);
}
