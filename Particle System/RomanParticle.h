#pragma once
#include "RocketParticle.h"
class CRomanParticle :
public CRocketParticle
{
private:
	CRomanParticle(void);
	
public:
	CRomanParticle(D3DXVECTOR3 position,D3DXVECTOR3 velocity, float life_time)
		: CRocketParticle(position,velocity)
	{
		yPlaneHeight = 5.0f;
		CAbstractParticle::life_length = life_time;
	}

virtual ~CRomanParticle(void);
virtual void update() { CRocketParticle::update(); if(pos.y < yPlaneHeight) vel.y = abs(vel.y-(getDeltaTime()*0.3f*vel.y)); return;}
protected:
	float yPlaneHeight;
};

