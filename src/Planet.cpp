/*
 *  Planet.cpp
 *  577Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Planet.h"

#include "world.h"
#include "Player.h"

void Planet::setup( World *w, const float scopr){
	world = w;
	ofPoint loc = w->getRndPointIn80World();
	radmin = 70;
	radmax = 550;
	float r = ofRandom(radmin,radmax);
	
	bool aom = ofRandom(1)<0.5?true:false;
	setup(w, loc, r, aom, scopr);
	
}
void Planet::setup( World *w, const ofPoint & loc, const float rad, const bool aom, const float scopr){
	radmin = 70;
	radmax = 550;
	rot = 0;
	rotspeed = ofRandom(1e-2, 1);
	
	world = w;
	pos = loc;
	this->rad = rad;
	audioom = aom;

	this->scopr = scopr;
	diameter = 2*rad;
	radinfluence = scopr*rad;//25*rad;
	radinfluencesquared = radinfluence*radinfluence;
	
	volume = phase = phaseinc = frequency = 0.0f;
	calcFrequency();

//	animationTime = 0;
//	modelscale = 1;
//	
////	if(model.loadModel("astroBoy_walk.dae",true)){
//	model.loadModel("models/planet02.3ds",1);
////	model.loadModel("planet02.3ds",1);
//	model.setPosition(pos.x, pos.y, pos.z);
//	model.setScale(rad*100,rad*100,rad*100);
	
//	if(model.loadModel("planet02.3ds",1)){
////    	model.setAnimation(0);
//    	model.setPosition(pos.x, pos.y, pos.z);
////		modelscale = model.getNormalizedScale();
//		
//	}
	
//	
//	if(model.loadModel("planet02.dae",true)){
////		cout << "read astroboy" << endl;
//    	model.setAnimation(0);
//    	model.setPosition(pos.x, pos.y, pos.z);
//		modelscale = model.getNormalizedScale();
//		
//	}
	
//	if(model.loadModel("astroBoy_walk.dae",true)){
//    	model.setAnimation(0);
//    	model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
//    	//model.createLightsFromAiModel();
//    	//model.disableTextures();
//    	//model.disableMaterials();
//		
//    	mesh = model.getMesh(0);
//    	position = model.getPosition();
//    	normScale = model.getNormalizedScale();
//    	scale = model.getScale();
//    	sceneCenter = model.getSceneCenter();
//    	material = model.getMaterialForMesh(0);
//        tex = model.getTextureForMesh(0);
//    }
	
	
}

void Planet::calcFrequency( ){
//	frequency = ofMap(rad, radmin,radmax, 100, 5000);
	if(!audioom){
		frequency = ofMap(rad, radmin,radmax, 1050, 70);
		phaseinc = frequency/44100.0f*TWO_PI;
	} else {
		frequency = ofMap(rad, radmin,radmax, 100, 500);
		phaseinc = frequency/44100.0f;			
	}
}


void Planet::calcVolume(const Player *player){
	float maxvol = 1;
	float distsq = pos.distanceSquared(player->pos);
	volume = ofMap(distsq, 0, radinfluencesquared, maxvol, 0.01, true);
}


void Planet::update(){
	
}

void Planet::draw(){
	
//	if( true ){
//		animationTime += ofGetLastFrameTime();
//		if( animationTime >= 1.0 ){
//			animationTime = 0.0;
//		}
//	    model.setNormalizedTime(animationTime);
//		mesh = model.getCurrentAnimatedMesh(0);
//	}

//	model.setRotation(0, ofGetFrameNum()*0.05, 0, 1, 0);

	rot+=rotspeed;
	
	if(audioom) ofSetColor(0);
	else ofSetColor(100,200,0);
	
	ofPushMatrix();
	ofTranslate(pos);
	ofRotateZ(rot);
	ofNoFill();
	ofCircle(0,0, radinfluence);
	ofFill();
	ofCircle(0,0, rad);
	
	ofPopMatrix();
	
//	ofPushMatrix();
//	ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
//	ofRotate(-ofGetMouseX(), 0, 1, 0);
//	ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

//	float s = modelscale*rad;
//	ofScale(s,s,s);
//	model.drawFaces();
//	model.draw();
	
//    ofPopMatrix();
	
	
//	ofSetColor(128, 255, 0);
//	ofDrawBitmapString(ofToString(volume), pos.x, pos.y);
}
