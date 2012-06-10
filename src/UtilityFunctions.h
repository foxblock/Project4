#ifndef _UTILITY_FUNCTIONS_H
#define _UTILITY_FUNCTIONS_H

#include <sstream>
#include <string>

#define FLOAT_ACCURACY 1e-6

namespace Utility
{
template <class T>
std::string numToStr( T number )
{
	std::stringstream str;
	std::string out;
	str << number;
	str >> out;
	return out;
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

