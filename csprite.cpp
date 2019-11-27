//csprite.cpp

#include "csprite.h"   

//--------------------------------------------- 
 
int CSprite::init(CSpriteBase *base, SDL_Surface *screen){ 
  mSpriteBase=base;
  if(mSpriteBase->mBuilt)
  {
     if(mSpriteBase->mNumFrames>1) mAnimating=1;
     mBackReplacement=SDL_DisplayFormat(mSpriteBase->mAnim[0].image);
  }
  mScreen=screen; 
  return 0;
    
}
//---------------------------------------------

void CSprite::saveBGpart()
{
  SDL_Rect srcrect;
  
  srcrect.w=mSpriteBase->mW;
  srcrect.h=mSpriteBase->mH;
  srcrect.x=mX;
  srcrect.y=mY;
  mOldX=mX; 
  mOldY=mY;
  SDL_BlitSurface(mScreen, &srcrect, mBackReplacement, NULL);
}

//---------------------------------------------

void CSprite::restoreBGpart()const
{ 
  if(mDrawn==1)
  {
     SDL_Rect dest; 
	 
     dest.x = mOldX;
     dest.y = mOldY;
     dest.w = mSpriteBase->mW;
     dest.h = mSpriteBase->mH;
     SDL_BlitSurface(mBackReplacement, NULL, mScreen, &dest);
  }
}
 
//---------------------------------------------
void CSprite::draw() 
{
  SDL_Rect dest; 
	
  if(mAnimating==1)
  { 
     if(mLastUpdate+mSpriteBase->mAnim[mFrame].pause*mSpeed<SDL_GetTicks())
     {
        mFrame++;
        if(mFrame>mSpriteBase->mNumFrames-1) mFrame=0;
        mLastUpdate=SDL_GetTicks();
     }
  } 

  if(mDrawn==0) {
     mDrawn=1;
  }

  dest.x = mX; 
  dest.y = mY;
  SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, NULL, mScreen, &dest);
}

//---------------------------------------------
