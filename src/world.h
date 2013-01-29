/*
 *  world.h
 *  577 Rhea
 *
 *  Created by andré sier on 20130126.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h"
#include "Planet.h"
#include "Player.h"
#include "Goodies.h"
#include "Debris.h"
#include "BlackHole.h"

static int menuinfo = 0;

class World {
public:
	
	Player				    player;
	vector<Planet *>		planets;
	vector<Debris *>		debries;
	vector<Goodie *>		goodies;
	BlackHole				blackhole;
	
	int						gamestate;
	int						level;
	int						goodiesToCollect, goodgoodies;
	bool					leveldone, levelcomplete;
	
	// space
	ofPoint				camLoc;
	ofPoint				worldBoundsMin, 
						worldBoundsMax, 
						worldCenter, 
						worldDim; 
	float				worldRad;
	
	vector<float>			audioline;
	ofMutex					audiomutex;
	float					audiovolumecopy;
	
	ofSoundPlayer		thrust, 
						goodie0, goodie1, goodie2,
						explo0, explo1, explo2;
	ofTrueTypeFont		fontbig, fontsmall;
	
	
	void setup(){
		audioline.assign(256, 0.0f);
		gamestate = 0;
		audiovolumecopy = 0.0f;
		
		thrust.loadSound("sounds/feadi_thrust_01.wav", true);
		thrust.setVolume(0.);
		thrust.setLoop(true);
		thrust.play();
		
		goodie0.loadSound("sounds/feadi_success_01.wav", true);
		goodie0.setMultiPlay(true);
		goodie0.setVolume(1);
		goodie0.play();

		explo0.loadSound("sounds/feadi_explosion_01.wav", true);
		explo0.setMultiPlay(true);
		explo0.setVolume(1);
//		explo0.play();
		
		fontbig.loadFont("04B_03B_.TTF", 52);
//		fontsmall.loadFont("04B_03B_.TTF", 10);
		
		
		level = 0;
		leveldone = levelcomplete = false;
		
		setupWorld();
		player.setup(this);
		camLoc.set(worldCenter.x, worldCenter.y, 1800);
		buildGalaxy();
		makeGoodies();
	}
	
	void setupWorld(){
		int level1 = CLAMP(level + 1, 1, 7)*5000;
		worldBoundsMin.set(0,0,0);
//		worldBoundsMax.set(15000,15000,0);
		worldBoundsMax.set(level1,level1,0);
		worldDim = worldBoundsMax - worldBoundsMin;
		worldCenter = (worldBoundsMax + worldBoundsMin) * 0.5;
		worldRad = worldBoundsMax.y - worldCenter.y;		
	}
	
	void setMenuInfo(){
		menuinfo = 0;
	}
	
	ofPoint getRndPointInWorld(){
		return ofPoint( ofRandom(worldBoundsMin.x, worldBoundsMax.x),
					   ofRandom(worldBoundsMin.y, worldBoundsMax.y), 0);
	}
	ofPoint getRndPointIn80World(){
		float xw = worldDim.x * 0.2;
		float yw = worldDim.y * 0.2;
		return ofPoint( ofRandom(worldBoundsMin.x+xw, worldBoundsMax.x-xw),
					   ofRandom(worldBoundsMin.y+yw, worldBoundsMax.y-yw), 0);
	}
	
	void buildGalaxy(){
		
		while(planets.size()>0){
			delete planets[0];
			planets.erase(planets.begin());
		}
		
		planets.clear();
		blackhole.setup(this);
				
		int level1 = CLAMP(level + 1, 1, 100)*2;
		float scopr =  2.05f + level * 1.61f;//2.5;
		scopr = CLAMP(scopr, 2, 10);
		
		int n = level1 > 100 ? ofRandom(level1*0.7, level1*1.3) : level1;
		//,(int) ofRandom(2,50);
		
		for(int i=0; i<n; i++){
			Planet * p = new Planet();
			p->setup(this, scopr);
			p->calcVolume(&player);
			
			planets.push_back(p);
		}
		
		//1 green
		Planet * p0 = new Planet();
		p0->setup(this, scopr);
		p0->setup(this, p0->pos, p0->rad, 0, scopr);
		p0->calcVolume(&player);
		
		planets.push_back(p0);
		
	}
	
	
	
	void makeGoodies(){
		
		goodgoodies = 0;
		
		while(goodies.size()>0){
			delete goodies[0];
			goodies.erase(goodies.begin());
		}

		int level1 = CLAMP(level + 1, 1, 100)*2;
		
		
		int n = level1 > 4 ? ofRandom(level1*0.7, level1*1.3) : level1;

		//int n = (int) ofRandom(2,50);
		
		for(int i=0; i<n; i++){
			Goodie * g = new Goodie();
			g->setup(this);
			if(g->good){
				goodgoodies++;
			}
			goodies.push_back(g);
		}
		
		
		if(goodgoodies<=1){
			makeGoodies();
		}
		
//		goodiesToCollect = (int) (goodgoodies *ofRandom(0.5,0.7) + 1);
//		float maxgoodies = 
		goodiesToCollect = (int) (goodgoodies *ofRandom(0.5,0.8) + 1);
		
		
	}
	
	void playGoodieSnd(){
		goodie0.setSpeed(ofRandom(0.9,1.1));
		goodie0.play();
		player.heal();
		goodiesToCollect--;
		if(goodiesToCollect==0){
			leveldone = true;
			blackhole.mayenter = true;
			blackhole.hiblink = 5;
		}
		player.numCollected++;
	}
	void playExploSnd(){
		explo0.setSpeed(ofRandom(0.9,1.1));
		explo0.play();
	}
	
	void makeDebris(const ofPoint &loc, const float speed){
		/// 
		player.hurt();
		
		///
		ofPoint baseLoc = loc;
		ofPoint currentloc, vel;
		
		int n = ofRandom(5,15);// * speed;
		
		float angleinc = TWO_PI / n;
//		float rad = ofRandom(50,200) * speed;
		float rad = ofMap(speed,0,25,250,550);// * speed;
		
		for(int i=0; i<n; i++){
			Debris * d = new Debris();
			
			float angle = i*angleinc;
			vel.set( cosf(angle)*rad, sinf(angle)*rad, 0);
			currentloc.x = vel.x + baseLoc.x;
			currentloc.y = vel.y + baseLoc.y;
			
			vel.normalize();
			vel *= 0.5;
			
			d->setup(this, currentloc, vel);
			
			debries.push_back(d);
			
		}
		
		playExploSnd();
		
	}
	
	
	void update(){
		
		//if(gamestate==1||gamestate==3||gamestate==4){	
		if(gamestate==1){	
			player.update( );
			thrust.setVolume(ofMap(player.speed, 0, 55, 0.01, 2));
		}
		
		
//		audiomutex.lock();
		for(int i=0; i<planets.size(); i++){
			planets[i]->calcVolume( &player );
		}
		blackhole.calcVolume(&player);
//		audiomutex.unlock();
		

		for(int i=0; i<debries.size(); i++){
			debries[i]->update( );
			if(debries[i]->lifetime <= 0){
				
				delete debries[i];
				debries.erase( debries.begin() + i);
			}
		}
		
		
		// cam	
		camLoc.x += (player.pos.x - camLoc.x) * 0.052;
		camLoc.y += (player.pos.y - camLoc.y) * 0.052;
		if(player.koliding){
			camLoc.x += ofRandom(-player.colide, player.colide);
			camLoc.y += ofRandom(-player.colide, player.colide);
		}
		
		
		float target = ofMap(ABS(player.speed), 0, 55, 1200, 10000);
		target = CLAMP(target, 1000, 5500);
		camLoc.z += (target - camLoc.z) * 0.00725;
		
		
		if(gamestate==1 && player.lives && blackhole.mayenter){
			float d = player.pos.distanceSquared(blackhole.pos);
			if( d <  10){ //5 ){ //10
				advanceLevel();
			}
		}

		
		if(gamestate==1 && !player.lives){
			gamestate = 3;
		}

		if(gamestate==1 && player.lives && level > 10){ // limit at 10
			gamestate = 4;
		}
		
	}
	
	void draw(){
		
		ofBackground(34);
		
		
		gluLookAt(camLoc.x, camLoc.y,camLoc.z, camLoc.x, camLoc.y, 0, 0, 1, 0);

		
		ofSetColor(0);
		
		/// draw bounds
		ofNoFill();
		ofRect(worldBoundsMin.x, worldBoundsMin.y,
			   (worldBoundsMax.x-worldBoundsMin.x), 
			   (worldBoundsMax.y-worldBoundsMin.y));
		
		
	//	audiomutex.lock();

		for(int i=0; i<planets.size();i++){			
			planets[i]->draw();
		}
		blackhole.draw();
		for(int i=0; i<goodies.size();i++){			
			goodies[i]->draw();
		}
		ofSetColor(0);
		for(int i=0; i<debries.size(); i++){
			debries[i]->draw( );
		}
		
		

	//	audiomutex.unlock();

		
		player.draw();
		
//		ofSetColor(255,0,0);
//		ofCircle(camLoc.x, camLoc.y, 20);
		
		ofSetupScreen();

		ofSetColor(0,255,128);		
		ofRect(0,0, player.energy*ofGetWidth(), 25);
		ofSetColor(0, 255, 0);		
		ofRect(0,25, ofMap(goodiesToCollect, 0, goodgoodies, 0,  ofGetWidth()), 25);
		if(goodiesToCollect==0 && ofGetFrameNum()%10==0){
			ofRect(0,25,  ofGetWidth(), 25);				
		}
		
		ofSetColor(0,50,25);		
		float y = 70;
		ofDrawBitmapString("577 Rhea", 5,y); y+= 10;
		ofDrawBitmapString("level " +ofToString(level), 5,y); y+= 10;
		ofDrawBitmapString("energy " +ofToString(player.energy), 5,y); y+= 10;
		ofDrawBitmapString("distance " +ofToString(player.distanceTravelled), 5,y);y+= 10;		
		ofDrawBitmapString("musicvolume " +ofToString(audiovolumecopy), 5,y);y+= 10;
		
		ofSetColor(0, 255, 0);
		if(gamestate==0){
			drawInfo();
		}
		if(gamestate==2){
			drawLevelUp();
		}
		if(gamestate==3){
			drawGameOver();
		}
		if(gamestate==4){
			drawWin();
		}
		
//		ofDrawBitmapString("fps:" +ofToString(ofGetFrameRate())+"\ncam:"+ofToString(camLoc)+
//						   "\naccPlanets"+ofToString(player.accPlanets)+
//						   "\naccHeading"+ofToString(player.accHeading)+
//						   "\nacc"+ofToString(player.acc)+
//						   "\nspeed angle"+ofToString(player.speed)+" "+ofToString(player.angle)
//						   +" kl: "+ofToString(player.kleft)+" kr: "+ofToString(player.kright)
//						   
//						   , 5, ofGetHeight()-75);
	}

	
	
	
	void drawInfo(){
		if(ofGetFrameNum()%60==0){
			menuinfo = (menuinfo+1) % 18;
		}
		
		string info[18]={
			"577 RHEA",
			" ",
			"PRESS ANY KEY\n TO BEGIN", 
			" ",
			"CURSOR or \nWASD keys\n+- VOLUME",
			" ",
			"SAVE YOUR EARTH",
			" ",
			"COLLECT ESSENCES", // \nTIMETRAVEL \nTHRU \nBLACKHOLE",
			" ",
			"TIMETRAVEL\nTHRU\nBLACKHOLE",
			" ",
			"FIX HOME",
			" ",
//			"577 RHEA\nAndre Sier + \nFerdinand Meier + \nMauricio Martins \n s373.net/x production \n20130127 #GGJ2013",
			"577 RHEA\nAndre Sier + \nFerdinand Meier + \nMauricio Martins \n s373.net/x production \n20130127 #GGJ2013",
			" ",
			"577 RHEA\nAndre Sier + \nFerdinand Meier + \nMauricio Martins \ns373.net/x production \n20130127 #GGJ2013",
			" "
		};
		
		fontbig.drawString(info[menuinfo], ofGetWidth()/5, ofGetHeight()/2);
		
	}
	
	void drawLevelUp(){
		if(ofGetFrameNum()%60==0){
			menuinfo = (menuinfo+1) % 10;
		}
		
		string info[10]={
			"YOU WIN\nLEVEL "+ofToString(level-1),
			"",
			"TRAVELLED "+ofToString(player.distanceTravelled),
			"",
			"GETREADY\nLEVEL "+ofToString(level),
			"",
			"PRESS ANY KEY\n TO BEGIN", //any key\nto begin",
			"",
			"577 RHEA",
			""
		};
		
		fontbig.drawString(info[menuinfo], ofGetWidth()/5, ofGetHeight()/2);
		
	}

	void drawGameOver(){
		if(ofGetFrameNum()%60==0){
			menuinfo = (menuinfo+1) % 14;
		}
		
		string info[14]={
			"GAME OVER",
			"",
			"YOU REACHED\nLEVEL "+ofToString(level),
			"",
			"TRAVELLED\n"+ofToString(player.distanceTravelled), //any key\nto begin",
			"",
			"COLLECTED "+ofToString(player.numCollected)+"\nESSENCES"
			"",
			"YOUR WORLD IS DEAD"
			"",
			"FOLD SPACE\nTRY AGAIN",
			"",
			"PRESS SPACE TO START",
			"",
		};
		
		fontbig.drawString(info[menuinfo], ofGetWidth()/5, ofGetHeight()/2);
		
	}
	
	void drawWin(){
		if(ofGetFrameNum()%60==0){
			menuinfo = (menuinfo+1) % 14;
		}
		
		string info[14]={
			"YOU WIN!!",
			"",
			"YOU REACHED\nLEVEL "+ofToString(level),
			"",
			"TRAVELLED\n"+ofToString(player.distanceTravelled), //any key\nto begin",
			"",
			"COLLECTED "+ofToString(player.numCollected)+"\nESSENCES"
			"",
			"YOUR WORLD WILL ENDURE!"
			"",
			"CONGRATULATIONS!"
			"",
			"CONGRATULATIONS!\nYOU BEAT 577 RHEA!",
			""
		};
		
		fontbig.drawString(info[menuinfo], ofGetWidth()/5, ofGetHeight()/2);
		
	}
	
	void advanceLevel(){
		level ++;
		
		//regenerate world and
		setupWorld();
		buildGalaxy();
		makeGoodies();
		player.distanceTravelled += 1;
		player.heal();
//		player.heal();
		player.accPlanets.set(0,0,0);
		// 
		gamestate = 2;
	}
	
	void resetPlayer(){
		player.pos = getRndPointInWorld();
		player.energy = 1.0f;
		player.lives = true;
		player.distanceTravelled = 0.0f;
		player.numCollected = 0;
		level = 0;
		
		setupWorld();
		buildGalaxy();
		makeGoodies();
	}
	
	float * processAudio(){
		
		
//		audiomutex.lock();

		
		for (int i = 0; i < 256; i++){
			
			audioline[i] = 0.0f;					
		
			if(planets.size()>0){
						
				for(int k=0; k<planets.size(); k++){			
						
					Planet * p = planets[k];										
					p->phase += p->phaseinc;						
							
					if(!p->audioom){
								
						if(p->phase>TWO_PI) { p->phase -= TWO_PI; }
						
						audioline[i] += cosf(p->phase)*p->volume;
								
					} else {
								
						if(p->phase>1) { 
							p->phase -= 1.0f; 
							p->source = p->target;
							p->target = ofRandom(-1,1);
							p->delta = p->target - p->source;				
						}				
						
						audioline[i] += (p->source + p->phase * p->delta) * p->volume;	
								
					}
							
							
				}		
			}
		
		}
	//	audiomutex.unlock();

		return &audioline[0];
		
	}
	
	
	
	void clean(){
		while(goodies.size()>0){
			delete goodies[0];
			goodies.erase(goodies.begin());
		}

		while(debries.size()>0){
			delete debries[0];
			debries.erase(debries.begin());
		}

		while(planets.size()>0){
			delete planets[0];
			planets.erase(planets.begin());
		}
		
	}
	
	
};