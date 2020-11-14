#pragma once
#include "RomanParticle.h"
#include "Rocket.h"
class CRomanCandle :
public CRocket
{
public:
	CRomanCandle(vector<CAbstractParticle*>* pManagersLeftOverParticles,const D3DXVECTOR3& pos,D3DXVECTOR3 vel = D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3 acc = D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3 thrust_ = D3DXVECTOR3(0.0f,0.0f,0.0f),float power = 0.0f,float life_time = 30.0f):
	  CRocket(pManagersLeftOverParticles,pos, vel, acc, thrust_, power, life_time)
	  {	  }
	~CRomanCandle(void);
	virtual vector<CAbstractParticle>* command1() {return 0;} //called on event 1 i.e. explode into more particles
	virtual bool 					   command2() {return true;} //called on event 2 i.e. destroy

	virtual void update();
private:
	CRomanCandle(void);
};

