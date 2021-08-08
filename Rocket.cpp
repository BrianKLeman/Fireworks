#include "Rocket.h"

Rocket::Rocket(TIndex start, TIndex count) : mStart(start), mCount(count), StructParticle(), mNextParticleIndex(start)
{

}

Rocket::Rocket()
	: StructParticle()
{

}

Rocket::Rocket(Rocket& cpy) : mStart(cpy.mStart), mCount(cpy.mCount), mNextParticleIndex(cpy.mNextParticleIndex), StructParticle(cpy)
{

}
