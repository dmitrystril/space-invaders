//cspritebase.cpp

#include <stdio.h>
#include <stdlib.h>

#include "cspritebase.h"
#include <string.h>

//-------------------------------------------------
int CSpriteBase::init(const char* dir){

  char buffer[255];
  char filename[255];
  char name[255];
  int r=0, g=0, b=0, pause=0;
  FILE *fp;
  
  sprintf(filename,"%s/info",dir);
  if((fp=fopen(filename,"r"))==NULL){
	 fprintf(stderr,"Error opening the file %s\n\n",filename); 
     exit(1);
  }   
  
  fgets(buffer,255,fp);
  sscanf(buffer,"FILES: %d",&mNumFrames);
  mAnim = new SpriteFrame[mNumFrames];
  
  fgets(buffer,255,fp);
  sscanf(buffer,"ALPHA: %d %d %d",&r,&g,&b);
  
  mBuilt=1;
  int count=0; 
  
  while(!feof(fp) && count<mNumFrames){
     fgets(buffer,255,fp);
     if(buffer[0]!='#' && buffer[0]!='\r' && buffer[0]!='\n' && buffer[0]!='\0' && strlen(buffer)!=0)
     {
        sscanf(buffer,"%s %d",name,&pause);
        sprintf(filename,"%s/%s",dir,name);

        SDL_Surface *temp;
        if((temp=IMG_Load(filename))==NULL){
		   fprintf(stderr,"IMG_Load: %s\n", IMG_GetError()); 	
           exit(1);
		}
      
        SDL_SetColorKey(temp,SDL_SRCCOLORKEY,SDL_MapRGB(temp->format,r,g,b));
        mAnim[count].image = SDL_DisplayFormat(temp);
        SDL_FreeSurface(temp);
        mAnim[count].pause = pause;
        mW = mAnim[count].image->w;
        mH = mAnim[count].image->h;
        
        count++; 
     } 
  }
   
  fclose(fp);
  return 0;
}

//-------------------------------------------------


