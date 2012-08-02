#include "UtilityFunctions.h"

namespace Utility {

void seedRand( const uint32_t &a, const uint32_t &b,
				const uint32_t &c, const uint32_t &d )
{
	w = a;
	x = b;
	y = c;
	z = d;
}

uint32_t rand()
{
	uint32_t t = x ^ ( x << 11 );
	x = y; y = z; z = w;
	w ^= ( w >> 19 ) ^t ^ ( t >> 8 );

	return w;
}
}
