#ifndef _UTILITY_FUNCTIONS_H
#define _UTILITY_FUNCTIONS_H

#include <sstream>
#include <string>
#include <vector>
#include "Vector2d.h"
#include "sparrowFont.h"

#define FLOAT_ACCURACY 1e-10
#define MY_RAND_MAX 2147483647
#define VEC_DELIMIT_CHAR ((std::string)",")

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#ifdef _DEBUG
#define LOG_MESSAGE(x) printf("%s:%i\t\t%s\n",__FILE__,__LINE__,(x))
#else
#define LOG_MESSAGE(x)
#endif

namespace Utility
{

// -------- FUNCTIONS ----------------------------------------------------------

template <class T>
inline std::string numToStr( const T &number )
{
	std::ostringstream str;
	str << number;
	return str.str();
};

template <class T>
inline T strToNum( const std::string &str )
{
	std::istringstream ss (str);
	T out;
	ss >> out;
	return out;
};
template <class T>
inline T strToNum( const char* &str )
{
	std::istringstream ss (str);
	T out;
	ss >> out;
	return out;
};
template <class T>
inline T strToNum( const char &str )
{
	std::stringstream ss;
	ss << str;
	T out;
	ss >> out;
	return out;
};

template <class T>
inline std::string vecToStr( const Vector2d<T> &vec )
{
	std::string result = numToStr<T>( vec.x ) + VEC_DELIMIT_CHAR + numToStr<T>( vec.y );
	return result;
}

template <class T>
inline int sign( const T &number )
{
	return ( ( number > 0 ) - ( 0 > number ) );
}

template <class T>
inline T sqr( const T &number )
{
	return number * number;
}

template <class T>
inline T clamp( const T &number, const T &minV, const T &maxV )
{
	return std::min( std::max( minV, number ), maxV );
}

void tokenize( const std::string &str, std::vector<std::string> &tokens,
				const std::string &delimiters, const int &maxParts = -1 );

extern Uint32 w;
extern Uint32 x;
extern Uint32 y;
extern Uint32 z;

void seedRand( const Uint32 &a = 88675123, const Uint32 &b = 123456789,
				const Uint32 &c = 362436069, const Uint32 &d = 521288629);

Uint32 rand();

inline Sint32 randomRange( Sint32 lower = 0, Sint32 upper = MY_RAND_MAX )
{
	if ( upper < lower )
	{
		Sint32 temp = lower;
		lower = upper;
		upper = temp;
	}

	if ( lower < 0 && upper < 0 )
		return -( Utility::rand() % ( -lower + upper + 1 ) - upper );
	else
		return Utility::rand() % ( upper - lower + 1 ) + lower;
}

inline bool floatComp( const float &a, const float &b )
{
	return ( abs( a - b ) < FLOAT_ACCURACY );
}

inline bool floatComp( const Vector2d<float> &a, const Vector2d<float> &b )
{
	return ( floatComp(a.x, b.x) && floatComp(a.y, b.y) );
}

void setFontAlpha( spFontPointer font, const int &alpha );

// -------- STRUCTS ------------------------------------------------------------

struct colour
{
	float r,g,b,intensity;
};

};

#endif // _UTILITY_FUNCTIONS_H

