#pragma once

#include "my_utilities.h"

class CRocketParticleManager :
public CAbstractParticleManager
{
public:
CRocketParticleManager(void);
virtual ~CRocketParticleManager(void);


int nbUpdates;
int getNbUpdates() { return nbUpdates; }

virtual bool command1(D3DXVECTOR3 dest) 
{ 
	float life_((((float)(rand()%100)+1.0f)/1000.0f)+1.5f);
	
	mEmittedParticles.push_back(new CRocket(&mParticles,dest,D3DXVECTOR3(0.0f,10.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,10.0f,0.0f),160.0f,life_)); 
	return true;
} //add particle emitter left
virtual bool command2() { mEmittedParticles.push_back(new CRocket(&mParticles,D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,50.0f,0.0f),D3DXVECTOR3( 0.5f,1.0f,0.0f),1.0f,10.0f)); return true; } //add particle emitter right

virtual bool command3(unsigned int emitter)	 //not used yet	
{ 
	
	return true; 
}

virtual bool command4(D3DXVECTOR3 dest)		
{ 
	float life_((((float)(rand()%100)+1.0f)/1000.0f)+0.5f);
	mEmittedParticles.push_back(new CSideWinderRocket(&mParticles,dest,D3DXVECTOR3(0.0f,10.0f,0.0f),D3DXVECTOR3(0.0f,2.5f,0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),30.0f,life_)); 
	return true; 
}

virtual bool command5(D3DXVECTOR3 dest)		
{ 
	float life_((((float)(rand()%3)+1.0f)/4.0f)+7.25f);
	mEmittedParticles.push_back(new CCatherineWheel(&mParticles,dest,D3DXVECTOR3(1.25f,10.0f,0.0f),D3DXVECTOR3(0.5f,2.5f,0.0f),D3DXVECTOR3(0.2f,0.6f,0.0f),10.0f,life_)); 
	return true; 
}


virtual void update() 
{ /* updateParticles();*/ 
	CAbstractParticleManager::callUpdateOnAllParticlesAndEmitters();
}

//paramterised commands
virtual bool command1(const D3DXVECTOR3& pos,const D3DXVECTOR3& vel,const D3DXVECTOR3& acc,const D3DXVECTOR3& thrust_,float energy_,float life_time) 
{ 
	float life_((((float)(rand()%1000)+1.0f)/50.0f)+1.5f);
	
	mEmittedParticles.push_back(new CRocket(&mParticles,pos,vel,acc,thrust_,energy_,life_)); 
	return true;
} //add particle emitter left

virtual bool coloured_rocket(D3DXVECTOR3 dest,ROCKET_COLOURS COLOUR) 
{ 
	float life_ = 1.f;
	
	const D3DXVECTOR3 RED_COLOUR(1.f,0.f,0.f);
	const D3DXVECTOR3 BLUE_COLOUR(0.f,0.f,0.f);
	const D3DXVECTOR3 GREEN_COLOUR(0.F,0.F,1.F);
	const D3DXVECTOR3 YELLOW_COLOUR(1.F,1.0F,0.F);
	switch(COLOUR)
	{
		case RED: mEmittedParticles.push_back(new CRocket(&mParticles,dest,D3DXVECTOR3(0.0f,25.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,10.0f,0.0f),160.0f,life_, RED_COLOUR)); break;
		case BLUE: mEmittedParticles.push_back(new CRocket(&mParticles,dest,D3DXVECTOR3(0.0f,25.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,10.0f,0.0f),160.0f,life_, BLUE_COLOUR)); break;
		case YELLOW: mEmittedParticles.push_back(new CRocket(&mParticles,dest,D3DXVECTOR3(0.0f,25.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,10.0f,0.0f),160.0f,life_, YELLOW_COLOUR)); break;
		case GREEN: mEmittedParticles.push_back(new CRocket(&mParticles,dest,D3DXVECTOR3(0.0f,25.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,10.0f,0.0f),160.0f,life_, GREEN_COLOUR)); break;
	}
	
	return true;
} //add particle emitter left
};

