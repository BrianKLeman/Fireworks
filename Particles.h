#pragma once

#include <d3dx9.h>
#include <iostream>
#include <vector>
using namespace std;

#define RAN(x) ((FLOAT)rand()*x/(FLOAT)RAND_MAX)
#define GRAVITATIONAL_STRENGTH 200.f
#define PARTICLE_GRAVITY_FACTOR GRAVITATIONAL_STRENGTH*0.5f
#define PARTICLE_LIFETIME 0.002
#define MAX_PARTICLES 40000

typedef FLOAT TUnit;
typedef unsigned short TColor;
typedef unsigned short TIndex;
typedef FLOAT TTimeUnit;

extern float mousex;
extern float mousey;
extern float screenWidth;
extern float screenHeight;


struct StructParticle
{
	D3DXVECTOR4 colour;
	D3DXVECTOR3 p, v;
	TUnit ay;
	TTimeUnit mLifeTime;
	TTimeUnit mTime;
	bool mAlive;
	vector<D3DXVECTOR4> mColours;
	short mGradientStopCount;
	StructParticle();

	StructParticle(StructParticle& sp);
};


extern StructParticle* mParticles;

struct Rocket
	: public StructParticle
{
	TIndex mStart, mCount, mNextParticleIndex;
	Rocket(TIndex start, TIndex count);

	Rocket();
	Rocket(Rocket& cpy);
};

#define MAX_ROCKETS 10
#define EXPLOSION_TRAILS 10
#define FIRE_WORKS_LIMIT 5
const short particlesPerRocket = MAX_PARTICLES / (EXPLOSION_TRAILS*MAX_ROCKETS);

// EXAMPLE: ROCKET 2, TRAIL 1: (MAX_PARTICLES / EXPLOSION_TRAILS) + particlesPerRocket * ROCKET_INDEX * EXPLOSION_TRAILS + particlesPerRocket*TRAIL_INDEX
#define CALC_ROCKET_TRAIL_INDEX(ROCKET_INDEX, TRAIL_INDEX) (MAX_ROCKETS+EXPLOSION_TRAILS*ROCKET_INDEX+TRAIL_INDEX)
#define CALC_TRAIL_INDEX(ROCKET_INDEX, TRAIL_INDEX) (CALC_ROCKET_TRAIL_INDEX(ROCKET_INDEX, TRAIL_INDEX))*particlesPerRocket;

extern Rocket* mRockets;

void Init();

void UpdateParticles(TTimeUnit step);

void UpdateParticle( Rocket* rocket );

void ResetRocket( Rocket* rocket );

void ResetFire( Rocket* rocket );

void UpdateFireParticle( Rocket* rocket );
void UpdateRockets(int rocketIndex, TTimeUnit step, bool restart = true);

void UpdateFire(int rocketIndex, TTimeUnit step, bool restart = true);
void Update(float dt);

void SetupRockets();

