#pragma once
#include "rocket.h"

class CGreenRocket :
	public CRocket
{
public:
	CGreenRocket(vector<CAbstractParticle*>* pManagersLeftOverParticles,const D3DXVECTOR3& pos,const D3DXVECTOR3& vel,const D3DXVECTOR3& acc,const D3DXVECTOR3& thrust_,float power,float life_time);
	virtual ~CGreenRocket(void);

	virtual void CGreenRocket::update();
	virtual vector<CAbstractParticle>* command1(); //called on event 1 i.e. explode into more particles
};
