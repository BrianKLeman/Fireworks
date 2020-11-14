#include "RedRocket.h"

CRedRocket::CRedRocket(vector<CAbstractParticle*>* pManagersLeftOverParticles,const D3DXVECTOR3& pos,const D3DXVECTOR3& vel,const D3DXVECTOR3& acc,const D3DXVECTOR3& thrust_,float energy_,float life_time)
:CRocket(pManagersLeftOverParticles,pos,vel,acc,thrust_,energy_,life_time)
{

}

CRedRocket::~CRedRocket(void)
{
}

void CRedRocket::update()
{

	
	
	if((timer->getTimeElapsedFromStart() - time_born) > (life_length+10.f)) { alive = false; }
	float dt = getDeltaTime();

	//if passed a set time interval create a particle
	if(((timer->getTimeElapsedFromStart() - time_born) > (life_length))&&exploded==false)
	{
		last_emission = timer->getTimeElapsedFromStart();
		this->command1();
		exploded = true;
	}

	if(alive && !exploded)
	{
		if(nbParticlesCreated < MAX_PARTICLES_PER_ROCKET && (timer->getTimeElapsedFromStart() - last_emission) > WHEN_TO_CREATE_NEW_PARTICLE_ROCKET && ((timer->getTimeElapsedFromStart()-time_born)<(life_length*0.7) ))
		{
			last_emission = timer->getTimeElapsedFromStart();
			float life_(PARTICLE_LIFE_TIME);
			nbParticlesCreated++;
			//these created with the new are deleted by the sparkle effect destructor
			float* colour_ = new float[3];
			colour_[0] = 1.0f;
			colour_[1] = 0.0f;
			colour_[2] = 0.0f;

			float* colour_sparkle = new float[3];
			colour_sparkle[0] = 1.0f;
			colour_sparkle[1] = 1.0f;
			colour_sparkle[2] = 1.0f;

			emittedParticles.push_back(new CRocketParticle(position,D3DXVECTOR3(0.0f,0.0f,0.0f),life_,colour_,colour_sparkle));
		}
	}
	//we need to create some thrust (acceleration)
	//this deteriates with time
	//but... stops completely after set amount of time... when it runs out of energy
	if((timer->getTimeElapsedFromStart()-time_born)<(life_length*0.6) ) //quarter of the time to explosion
	{
		D3DXVECTOR3  thrust_current = thrust * energy * (life_length - (timer->getTimeElapsedFromStart() - time_born));
		//s = ut + 0.5at*t
		position += velocity*dt + 0.25f*(thrust_current*dt)*dt*dt+0.5f*dt*dt*D3DXVECTOR3(0.0f,-9.81f,0.0f);

		//v = u + at
		velocity += (acceleration+thrust_current*dt)*dt;
	}
	else //only deal with velocity and gravity here, maybe wind later
	{
		position += velocity*dt + 0.5f*dt*dt*D3DXVECTOR3(0.0f,-20.0f,0.0f);//it will decelerate becuase with air and wind resistance
		//v = u + at
		velocity += D3DXVECTOR3(0.0f,-50.0f,0.0f)*dt;
	}
	
	//we need to deviate the path in a set region
	//by making it wander
	// I got this idea from an article on wandering behavior in a book called programming AI by Example
	//From what i remember, imagine a fixed circle around the current position of the object
	//Now my bit is to calculate a fixed range of angles around the heading.
	//I'm going to extrapolate the unit velocity vector by the radius of the sphere
	//Give a range of say 1 radian around the acceleration vector.
	//generate a random number between one and two ( say choose left or right)
	//then when each 100th of a second is up the velocity and acceleration is rotated 1 degree
	//In fact for a short cut, and because i am happy with the results of the settings for my rocket
	//I am going to just rotate all the vectors left or right randomly for a 100th of a second. 
	//update living particles of this class using the function in the base class
	
	//THE ABOVE NOTE IS NOT IMPLEMENTED , i m using a random number generator
	//only change direction every split second
	if(timer->getTimeElapsedFromStart() - lastAdjustmentOfDirection > 0.2f)
	{
		lastAdjustmentOfDirection = timer->getTimeElapsedFromStart();

		D3DXVECTOR4 tVel;//(velocity.x,velocity.y,velocity.z, 0.0f);
		switch (direction)
		{
		case LEFT:	D3DXVec3Transform(&tVel,&velocity,&rotationL); 
						velocity.x = tVel.x;
						velocity.y = tVel.y;
						velocity.z = tVel.z;
						break;
		case RIGHT:D3DXVec3Transform(&tVel,&velocity,&rotationR); 
						velocity.x = tVel.x;
						velocity.y = tVel.y;
						velocity.z = tVel.z;
						break;
		}
		direction = static_cast<DIRECTION>(rand()%2); //again LEFT or RIGHT... 0 or 1 respectively.
	}

	CAbstractParticleEmitter::update(); 

	
	//debug info 
	if(debug)
	{
		std::vector<D3DXVECTOR3> debug_details; 
		debug_details.push_back(position);
		csv->writeLine(debug_details);
	}
	

}

vector<CAbstractParticle>* CRedRocket::command1() //called on event 1 i.e. explode into more particles
{
	return CRocket::command1();
	//to explode create 10 particles, starting froma random angle turn that angle into a vector
	//rotate that vector 9 more times incrementing by 360/nbParticles = 36 degrees
	int			nbParticles(20);

	D3DXVECTOR4 initialDirection(0.0f,1.0f,0.0f,1.0f);
	float		speed(12.0f);
	float		randAngleRad((float)(rand()%628)/100.0f); //roughly a circle in radians
	D3DXMATRIX	initial_rotation, x_initial_rotation;

				D3DXMatrixRotationZ(&initial_rotation,randAngleRad);
				D3DXMatrixRotationX(&x_initial_rotation,randAngleRad);

	D3DXMATRIX	incrementalRotation;
	D3DXMATRIX  xIncremntalRotation;
	D3DXVECTOR4 incrementedDirection
		(0.0f,1.0f,0.0f,1.0f);

				D3DXMatrixRotationZ(&incrementalRotation,6.28318f/(static_cast<float>(nbParticles == 0 ? nbParticles :10)));
				D3DXMatrixRotationX(&xIncremntalRotation,6.28318f/(static_cast<float>(nbParticles/2 == 0 ? nbParticles/2 :5)));
				D3DXVec4Transform(&initialDirection,&initialDirection,&initial_rotation);
				D3DXVec4Transform(&initialDirection,&initialDirection,&x_initial_rotation);
	D3DXVECTOR3 randDirection(initialDirection.x,initialDirection.y,initialDirection.z);
	D3DXVECTOR3 cumulatedRandDirection(randDirection);
	
				

	float* colour1_ = new float[3];
	float* colour2_ = new float[3];
	float* colour_sparkle = new float[3];
	colour1_[0] = 1.0f;
	colour1_[1] = 0.0f;
	colour1_[2] = 0.0f;
	
	colour2_[0] = 1.0f;
	colour2_[1] = 1.0f;
	colour2_[2] = 1.0f;
	
	colour_sparkle[0] = 1.5f;
	colour_sparkle[1] = 1.5f;
	colour_sparkle[2] = 1.5f;
	
	bool colour1or2(false);
	float life_(-(((float)(rand()%3)+1))+3.0f);
	emittedParticles.push_back(new CRocketParticle(position,cumulatedRandDirection*speed,life_,colour1_,colour_sparkle));
	
	incrementedDirection =  initialDirection;

	for(int i = 0; i < nbParticles-1; ++i)
	{
		D3DXVec4Transform(&incrementedDirection,&incrementedDirection,&incrementalRotation);
		
		for(int j = 0; j < (nbParticles/2); ++j)
		{
			D3DXVec4Transform(&incrementedDirection,&incrementedDirection,&xIncremntalRotation);
			cumulatedRandDirection.x = incrementedDirection.x;
			cumulatedRandDirection.y = incrementedDirection.y;
			cumulatedRandDirection.z = incrementedDirection.z;

			life_ = (-(((float)(rand()%3)+1))+3.0f);
			colour1or2 = !colour1or2;
			if(colour1or2)
				emittedParticles.push_back(new CRocketParticle(position,cumulatedRandDirection*speed,life_,colour1_,colour_sparkle));
			else
				emittedParticles.push_back(new CRocketParticle(position,cumulatedRandDirection*speed,life_,colour2_,colour_sparkle));
		}
	}

	delete[] colour_sparkle;
	delete[] colour1_;
	delete[] colour2_;
	return 0;
}