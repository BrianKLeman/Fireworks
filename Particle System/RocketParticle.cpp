#include "RocketParticle.h"


CRocketParticle::CRocketParticle(const D3DXVECTOR3 dest,const D3DXVECTOR3 i_vel):
CAbstractParticle(0.5f)
{	
	addGAcc();													//add gravity to acceleration
	alive 		= true;											//alive
	colour 		= D3DXVECTOR4(1.f,1.f,1.f,1.f);							//colour white
	vel 		= i_vel*0.5f;									//8 tenths of the emitter velocity			
	pos 		= dest;											//start position
	last_update = timer->getTimeElapsedFromStart();				//initialise last_update
}



CRocketParticle::~CRocketParticle(void)
{

}

void CRocketParticle::update()
{

	if((timer->getTimeElapsedFromStart() - time_born) > life_length) { alive = false; colour = D3DXVECTOR4(0.0f,0.0f,0.0f,0.f); }

	float dt = getDeltaTime();

	//s = ut + 0.5at*t
	pos += vel*(dt) + acc*(0.5f*dt*dt);

	//v = u + at
	vel = vel + acc*dt;

	//decay colour
	colour.w = ((life_length - (timer->getTimeElapsedFromStart() - time_born))/life_length);

}

vector<CAbstractParticle>* CRocketParticle::command1() //called on event 1 i.e. explode into more particles
{

	return 0;
}

bool CRocketParticle::command2() //called on event 2 i.e. destroy
{
	return false;
}