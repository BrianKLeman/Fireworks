#pragma once
#include "AbstractParticle.h"
#include "my_utilities.h"
class CRocketParticle :
public CAbstractParticle
{
public:
CRocketParticle(const D3DXVECTOR3 dest,const D3DXVECTOR3 i_vel);
CRocketParticle::CRocketParticle(const D3DXVECTOR3 dest,const D3DXVECTOR3 i_vel, float life_):
CAbstractParticle(life_)
{	
	addGAcc();													//add gravity to acceleration
	alive 		= true;											//alive
	colour 		= D3DXVECTOR4(1.0f,1.0f,1.0f,1.f);							//colour white
	vel 		= i_vel*0.8f;									//8 tenths of the emitter velocity			
	pos 		= dest;											//start position
	last_update = timer->getTimeElapsedFromStart();				//initialise last_update
}
CRocketParticle::CRocketParticle(const D3DXVECTOR3 dest,const D3DXVECTOR3 i_vel, float life_, float colour_[], float sparkle_[]):
CAbstractParticle(life_,colour_,sparkle_)
{	
	addGAcc();													//add gravity to acceleration
	alive 		= true;											//alive
	colour 		= D3DXVECTOR4(1.0f,1.0f,1.0f,1.f);							//colour white
	vel 		= i_vel*0.8f;									//8 tenths of the emitter velocity			
	pos 		= dest;											//start position
	last_update = timer->getTimeElapsedFromStart();				//initialise last_update
}
virtual ~CRocketParticle(void);

virtual void update();

virtual vector<CAbstractParticle>* command1(); //called on event 1 i.e. explode into more particles
virtual bool 					   command2(); //called on event 2 i.e. destroy
private:
	// this is to set the colour of each particle
	sparkle colour_;
};

