#pragma once

#ifndef untilities_mine
#define untilities_mine

#include <math.h>
//random useful functions, classes and functors

#define MAX_PARTICLES_PER_ROCKET 1000
#define WHEN_TO_CREATE_NEW_PARTICLE_ROCKET 0.002f
#define PARTICLE_LIFE_TIME 0.05f
enum ROCKET_COLOURS {RED,BLUE,GREEN,YELLOW};

template<class type_specifier>
type_specifier randInRange(type_specifier lower_bound,type_specifier upper_bound)
{	
	return (type_specifier)(lower_bound+rand()%((int)(((upper_bound - lower_bound)*100)==0.0f? 1 : ((upper_bound - lower_bound)*100))))/100;
}

class sparkle
{
protected:
	float* colour_,*sparkle_colour_;
	float* temp_colour;
	float  random; //used as an offset
	bool destroyed;
public:
	sparkle(float colour[],float sparkle_colour[])
	{
		destroyed = false;
		random = randInRange<float>(0.0f,6.0f);

		colour_ 		= new float[3];
		sparkle_colour_ = new float[3];
		temp_colour 	= new float[3];
		
		colour_[0] = colour[0];
		colour_[1] = colour[1];
		colour_[2] = colour[2];

		sparkle_colour_[0] = sparkle_colour[0];
		sparkle_colour_[1] = sparkle_colour[1];
		sparkle_colour_[2] = sparkle_colour[2];
	}

	sparkle() 
	{
		destroyed = false;
		random = randInRange<float>(0.0f,6.0f);

		colour_ 		= new float[3];
		sparkle_colour_ = new float[3];
		temp_colour 	= new float[3];
		
		colour_[0] = 0.0f;
		colour_[1] = 0.0f;
		colour_[2] = 0.0f;

		sparkle_colour_[0] = 0.0f;
		sparkle_colour_[1] = 0.0f;
		sparkle_colour_[2] = 0.0f;
	}
	

	void init(float colour[],float sparkle_colour[])
	{
		destroyed = false;
		random = randInRange<float>(0.0f,6.0f);

		colour_ 		= new float[3];
		sparkle_colour_ = new float[3];
		temp_colour 	= new float[3];
		
		colour_[0] = colour[0];
		colour_[1] = colour[1];
		colour_[2] = colour[2];

		sparkle_colour_[0] = sparkle_colour[0];
		sparkle_colour_[1] = sparkle_colour[1];
		sparkle_colour_[2] = sparkle_colour[2];
	}
	
	~sparkle()
	{
		destroyed = true;
		delete[] colour_;
		delete[] sparkle_colour_;
		delete[] temp_colour;
	}

	float* operator() (float t)
	{
		float delta = tan(18.0f*(t+random)*random);//pow(2.0f,0.1f*(sin((t+random))));
		delta *= delta;
		
		temp_colour[0] = (colour_[0] + (delta)*sparkle_colour_[0]) > 1.0f ? 1.0f:(colour_[0] + (delta)*sparkle_colour_[0]);
		temp_colour[1] = (colour_[1] + (delta)*sparkle_colour_[1]) > 1.0f ? 1.0f:(colour_[1] + (delta)*sparkle_colour_[1]);
		temp_colour[2] = (colour_[2] + (delta)*sparkle_colour_[2]) > 1.0f ? 1.0f:(colour_[2] + (delta)*sparkle_colour_[2]);
		return temp_colour;
	}

	float* operator() (float t, float brightness)
	{
		float delta = tan(18.0f*(t+random)*random);//pow(2.0f,0.1f*(sin((t+random))));
		delta *= delta;
		
		temp_colour[0] = brightness * (colour_[0] + (delta)*sparkle_colour_[0]) > 1.0f ? 1.0f:(colour_[0] + (delta)*sparkle_colour_[0]);
		temp_colour[1] = brightness * (colour_[1] + (delta)*sparkle_colour_[1]) > 1.0f ? 1.0f:(colour_[1] + (delta)*sparkle_colour_[1]);
		temp_colour[2] = brightness * (colour_[2] + (delta)*sparkle_colour_[2]) > 1.0f ? 1.0f:(colour_[2] + (delta)*sparkle_colour_[2]);
		return temp_colour;
	}

};

#endif