#pragma once
#include "rocket.h"

class CPurpleRocket :
	public CRocket
{
public:
	CPurpleRocket(vector<CAbstractParticle*>* pManagersLeftOverParticles,const D3DXVECTOR3& pos,const D3DXVECTOR3& vel,const D3DXVECTOR3& acc,const D3DXVECTOR3& thrust_,float power,float life_time);
	virtual ~CPurpleRocket(void);

	virtual void CPurpleRocket::update();
	virtual vector<CAbstractParticle>* command1(); //called on event 1 i.e. explode into more particles
};
