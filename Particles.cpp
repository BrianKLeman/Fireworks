#include "Particles.h"
#include "./Shared/Timer.h"
#include "FireworksApp.h"
float mousex = 0, mousey = 0;
StructParticle* mParticles = nullptr;
Rocket* mRockets = nullptr;

void UpdateParticlesColours( float step )
{
	StructParticle* p = mParticles;

	// For each particle update its position, velocity and  colour.
    for (unsigned short n = 0; n < MAX_PARTICLES; ++n)
	{
		// Position
		p->p += p->v*step; // Move
		p->p += -0.5f*p->a*step*step;;	// Accelerate

		// Vertical speed.
 		p->v -= p->a*step; 

		// Determine if life has stopped.
		p->mTime += step;
		p->mAlive = p->mTime <= p->mLifeTime;

		// Colour
		// Determine which gradient stops to use.
		float x = (p->mTime)/p->mLifeTime;
		int gradientCount = p->mGradientStopCount;
		signed int index = min(signed int(x*gradientCount), gradientCount);		
		D3DXVECTOR4 c1 = p->mColours[max(signed int(0), index-1)];
	    D3DXVECTOR4 c2 = p->mColours[min(gradientCount-1, index)];
		p->colour = c2*(1.f-x) + c1*x; // linear interpolate between two colours within the gradient.
		p->colour.w = 255.f; // Opaque

		++p; // Next particle.
	}
}

void UpdateParticle( Rocket* rocket )
{
	StructParticle* particle = &(mParticles[rocket->mNextParticleIndex]);
	particle->p = rocket->p;
	particle->v.x = 0.f;// RAN(20);
	particle->v.y = 0.f;
	particle->mLifeTime = 1.0f + RAN(0.5f);
	particle->mTime = 0.f;
	particle->mAlive = true;
	
	if (rocket->mGradientStopCount > 0)
	{
		particle->mColours = rocket->mColours;
		particle->mGradientStopCount = rocket->mGradientStopCount;
	}

	rocket->mNextParticleIndex++;
}

// Recycle the rocket.
void ResetRocket( Rocket* rocket )
{
	rocket->mAlive = true; // Set to alive. Renew...
	rocket->p.x = RAN(1000); // Start at random horizontal position.
	rocket->p.y = 0; // Start at the bottom of the viewport.

	// Determine InitialVelocity of the rocket.
	rocket->v.y = -(mousey - FireworksApp::screenHeight);
	rocket->v.x = (mousex - FireworksApp::screenWidth/2.f);
	rocket->mTime = 0; // Born.
	rocket->colour = D3DXVECTOR4(255.f, 255.f,255.f,255.f); // Starts white.
	rocket->mLifeTime = RAN(5); // Random lifetime < 5 seconds.
}

void ResetFire( Rocket* rocket )
{
	rocket->mAlive = true;
	rocket->p.x = mousex ;
	rocket->p.y = mousey;
	rocket->v.y = 0;
	rocket->v.x = 0;
	rocket->mTime = 0;
	rocket->colour.x = 255.f;//+RAN(128.f);
	rocket->colour.y = RAN(255.f);
	rocket->colour.z = 0.f;
	rocket->colour.w = 255.f;
	rocket->mLifeTime = RAN(10);
}

void UpdateFireParticle( Rocket* rocket )
{
	StructParticle* particle = &(mParticles[rocket->mNextParticleIndex]);
	particle->p.x = rocket->p.x + 20.f - RAN(40) ;
	particle->p.y = rocket->p.y  + 20.f -RAN(60);
	particle->v.x = 50.f - (float)0.3f*100.f/255.f; 
	particle->v.y = ((float)0.3f)*80.f / 255.f;
	particle->mLifeTime = 0.5f + (RAN(2.f));
	particle->mTime = 0.f;
	particle->mAlive = true;	
	rocket->mNextParticleIndex++;	
}

void UpdateRockets( int rocketIndex, float step, bool restart /*= true*/ )
{
	Rocket* rocket = &(mRockets[rocketIndex]);

	if (rocket->mAlive)
	{
		rocket->p += rocket->v*step;
		rocket->p.y -= 0.5f*GRAVITATIONAL_STRENGTH*step*step;
		rocket->v.y -= GRAVITATIONAL_STRENGTH*step;	
		rocket->v += rocket->a * step;
		rocket->mAlive = ((rocket->mTime += step) < rocket->mLifeTime);
		UpdateParticle(rocket);
		rocket->mNextParticleIndex = (rocket->mNextParticleIndex > rocket->mStart + rocket->mCount - 1) ? rocket->mStart : rocket->mNextParticleIndex;
	}
	else if(restart)
	{
		ResetRocket(rocket);
	}
}

void UpdateFire( int rocketIndex, TTimeUnit step, bool restart /*= true*/ )
{
	Rocket* rocket = &(mRockets[rocketIndex]);

	if (rocket->mAlive)
	{
		rocket->p.x = mousex ;
		rocket->p.y = mousey;
		rocket->mAlive = ((rocket->mTime += step) < rocket->mLifeTime);
		UpdateFireParticle(rocket);
		rocket->mNextParticleIndex = (rocket->mNextParticleIndex > rocket->mStart + rocket->mCount) ? rocket->mStart : rocket->mNextParticleIndex;
	}
	else if(restart)
	{
		ResetFire(rocket);
	}
}

void UpdateParticles( float dt )
{
	for(int i = 0; i < FIRE_WORKS_LIMIT; ++i)
		UpdateFire(i,dt,true);

	for(int i = FIRE_WORKS_LIMIT; i < MAX_ROCKETS; ++i)
	{
		UpdateRockets(i,dt);
		Rocket* p = &(mRockets[i]);

		if (!p->mAlive || p->v.y < 50.f && p->v.y > 0.f)
		{ 
			p->mAlive = false;
			// explode
			int count = EXPLOSION_TRAILS-10;
			int trails = (int)floor(RAN(count));
			trails += 10;

			// Determine gradient stops.
			int colourStopCount(int(ceilf(RAN(10))));
			vector<D3DXVECTOR4> colourGradientStops(colourStopCount);
			for(int j(0); j < colourStopCount; ++j)
			{
				colourGradientStops[j].x = RAN(255);
				colourGradientStops[j].y = RAN(255);
				colourGradientStops[j].z = RAN(255);
				colourGradientStops[j].w = 255.f;
			}

			for(int j = 0; j < trails; ++j)
			{				
				Rocket* rocket = &(mRockets[CALC_ROCKET_TRAIL_INDEX(i, j)]);
				rocket->mCount = particlesPerRocket;
				rocket->mNextParticleIndex = rocket->mStart = CALC_TRAIL_INDEX(i,j);
				rocket->mAlive = true;
				rocket->p = p->p;
				float theta = FLOAT(j)*6.3f/FLOAT(trails);
				rocket->v.x = TUnit(100.f*cosf(theta));
				rocket->v.y =  TUnit(100.f*sinf(theta));
				rocket->a.x = rocket->v.x*5.f;
				rocket->a.y = rocket->v.y*5.f;
				rocket->v += rocket->a * dt;
				rocket->mTime = 0;			
				rocket->mColours.clear();
				for(size_t z = 0; z < colourGradientStops.size(); ++z)
					rocket->mColours.push_back( colourGradientStops[z]);
				rocket->mGradientStopCount = static_cast<short>(rocket->mColours.size());
				rocket->colour.w = 255.f;
				rocket->mLifeTime = RAN(1);
			}
		}
	}

	for(int i = MAX_ROCKETS; i < MAX_ROCKETS*EXPLOSION_TRAILS; ++i)
	{
		UpdateRockets(i, dt, false);
	}

	UpdateParticlesColours(dt);
}

void SetupRockets()
{
	for(int i = 0; i < (MAX_ROCKETS+1)* EXPLOSION_TRAILS; ++i)
		mRockets[i] = Rocket(i*(particlesPerRocket), particlesPerRocket);
}

void Init()
{
	mRockets = new Rocket[(MAX_ROCKETS+1)* EXPLOSION_TRAILS];
}

//-----------------------------------------------------------------------------
// Release (delete) all the resources used by this program.
void CleanUp()
{
	delete[] mParticles;
}