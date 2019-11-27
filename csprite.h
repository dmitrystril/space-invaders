//csprite.h 

#ifndef CSPRITE_H
#define CSPRITE_H

#include "SDL/SDL.h" 
#include "cspritebase.h"
 
class CSprite 
{
  public: 
     int init(CSpriteBase *base, SDL_Surface *screen);
     void draw();
	 void saveBGpart();
     void restoreBGpart()const;
    
     void setFrame(int value){ 
	   mFrame=value; 
	 }
	
     int getFrame()const{ 
	   return mFrame; 
	 }
  
     void setSpeed(float value){
	   mSpeed=value; 
	 }
	 
     float getSpeed()const{ 
	   return mSpeed; 
	 }
  
     void startAnim(){ 
	   mAnimating=1; 
	 }
	
     void stopAnim(){
	   mAnimating=0; 
	 }
	
     void rewind(){
	   mFrame=0; 
	 }
  
     void xadd(int value){ 
	   mX+=value; 
	 }
	
     void yadd(int value){ 
	   mY+=value; 
	 }  
	
     void xset(int value){ 
	   mX=value; 
	 }
	
     void yset(int value){ 
	   mY=value; 
	 }
	
	 int getx()const{ 
	   return mX;  
	 }
	
	 int gety()const{ 
	   return mY; 
	 }
	
     void setxy(int x, int y){ 
	   mX=x; 
	   mY=y; 
	 }
	
	 int getVisibility()const{
	   return mIsVisible; 
	 }
	
	 void setVisibility(int value){ 
	   mIsVisible=value;
	 }
	  
  private:
     CSpriteBase *mSpriteBase;
     SDL_Surface *mBackReplacement;
     SDL_Surface *mScreen;  
     int mFrame;
     int mX, mY, mOldX, mOldY;  
     int mAnimating;
     int mDrawn;
     float mSpeed;
     long mLastUpdate;
	 int mIsVisible;
};

#endif
