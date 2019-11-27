//cspritemissile.h

#ifndef CSPRITEMISSILE_H 
#define CSPRITEMISSILE_H 


class CSpriteMissile : public CSprite
{
  public:  
	 
	 void setType(int type){
	    mType=type;
	 }

     int getType()const{
        return mType; 
	 }	 
	
  private:
	 int mType; //0=simple missile, 1=auto-pilot missile 

};

#endif
