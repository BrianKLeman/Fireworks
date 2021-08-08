#pragma once

#include "StructParticle.h"

struct Rocket
	: public StructParticle
{
	TIndex mStart, mCount, mNextParticleIndex;
	Rocket(TIndex start, TIndex count);

	Rocket();
	Rocket(Rocket& cpy);
};

