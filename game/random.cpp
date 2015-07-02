#include "random.h"
#include <math.h>
Random::Random()
{
	n = 9.036192184;
	reset();
}

float Random::frandom()
{
	x = x * n;
	x = x-floor(x);
	if (x==0.0f)
		x = 0.9036192184;
	return (float) x;
}
int Random::irandom (int N)
{
	float xx = frandom() *N;
    return (int) xx;
}

float Random::frandom(float lo, float hi)
{
	double xx = frandom();
	xx = lo + xx * (hi-lo);
	return xx;
}

int Random::irandom(int lo, int hi)
{
	double xx = frandom();
	return (int) round (lo + xx * (hi-lo));

}

void Random::reset()
{
	x = 0.9036192184;
}
