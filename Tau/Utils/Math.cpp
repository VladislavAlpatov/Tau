#include "Math.h"

#define _USE_MATH_DEFINES
#include <cmath>


float RadToDeg(const float rads)
{
	return rads * 180.f / M_PI;
}

float DegToRad(const float degrees)
{
	return degrees * M_PI / 180.f;
}