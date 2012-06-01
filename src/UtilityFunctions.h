#ifndef _UTILITY_FUNCTIONS_H
#define _UTILITY_FUNCTIONS_H

#include <sstream>
#include <string>

namespace Utility
{
	template <class T>
	std::string numToStr(T number)
	{
		std::stringstream str;
		std::string out;
		str << number;
		str >> out;
		return out;
	};

	template <class T>
	T strToNum(std::string str)
	{
		std::stringstream ss;
		T out;
		ss << str;
		ss >> out;
		return out;
	};

	template <class T>
	int sign(T number)
	{
		return ((number > 0) - (0 > number));
	}
};

#endif // _UTILITY_FUNCTIONS_H

