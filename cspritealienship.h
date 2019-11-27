//cspritealienship.h

#ifndef CSPRITEALIENSHIP_H 
#define CSPRITEALIENSHIP_H


class CSpriteAlienShip : public CSprite
{
  public:  
     bool canShoot()const{
		return mCanShoot;
	 }
	 
	 void canShoot(bool c){
	    mCanShoot=c;
	 }
	 
	 void setType(int type){
	    mType=type;
	 }

     int getLastShotTime()const{
        return mLastShotTime;
	 }

     void setLastShotTime(unsigned long time){
	    mLastShotTime=time;
	 }

     int getType()const{
        return mType;
	 }	 
	
  private:
     bool mCanShoot;  
	 int mType;
	 unsigned long mLastShotTime;
};

#endif
