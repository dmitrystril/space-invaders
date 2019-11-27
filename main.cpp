//main.cpp

#include <stdio.h> 
#include <stdlib.h>
#include "SDL\SDL.h"  
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "csprite.h"
#include "cspritebase.h"   
#include "ctextmsg.h"
#include "cspritealienship.h"  
#include "cspritemissile.h"  
 
void InitObjects();
SDL_Surface *LoadImage(const char *file); 
void DrawImage(int x, int y, SDL_Surface* source, SDL_Surface* destination);
void DrawScene(); 
void ShowSecondaryScreen(int num);
void CleanUp();

void MoveAliens(const int& delay);  
void DefeatAlien(int x, int y); 
void StartMyMissile();
void MoveMyMissile();
void StartAlienMissiles(const int& delay);
void MoveAlienMissiles();  

void GamePlay(int flag);
void LevelUp(int level);
void RestartObjects();

const int SCREEN_WIDTH = 1024;    
const int SCREEN_HEIGHT = 768; 
const int SCREEN_BPP = 32; 
const int NROWS = 5;          //NROWS*MCOLS=total number of alien ships
const int MCOLS = 8;          //possible min=4  (5x8 max)
const int X_COORD = 160;
const int Y_COORD = 70; 
const int MISSILES_NUM = 15;   //missile[0] is mine
const int HDISTANCE = 90;     //distance among alien ships
const int VDISTANCE = 60;    
 
SDL_Surface *screen, *background;
CSpriteBase myShipbase, sunbase, alienShip1base, alienShip2base, alienShip3base, explodebase, missilebase;
CSprite myShip, sun, explode;
CSpriteAlienShip alienShip[NROWS][MCOLS];
CSpriteMissile missile[MISSILES_NUM];
CTextMsg msgPoints, msgLives, msgDanger, msgLevel, msgPressSpace, msgPopUpLevel, msgWinLoose;
//music
Mix_Music *musicMain=NULL;
Mix_Music *music1=NULL;

//other sound effects
Mix_Chunk *blow1=NULL;
Mix_Chunk *blow2=NULL;
Mix_Chunk *sirena=NULL; 

int step=10,limitN=4, limitM=4,limitMissiles=3,superMissilePossibility=30;
unsigned int lives=3,level=1,points=0,aliensAliveCounter=limitN*limitM;
char tempTextBuffer[20];

//----------------------------------------- 
int main(int argc, char *argv[]){  
  Uint8* keys;
  unsigned long lastMoveMissiles=0;
  int delayMove=700, delayShot=100, delayMoveMissiles=5; 
   
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0){ 
	 fprintf(stderr,"Unable to init SDL: %s\n", SDL_GetError());  
     exit(1); 
  }  
  atexit(CleanUp);  
 
  screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN); 
  if (screen==NULL){ 
	 fprintf(stderr,"Unable to set the video mode: %s\n", SDL_GetError()); 
     exit(1); 
  }
  
  if(TTF_Init()<0){
	 exit(1);
  }
  
  if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)<0)
  {
	 exit(1);
  }
  SDL_ShowCursor(0);
 
  ShowSecondaryScreen(1);
  
  InitObjects();
 
  //..GAME LOOP..//  
  int gameloop=1,pause=0;
  while(gameloop==1)
  {
     SDL_Event event;
	 
     while (SDL_PollEvent(&event)){
        if (event.type==SDL_QUIT){
		   gameloop=0;
		} 
        if (event.type==SDL_KEYDOWN){
          if (event.key.keysym.sym==SDLK_ESCAPE){ 
		     gameloop=0; 
		  }  
          if (event.key.keysym.sym==SDLK_SPACE){ 
			 if(myShip.getVisibility()==1) 
		        if(missile[0].getVisibility()!=1) StartMyMissile();
		  }
		  if (event.key.keysym.sym==SDLK_p){ 
		     if(pause==0) pause=1; else pause=0;
		  }
		}	
	 }  
	 if(pause==1) continue;

     keys=SDL_GetKeyState(NULL);
     if(keys[SDLK_LEFT]){
	    myShip.xadd(-4);     
		if(myShip.getx()<0) myShip.xadd(4);
     }  
     if(keys[SDLK_RIGHT]){    
	    myShip.xadd(4); 
	    if(myShip.getx()>SCREEN_WIDTH-myShipbase.mW) myShip.xadd(-4);
     } 
	 
	 //...........................//
	 MoveAliens(delayMove);	 
	 StartAlienMissiles(delayShot);
	 
     if(lastMoveMissiles+delayMoveMissiles<SDL_GetTicks()) 
     {	 
	    MoveMyMissile();
	    MoveAlienMissiles();
		lastMoveMissiles=SDL_GetTicks();
	 }
	 

	 DrawScene(); 

  }//..END of GAME LOOP..//
  return 0;
}

//-----------------------------------------
void InitObjects(){ 
  int x,y,i,k;
   
  myShipbase.init("data/myShip");
  alienShip1base.init("data/alienShip1");
  alienShip2base.init("data/alienShip2");
  alienShip3base.init("data/alienShip3");
  sunbase.init("data/sun");
  explodebase.init("data/explode");
  missilebase.init("data/missile");
  
  background=LoadImage("data/bg.jpg");
  DrawImage(0,0,background,screen);
  
  myShip.init(&myShipbase,screen);
  myShip.setxy(SCREEN_WIDTH/2-myShipbase.mW,685);
  myShip.setSpeed(0.8); 
  myShip.setVisibility(1);
 
  sun.init(&sunbase,screen);
  sun.setxy(0,SCREEN_HEIGHT-sunbase.mH);
  sun.setSpeed(3.0); 
  
  explode.init(&explodebase,screen);
  explode.setSpeed(0.7);
  explode.setVisibility(0); 
  explode.stopAnim(); //because of [.init()] does mAnimating=1
  
  for(i=0; i<limitMissiles; i++)
  {
     missile[i].init(&missilebase,screen);
     missile[i].setSpeed(0.4);
     missile[i].setVisibility(0); 
	 missile[i].setType(0);
     missile[i].stopAnim();   
  } 
	
  x=X_COORD; 
  y=Y_COORD;
  for (i=0; i<limitN; i++)
  {  
	 for (k=0; k<limitM; k++)   
     {
	    if(i==limitN-1) alienShip[i][k].canShoot(1);
	    else alienShip[i][k].canShoot(0);
	    if(i==0) {alienShip[i][k].init(&alienShip1base,screen);alienShip[i][k].setType(3);}
		else if(i>0 && i<3) {alienShip[i][k].init(&alienShip2base,screen);alienShip[i][k].setType(2);} 
        else {alienShip[i][k].init(&alienShip3base,screen);alienShip[i][k].setType(1);} 			  
        alienShip[i][k].setxy(x,y);
        alienShip[i][k].setSpeed(3.5);  
		alienShip[i][k].setVisibility(1);
		alienShip[i][k].setLastShotTime(SDL_GetTicks());
	    x+=HDISTANCE;
	 } 
	 y+=VDISTANCE;
	 x=X_COORD;
  }  
  
//Message initialization    
    sprintf(tempTextBuffer,"Points %7d      ",points);
	msgPoints.setxy(10,10);
	msgPoints.init("data/fonts/15.ttf", 35, 0, 255, 0, tempTextBuffer, screen);
	msgPoints.saveBGpart();
	msgPoints.draw();
  
    sprintf(tempTextBuffer,"Lives %3d ",lives);	
    msgLives.init("data/fonts/15.ttf", 25, 255, 255, 0, tempTextBuffer, screen);
    msgLives.setxy(10,msgPoints.getX()+msgPoints.getH());
    msgLives.saveBGpart(); 
    msgLives.draw();
	
    sprintf(tempTextBuffer,"Level %3d  ",level);	
    msgLevel.init("data/fonts/6.ttf", 25, 255, 255, 0, tempTextBuffer, screen);
    msgLevel.setxy(SCREEN_WIDTH-msgLevel.getW(),SCREEN_HEIGHT-msgLevel.getH());
    msgLevel.saveBGpart(); 
    msgLevel.draw();	

    sprintf(tempTextBuffer,"D A N G E R !!!");	
    msgDanger.init("data/fonts/14.ttf", 25, 255, 0, 0, tempTextBuffer, screen);
    msgDanger.setxy(SCREEN_WIDTH/2-msgDanger.getW()/2, SCREEN_HEIGHT-msgDanger.getH());
	
//initialize music && sounds	
    musicMain = Mix_LoadMUS( "data/music/Fly theme.wav" );
    if(musicMain==NULL) exit(1);


    blow1=Mix_LoadWAV("data/music/blow1.wav");
    blow2=Mix_LoadWAV("data/music/blow2.wav");
    sirena=Mix_LoadWAV("data/music/sirena.wav");
    if((blow1==NULL)||(blow2==NULL)||(sirena==NULL)) exit(1);
	 
	Mix_PlayMusic(musicMain,-1);
} 

//-----------------------------------------
SDL_Surface *LoadImage(const char *file){
  SDL_Surface *loadedImage, *optimizedImage;
	
  loadedImage=IMG_Load(file);
  if(loadedImage!=NULL)
  { 
     optimizedImage=SDL_DisplayFormat(loadedImage);
     SDL_FreeSurface(loadedImage);
  }
  return optimizedImage;
}

//-----------------------------------------
void DrawImage(int x, int y, SDL_Surface* source, SDL_Surface* destination){
  SDL_Rect dest;
  
  dest.x=x;
  dest.y=y;
  SDL_BlitSurface(source, NULL, destination, &dest);
}
 
//-----------------------------------------
void DrawScene(){
  int i,k;
  
  if(explode.getFrame()==14){
	 explode.setVisibility(0);
     explode.stopAnim();		
	 explode.rewind();
  }		
    
  myShip.restoreBGpart();
  for (i=0; i<limitN; i++)
     for (k=0; k<limitM; k++) alienShip[i][k].restoreBGpart();
  
  myShip.saveBGpart();
  for (i=0; i<limitN; i++) 
     for (k=0; k<limitM; k++) alienShip[i][k].saveBGpart();
    
  if (myShip.getVisibility()==1) myShip.draw();
  sun.draw();
     
  for (i=0; i<limitN; i++)
     for (k=0; k<limitM; k++) 
		if (alienShip[i][k].getVisibility()==1) alienShip[i][k].draw();  
   	 
  if (explode.getVisibility()==1){
     explode.restoreBGpart();
	 explode.saveBGpart();
     explode.draw();  
  } 
	   
  for(i=0; i<limitMissiles; i++)
     if (missile[i].getVisibility()==1){
	    missile[i].restoreBGpart();
		missile[i].saveBGpart();
	    missile[i].draw();
	 }
  
  SDL_Flip(screen);  
}  

//-----------------------------------------
void MoveAliens(const int& delayMove){
  static unsigned long lastMove=0;
  int i,k;  
    
  if(lastMove+delayMove<SDL_GetTicks()) 
  { 
     if (step>0)
	 {
        int maxVisibleRightInd=0;		  
        for (i=0; i<limitN; i++)
           for (k=0; k<limitM; k++)   
		      if (alienShip[i][k].getVisibility()==1 && k>maxVisibleRightInd)
			     maxVisibleRightInd=k;
			   
		if(alienShip[0][maxVisibleRightInd].getx()+alienShip1base.mW>=SCREEN_WIDTH)
		{
           for (i=0; i<limitN; i++)
              for (k=0; k<limitM; k++) alienShip[i][k].yadd(alienShip1base.mH); 
		   step=-step;
		}
	 } 
	 else if (step<0)
	 {
        int maxVisibleLeftInd=limitM;		  
        for (i=0; i<limitN; i++)
           for (k=0; k<limitM; k++)   
	          if (alienShip[i][k].getVisibility()==1 && k<maxVisibleLeftInd)
		         maxVisibleLeftInd=k;
			   
		if(alienShip[0][maxVisibleLeftInd].getx()<=0)
		{ 
           for (i=0; i<limitN; i++)
              for (k=0; k<limitM; k++) alienShip[i][k].yadd(alienShip1base.mH); 
		   step=-step;
		}
	 }  
	  
	 for (i=0; i<limitN; i++)
        for (k=0; k<limitM; k++) alienShip[i][k].xadd(step);  
	 
	 lastMove=SDL_GetTicks();	 
  } 
}   
  
//-----------------------------------------

void DefeatAlien(int x, int y){
  int i;	
  bool lowerShipIsAlive=false;  
 
  Mix_PlayChannel(-1,blow1,0);
  
  alienShip[x][y].setVisibility(0);
  alienShip[x][y].stopAnim();
  alienShip[x][y].canShoot(0);
  explode.setxy(alienShip[x][y].getx(),alienShip[x][y].gety());
  explode.setVisibility(1); 
  explode.startAnim();	   
       
  alienShip[x][y].canShoot(0);
  for(i=x; i<limitN; i++) if(alienShip[i][y].getVisibility()==1){lowerShipIsAlive=true;break;}
  
  if(!lowerShipIsAlive)
     for(i=x; i>=0; i--) 
        if(alienShip[i][y].getVisibility()==1) 
	    {
		    alienShip[i][y].canShoot(1);
		    break;
	    } 
		 
  switch(alienShip[x][y].getType()){
     case 1: points+=10;break;
     case 2: points+=20;break;
     case 3: points+=40;break;
  } 
    
  sprintf(tempTextBuffer,"Points %7d      ",points);
  msgPoints.setText(tempTextBuffer);
  msgPoints.restoreBGpart();
  msgPoints.draw();
  if(--aliensAliveCounter<1) GamePlay(1);
}     
 
//-----------------------------------------

void StartMyMissile(){
	missile[0].setxy(myShip.getx()+myShipbase.mW/2-missilebase.mW/2, myShip.gety()+missilebase.mH/2);
	missile[0].setVisibility(1);
	missile[0].startAnim();
} 
 
//-----------------------------------------

void MoveMyMissile(){
	int i,k,br=0; 
	
    if(missile[0].getVisibility()==1){
	   missile[0].yadd(-7);
	   for(i=0;i<limitN;i++) 
	   {
	      if (br==1) break;  
	      for(k=0;k<limitM;k++)
		     if(alienShip[i][k].getVisibility()==1)
			    if(missile[0].getx()>=alienShip[i][k].getx() && 
				missile[0].getx()+missilebase.mW<=alienShip[i][k].getx()+alienShip1base.mW && 
				missile[0].gety()+missilebase.mH<=alienShip[i][k].gety()+alienShip1base.mH && 
				missile[0].gety()>=alienShip[i][k].gety())
				{
				   missile[0].setVisibility(0);
				   missile[0].stopAnim();
				   missile[0].rewind();
                   DefeatAlien(i,k); 
				   br=1;
				   break;  
				}
	   } 
	   if(missile[0].gety()<-missilebase.mH)
	   { 
	      missile[0].setVisibility(0); 
		  missile[0].rewind();
		  missile[0].stopAnim();
	   }		
	} 
} 

//-----------------------------------------

void StartAlienMissiles(const int& delayShot){
  int i,k,j,br=0,random,numCanShoot=0,counter;
  static unsigned long lastMove=0;
 
  if(lastMove+delayShot<SDL_GetTicks()) 
  { 	
	for(i=0; i<limitN; i++)
	   for(k=0; k<limitM; k++) 
		   if (alienShip[i][k].getVisibility()==1 && (alienShip[i][k].canShoot()==true)) numCanShoot++;
 
	for(j=1; j<limitMissiles; j++)
	{ 
	   if(missile[j].getVisibility()!=1){
          counter=0;
          random=rand()%numCanShoot;
		  for(i=0; i<limitN; i++){
			 if (br==1) {br=0;break;} 
		     for(k=0; k<limitM; k++) 
			 { 
				if(alienShip[i][k].canShoot()==true && alienShip[i][k].getLastShotTime()+50<SDL_GetTicks() && alienShip[i][k].getx()>myShip.getx()-5*myShipbase.mW && alienShip[i][k].getx()<myShip.getx()+5*myShipbase.mW){
					if(counter==random){
				      missile[j].setxy(alienShip[i][k].getx()+alienShip1base.mW/2-missilebase.mW/2, alienShip[i][k].gety()+missilebase.mH);
				      missile[j].setVisibility(1);
				      missile[j].startAnim(); 
					  if(random=rand()%superMissilePossibility<1){
						 Mix_PlayChannel(-1,sirena,0);
						 missile[j].setType(1);
						 msgDanger.saveBGpart(); 
                         msgDanger.draw();
						 msgDanger.setVisibility(1);
					  }
					  alienShip[i][k].setLastShotTime(SDL_GetTicks());
				      br=1;
				      break; 
				   }
			       counter++;
				}
			 }
		   }
	   }
	}
	lastMove=SDL_GetTicks();
  }
}  
   
//----------------------------------------- 

void MoveAlienMissiles(){ 
  int j;	

  for(j=1;j<limitMissiles;j++)
  {
	  if(myShip.getVisibility()==1)
	  if(missile[j].getVisibility()==1)
      { 
		 if(missile[j].getType()==0) missile[j].yadd(5);
		 else
		 { 
		    missile[j].yadd(5); 
            if(missile[j].getx()<myShip.getx()+myShipbase.mW/2) missile[j].xadd(2);
            else if(missile[j].getx()>myShip.getx()+myShipbase.mW/2) missile[j].xadd(-2);
			
			if(msgDanger.getVisibility()==1){
               msgDanger.setVisibility(0);
			   msgDanger.restoreBGpart();
			}
			else 
			{
				msgDanger.draw();
				msgDanger.setVisibility(1);
			}
		 } 
		 
		 if(missile[j].getx()>=myShip.getx() && 
		 missile[j].getx()+missilebase.mW<=myShip.getx()+myShipbase.mW && 
		 missile[j].gety()+missilebase.mH<=myShip.gety()+myShipbase.mH && 
		 missile[j].gety()>=myShip.gety())
		 {
			if(missile[j].getType()==1) msgDanger.restoreBGpart();
				
			missile[j].setVisibility(0);
			missile[j].setType(0);
		    missile[j].stopAnim();
		    missile[j].rewind();
		    myShip.setVisibility(0);
			myShip.stopAnim();
            explode.setxy(myShip.getx(),myShip.gety());
            explode.setVisibility(1); 
            explode.startAnim();
            GamePlay(0);	
		 } 
 
	     if(missile[j].gety()>SCREEN_HEIGHT+missilebase.mH)
	     {
			if(missile[j].getType()==1) msgDanger.restoreBGpart();

	        missile[j].setVisibility(0); 
			missile[j].setType(0);
		    missile[j].rewind();
		    missile[j].stopAnim();
         }	
	  }		  
  }
}
 
//-----------------------------------------
 
void GamePlay(int flag){
  if(flag==0){
	 Mix_PlayChannel(-1,blow2,0);  
	 DrawImage(0,0,background,screen);  
     SDL_Flip(screen); 
     lives--;
	 if (lives>0) LevelUp(level);
  } 
  if(lives<1){
     sprintf(tempTextBuffer,"GAME OVER. Try again!");	
     msgWinLoose.init("data/fonts/21.ttf", 70, 255, 0, 0, tempTextBuffer, screen);
     msgWinLoose.setxy(SCREEN_WIDTH/2-msgWinLoose.getW()/2,SCREEN_HEIGHT/2-msgWinLoose.getH()/2);
     msgWinLoose.saveBGpart(); 
     DrawImage(0,0,background,screen);  
     msgWinLoose.draw();
     SDL_Flip(screen); 
     SDL_Delay(5000); 
     msgWinLoose.restoreBGpart();
	  
	 lives=3; points=0; LevelUp(1);
	 flag=0;
  }//!!!!!Game Over!!!!! 
	
  if(flag==1 && level==7){ //You Win!!!)!!
     sprintf(tempTextBuffer,"YOU WIN! Your final score is:%5d Try again?", points);	
     msgWinLoose.init("data/fonts/18.ttf", 37, 0, 255, 0, tempTextBuffer, screen);
     msgWinLoose.setxy(SCREEN_WIDTH/2-msgWinLoose.getW()/2,SCREEN_HEIGHT/2-msgWinLoose.getH()/2);
     msgWinLoose.saveBGpart(); 
     DrawImage(0,0,background,screen);  
     msgWinLoose.draw();
     SDL_Flip(screen); 
     SDL_Delay(10000);
     msgWinLoose.restoreBGpart();
	  
	 lives=3; points=0; LevelUp(1);	  
	 flag=0;
  }
  
  if(flag==1){//!!!!!NextLevel!!! 
	 lives++; 
	 level++; 
     LevelUp(level);
  }
}
 
//-----------------------------------------

void ShowSecondaryScreen(int num){
  
  if(num==1){
	//show first page  
	 music1=Mix_LoadMUS( "data/music/Star Wars.wav" );
     if(music1==NULL) exit(1); 
	 Mix_PlayMusic(music1,-1);
	  
	 background=LoadImage("data/prebg.jpg");
     DrawImage(0,0,background,screen); 
	 
     sprintf(tempTextBuffer,"PRESS SPACE TO START");	
     msgPressSpace.init("data/fonts/18.ttf", 30, 255, 255, 255, tempTextBuffer, screen);
     msgPressSpace.setxy(SCREEN_WIDTH/2-msgPressSpace.getW()/2+20, 620);
	 SDL_Flip(screen);	 
	 SDL_Delay(2000);
	 msgPressSpace.draw();	  
	 SDL_Flip(screen); 
  
     int preGameLoop=1; 
     while(preGameLoop==1){
        SDL_Event event;
	   
        while (SDL_PollEvent(&event)){
           if (event.type==SDL_QUIT) preGameLoop=0;

           if (event.key.keysym.sym==SDLK_ESCAPE) exit(0); 
		   
           if (event.type==SDL_KEYDOWN)
              if (event.key.keysym.sym==SDLK_SPACE){
				 preGameLoop=0; 
				 Mix_HaltMusic();
			  }
		}	
	 }  
  }	  
  else if(num==2){
	//show best score's page  
  }
} 
 
void LevelUp(int level){ 
  ::level=level;
  sprintf(tempTextBuffer,"Lives %3d  ",lives);
  msgLives.setText(tempTextBuffer); 
  
  sprintf(tempTextBuffer,"Level %3d  ",level);
  msgLevel.setText(tempTextBuffer);

  sprintf(tempTextBuffer,"Level %d ",level);	
  msgPopUpLevel.init("data/fonts/21.ttf", 150, 0, 255, 0, tempTextBuffer, screen);
  msgPopUpLevel.setxy(SCREEN_WIDTH/2-msgPopUpLevel.getW()/2,SCREEN_HEIGHT/2-msgPopUpLevel.getH()/2);
  msgPopUpLevel.saveBGpart(); 
  DrawImage(0,0,background,screen);  
  msgPopUpLevel.draw();
  SDL_Flip(screen); 
  SDL_Delay(3000); 
  msgPopUpLevel.restoreBGpart();
  
  sprintf(tempTextBuffer,"Points %7d      ",points); 
  msgPoints.setText(tempTextBuffer);
  msgPoints.restoreBGpart();
  msgPoints.draw();
   
  switch(level){
     case 1: 
		limitN=4;
        limitM=4;
        step=10; 
		break;
     case 2: 
		limitN=4;
        limitM=5;
        step=12; 
		break;		 
     case 3: 
		limitN=4;
        limitM=6;
        step=14; 
		break;	
     case 4: ////Hard/// 
		limitN=5;
        limitM=4;
        step=10; 
		limitMissiles=5;			
		break;		
     case 5: 
		limitN=5;
        limitM=5;
        step=12; 
		limitMissiles=6;	
        superMissilePossibility=10;		
		break;	
     case 6: 
		limitN=5;
        limitM=6;
        step=14; 
		limitMissiles=7;	 
		break;		
     case 7: 
		limitN=5;
        limitM=6;
        step=14; 
		limitMissiles=10;			
		break;			
  }    
 
  RestartObjects();
}

//-----------------------------------------

void RestartObjects(){
  int x,y,i,k;	
	  
  myShip.setxy(SCREEN_WIDTH/2-myShipbase.mW,685);
  myShip.setVisibility(1);	
  myShip.saveBGpart();
  
  x=X_COORD; 
  y=Y_COORD;
  for (i=0; i<limitN; i++)
  {
	 for (k=0; k<limitM; k++)   
     {
	    if(i==limitN-1) alienShip[i][k].canShoot(1);
	    else alienShip[i][k].canShoot(0);
	    if(i==0) {alienShip[i][k].init(&alienShip1base,screen);alienShip[i][k].setType(3);}
		else if(i>0 && i<3) {alienShip[i][k].init(&alienShip2base,screen);alienShip[i][k].setType(2);} 
        else {alienShip[i][k].init(&alienShip3base,screen);alienShip[i][k].setType(1);} 			  
        alienShip[i][k].setxy(x,y);
        alienShip[i][k].setSpeed(3.5);  
		alienShip[i][k].setVisibility(1);
		alienShip[i][k].saveBGpart();
		alienShip[i][k].setLastShotTime(SDL_GetTicks());
	    x+=HDISTANCE;
	 } 
	 y+=VDISTANCE;
	 x=X_COORD;
  }  
  
  for(i=0; i<limitMissiles; i++)
  {
     missile[i].init(&missilebase,screen);
     missile[i].setSpeed(0.4);
     missile[i].setVisibility(0); 
	 missile[i].setType(0);
     missile[i].stopAnim();   
  }   
  
  explode.setVisibility(0); 
  explode.stopAnim(); //because of [.init()] does mAnimating=1
 
   
  msgPoints.draw();
  msgLives.draw();
  msgLevel.draw();
  aliensAliveCounter=limitN*limitM;
  SDL_Flip(screen);
}

//-----------------------------------------
  
void CleanUp(){
  SDL_FreeSurface(screen);	
  SDL_FreeSurface(background);
  
  Mix_FreeChunk(blow1); 
  Mix_FreeChunk(blow2); 
  Mix_FreeChunk(sirena); 
  
  Mix_FreeMusic(musicMain); 	
  Mix_FreeMusic(music1); 
  
  Mix_CloseAudio(); 
  TTF_Quit(); 
  SDL_Quit(); 
}

//-----------------------------------------
