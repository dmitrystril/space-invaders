//ctextmsg.h

#ifndef CTEXTMSG_H
#define CTEXTMSG_H

#include "SDL/SDL_ttf.h"
 
//---------------------------------------------

class CTextMsg 
{
  public: 
	 void init(const char* fontPath, int fontSize, int r, int g, int b, char* msg, SDL_Surface *screen);
	 void draw();
	 void saveBGpart();
     void restoreBGpart()const;
	 void setText(char* text);
	 void setxy(int x, int y); 
	 void setFont(char* path, int size);
	 void setTextColor(int r, int g, int b);
	 
	 int getW(){
		 return mW;
	 }
	 
	 int getH(){
		 return mH;
	 }
	 
	 int getX(){
		 return mX;
	 }
	 
	 int getY(){
		 return mY;
	 }
	 
	 int getVisibility()const{
	   return mIsVisible; 
	 }
	
	 void setVisibility(int value){ 
	   mIsVisible=value;
	 }
	
  private:
	 int mX, mY, mW, mH;
     SDL_Surface *mScreen, *mSfc_text, *mBackReplacement;
	 TTF_Font *mSfc_font;
	 SDL_Color mTextColor; 
	 int mIsVisible;
};


#endif
