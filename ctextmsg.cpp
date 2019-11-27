//ctextmsg.cpp

#include "ctextmsg.h"

//---------------------------------------------

void CTextMsg::init(const char* fontPath, int fontSize, int r, int g, int b, char* msg, SDL_Surface *screen)
	 { 
		 mScreen=screen;
		 mSfc_font=TTF_OpenFont(fontPath, fontSize);
		 mTextColor.r=r;
		 mTextColor.g=g;
		 mTextColor.b=b; 
		 mSfc_text=TTF_RenderText_Solid(mSfc_font, msg, mTextColor);
		 mBackReplacement=SDL_DisplayFormat(mSfc_text);
		 mW=mSfc_text->w;
         mH=mSfc_text->h;
	 }
	 
	 void CTextMsg::draw(){
		 SDL_Rect dest; 
		 
	     dest.x = mX; 
         dest.y = mY;
         SDL_BlitSurface(mSfc_text, NULL, mScreen, &dest);	 
	 }   
	 
	 void CTextMsg::saveBGpart()
	 {
		 SDL_Rect srcrect;
  
         srcrect.w=mSfc_text->w;
		 srcrect.h=mSfc_text->h;
	     srcrect.x=mX;
	     srcrect.y=mY;
 
	     SDL_BlitSurface(mScreen, &srcrect, mBackReplacement, NULL);
	 }
	 
     void CTextMsg::restoreBGpart()const
	 {
         SDL_Rect destrect; 
	 
         destrect.x = mX;
		 destrect.y = mY;
		 destrect.w = mSfc_text->w;
		 destrect.h = mSfc_text->h;
		 
		 SDL_BlitSurface(mBackReplacement, NULL, mScreen, &destrect);
	 }
	 
	 void CTextMsg::setText(char* text)
	 {
		 mSfc_text=TTF_RenderText_Solid(mSfc_font, text, mTextColor);
	 }
	 
	 void CTextMsg::setTextColor(int r, int g, int b)
	 {
		 mTextColor.r=r;
		 mTextColor.g=g;
		 mTextColor.b=b;		 
	 }  
	 
	 void CTextMsg::setxy(int x, int y)
	 {
		 mX=x;
		 mY=y;
	 }
	 
	 void CTextMsg::setFont(char* path, int size)
	 {
		 mSfc_font=TTF_OpenFont(path, size);
	 }

  
