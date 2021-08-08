#include "StructParticle.h"

StructParticle::StructParticle() :p({ 0.f, 0.f, 0.f }), v({ 0.f, 0.f,0.f }), colour(255.f, 255.f, 255.f, 255.f), mLifeTime(1.f), mAlive(false), mTime(0.f)
{
	D3DXVECTOR4 colourOne, colourTwo, colourThree, colourFour, colourFive;
	colourOne.x = 232; colourOne.y = 92; colourOne.z = 0; colourOne.w = 255;;
	colourTwo.x = 181; colourTwo.y = 0; colourTwo.z = 0; colourTwo.w = 255;
	colourThree.x = 255; colourThree.y = 0; colourThree.z = 0; colourThree.w = 255;
	colourFour.x = 0; colourFour.y = 0; colourFour.z = 0; colourFour.w = 255;
	colourFive.x = 0; colourFive.y = 0; colourFive.z = 0; colourFive.w = 0;
	mColours.push_back(colourOne); mColours.push_back(colourTwo);
	mColours.push_back(colourThree); mColours.push_back(colourFour);
	mColours.push_back(colourFive); mColours.push_back(colourFive);
	mGradientStopCount = short(mColours.size());
}

StructParticle::StructParticle(StructParticle& sp) : colour(sp.colour),
p(sp.p), v(sp.v), mLifeTime(sp.mLifeTime), mTime(sp.mTime), mAlive(sp.mAlive),
mGradientStopCount(sp.mGradientStopCount)
{
	this->a = sp.a;
	for (size_t z = 0; z < sp.mColours.size(); ++z)
		mColours.push_back(mColours[z]);
}