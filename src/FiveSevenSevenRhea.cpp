#include "FiveSevenSevenRhea.h"



void FiveSevenSevenRhea::setup(){	 	
	ofSetVerticalSync(true);
	ofSetCircleResolution(8);
	
	world.setup();
		
	bufferSize=256;
	sampleRate=44100;
	mainvolume=0.55;
	world.audiovolumecopy = mainvolume;


	audioline.assign(bufferSize, 0.0f);
	ofEnableAlphaBlending();
		
}

void FiveSevenSevenRhea::exit(){
	
	soundstream.close();
	world.clean();	
	
}


void FiveSevenSevenRhea::update(){
	
	if(ofGetFrameNum()==10)
		soundstream.setup(this, 2, 0, sampleRate, bufferSize, 4);
	

	world.update();
}



void FiveSevenSevenRhea::draw(){
	world.draw();
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void FiveSevenSevenRhea::keyPressed  (int key){ 
				
	
	if(key=='+'){
		mainvolume += 0.1;
		world.audiovolumecopy = mainvolume;
	}
	if(key=='-'){
		mainvolume -= 0.1;
		world.audiovolumecopy = mainvolume;
	}
	
	if(world.gamestate==1){

		if( key == 'g' ){
			world.makeGoodies();
		}

		if( key == 'a' || key == OF_KEY_LEFT ){
			world.player.kleft = true;
		}
		if( key == 'd' || key == OF_KEY_RIGHT){
			world.player.kright = true;
		}
		if( key == 's' || key == OF_KEY_DOWN) {
			world.player.kdown = true;
		}
		if( key == ' ' || key == 'w' || key == OF_KEY_UP ){
			world.player.kup = true;
		}
		
	}
}

//--------------------------------------------------------------
void FiveSevenSevenRhea::keyReleased(int key){ 
	
	//should turn on soundstream and restart to prevent clicks
	if(world.gamestate==0){
		world.gamestate = 1;
		world.setMenuInfo();
		return;
	}
	
	if(world.gamestate==2){
		world.gamestate = 1;
		world.setMenuInfo();
		return;
	}
	if( (world.gamestate==3 || world.gamestate==4) && key == ' '){
		world.resetPlayer();
		world.gamestate = 0;
		world.setMenuInfo();
		return;
	}
	
	
	if(world.gamestate==1){
		if( key == 'a' || key == OF_KEY_LEFT ){
			world.player.kleft = false;
		}
		if( key == 'd' || key == OF_KEY_RIGHT){
			world.player.kright = false;
		}
		if( key == 's' || key == OF_KEY_DOWN) {
			world.player.kdown = false;
		}
		if( key == ' ' || key == 'w' || key == OF_KEY_UP ){
			world.player.kup = false;
		}
		
		if( key == 'n' ){
			world.buildGalaxy();
		}
	}
}

//--------------------------------------------------------------
void FiveSevenSevenRhea::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void FiveSevenSevenRhea::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void FiveSevenSevenRhea::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void FiveSevenSevenRhea::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void FiveSevenSevenRhea::windowResized(int w, int h){

}

//--------------------------------------------------------------
void FiveSevenSevenRhea::audioOut(float * output, int bufferSize, int nChannels){
	
//	soundmutex.lock();
		
	float *audio = world.processAudio();
	
	for (int i = 0; i < bufferSize; i++){
		float s = audio[i] * mainvolume;
		output[i*nChannels] = s;
		output[i*nChannels+1] = s;
	}
	
	
	
}
