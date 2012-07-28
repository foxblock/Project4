#ifndef _UTILITY_FUNCTIONS_H
#define _UTILITY_FUNCTIONS_H

#include <sstream>
#include <string>

#define FLOAT_ACCURACY 1e-10
#define VEC_DELIMIT_CHAR ((std::string)",")

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

namespace Utility
{
template <class T>
std::string numToStr( T number )
{
	std::stringstream str;
	str << number;
	return str.str();
};

template <class T>
T strToNum( std::string str )
{
	std::stringstream ss;
	T out;
	ss << str;
	ss >> out;
	return out;
};

template <class T>
std::string vecToStr( Vector2d<T> vec )
{
	std::string result = numToStr(vec.x) + VEC_DELIMIT_CHAR + numToStr(vec.y);
	return result;
}

template <class T>
int sign( T number )
{
	return ( ( number > 0 ) - ( 0 > number ) );
}

template <class T>
T sqr( T number )
{
	return number*number;
}

template <class T>
T clamp( T number, T minV, T maxV )
{
	return std::min( std::max( minV, number ), maxV );
}
};

#endif // _UTILITY_FUNCTIONS_H

