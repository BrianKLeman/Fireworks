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

struct StructParticle
{
	D3DXVECTOR4 colour;
	D3DXVECTOR3 p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 v = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 a = D3DXVECTOR3(0.f,0.f,0.f);
	TTimeUnit mLifeTime;
	TTimeUnit mTime;
	bool mAlive;
	vector<D3DXVECTOR4> mColours;
	short mGradientStopCount;
	StructParticle();

	StructParticle(StructParticle& sp);
};
