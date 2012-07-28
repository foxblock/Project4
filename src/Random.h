#ifndef _RANDOM_H
#define _RANDOM_H

#include <list>
#include <time.h>
#include <stdlib.h>
#include <cstdio>
#include "gameDefines.h"

#define RANDOM Random::getInstance()

class Random
{
private:
	Random()
	{
		fromCache = false;
		srand( time(NULL) );
	}
	static Random *instance;
public:
	static Random *getInstance()
	{
		if (!instance)
			instance = new Random();
		return instance;
	}
	virtual ~Random() {}

	inline int getNumber( int lower = 0, int upper = RAND_MAX )
	{
		int result = 0;
		if ( fromCache )
		{
			if ( !cache.empty() )
			{
				result = cache.front();
				cache.pop_front();
				if ( result < lower || result > upper )
					printf("%s Random number from cache not matching bounds - supplying actual random number.\n", WARNING_STRING);
				else
					return result;
			}
			else
			{
				printf("%s Random cache ran out - supplying actual random numbers from now on. May fuck up a running replay.\n", WARNING_STRING );
				fromCache = false;
			}
		}

		if ( upper < lower )
		{
			int temp = lower;
			lower = upper;
			upper = temp;
		}

		if ( lower < 0 && upper < 0 )
			result = -( rand() % ( -lower + upper + 1 ) - upper );
		else
			result =  rand() % ( upper - lower + 1 ) + lower;

		if ( !fromCache )
			cache.push_back( result );
		return result;
	}

	void loadCache( const std::list< int > &numbers )
	{
		fromCache = true;
		cache = numbers;
	}
	void copyCache( std::list< int > &ccopy )
	{
		ccopy = cache;
	}
	void clearCache()
	{
		cache.clear();
		fromCache = false;
	}
protected:
	std::list< int > cache;
	bool fromCache;
private:

};

#endif // _RANDOM_H

