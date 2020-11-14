#include "RomanCandle.h"




CRomanCandle::~CRomanCandle(void)
{
}

void CRomanCandle::update() //called on event 1 i.e. explode into more particles
{
	
	if((timer->getTimeElapsedFromStart() - time_born) > (life_length+1.5f)) { mAlive = false; }
	float dt = GetDeltaTime();

	//if passed a set time interval create a particle
	
	
	//to explode create 10 particles, starting froma random angle turn that angle into a vector
	//rotate that vector 9 more times incrementing by 360/nbParticles = 36 degrees
	int			nbParticles(15);

	D3DXVECTOR4 initialDirection(0.1f,1.0f,0.0f,1.0f);
	float		speed(10.0f);
	float		randAngleRad((float)(rand()%7)); //roughly a circle in radians
	D3DXMATRIX	initial_rotation;

				D3DXMatrixRotationZ(&initial_rotation,randAngleRad);

	D3DXMATRIX	incrementalRotation;
	D3DXVECTOR4 incrementedDirection(0.1f,1.0f,0.0f,1.0f);

				D3DXMatrixRotationY(&incrementalRotation,6.28318f/static_cast<float>(nbParticles == 0 ? 15 : nbParticles));
				D3DXVec4Transform(&initialDirection,&initialDirection,&initial_rotation);

	D3DXVECTOR3 randDirection(initialDirection.x,initialDirection.y,initialDirection.z);
	D3DXVECTOR3 cumulatedRandDirection(randDirection);
	
				mEmittedParticles.push_back(new CRocketParticle(mPosition,randDirection*speed));
	
				incrementedDirection =  initialDirection;
	for(int i = 0; i < nbParticles-1; ++i)
	{
		D3DXVec4Transform(&incrementedDirection,&incrementedDirection,&initial_rotation);
		cumulatedRandDirection.x = incrementedDirection.x;
		cumulatedRandDirection.y = incrementedDirection.y;
		cumulatedRandDirection.z = incrementedDirection.z;
		mEmittedParticles.push_back(new CRomanParticle(mPosition,cumulatedRandDirection*speed,6.0f));
		
	}

	//update living particles of this class using the function in the base class
	CAbstractParticleEmitter::update();
	
}