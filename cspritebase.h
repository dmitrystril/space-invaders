//cspritebase.h

#ifndef CSPRITEBASE_H
#define CSPRITEBASE_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

struct SpriteFrame  
{ 
  SDL_Surface *image; 
  int pause; 
}; 

class CSpriteBase
{
  public:
     SpriteFrame *mAnim;
     int mBuilt, mNumFrames, mW, mH;	  
	  
     int init(const char* dir);
};


#endif
