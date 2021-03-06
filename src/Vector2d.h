#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include <cmath>
#include "SDL_video.h"

template <typename T>
class Vector2d
{
public:
	Vector2d() : x( 0 ), y( 0 ) { }
	Vector2d( const T &x, const T &y ) : x( x ), y( y ) { }
	Vector2d( const Vector2d<T> &v ) : x( v.x ), y( v.y ) { }
	// Compatibility to other template specialisations
	template <class S>
	Vector2d( const Vector2d<S> &v ) : x( v.x ), y( v.y ) { }

	~Vector2d() {;}
	void null()
	{
		x = 0;
		y = 0;
	}

	T dot( const Vector2d<T> &v ) const {return ( x * v.x + y * v.y );}
	T length() const {return sqrt( lengthSquared() );}
	T lengthSquared() const {return ( x * x + y * y );}
	// Returns the angle between a line drawn in the direction of the vector and
	// the coordinate-space (range: 0 to 2*M_PI)
	float angle() const 
	{
		if ( x >= 0 && y >= 0 )
			return atan( y / x );
		else if ( x < 0 && y > 0 )
			return M_PI_2 + atan( fabs( x ) / y );
		else if ( x < 0 && y < 0 )
			return M_PI + atan( y / x );
		else
			return M_PI + M_PI_2 + atan( x / fabs( y ) );
	}
	float angle(const Vector2d<T> &v) const
	{
		return acos(dot(v)/length()/v.length());
	}
	Vector2d<T> rotate(const float &atAngle) const
	{
		return Vector2d<T>(cos(atAngle) * x + sin(atAngle) * y, -sin(atAngle) * x + cos(atAngle) * y);
	}
	void rotateThis(const float &atAngle)
	{
		float temp = cos(atAngle) * x + sin(atAngle) * y;
		y = -sin(atAngle) * x + cos(atAngle) * y;
		x = temp;
	}

	Vector2d<T> unit() const
	{
		if ( x == 0 && y == 0 )
			return Vector2d<T>( 0, 0 );
		else
		{
			float len = length();
			return Vector2d<T>( x / len, y / len );
		}
	}
	void makeUnit()
	{
		if ( x == 0 && y == 0 )
			return;
		else
		{
			float len = length();
			x /= len;
			y /= len;
		}
	}

	bool isInRect( const SDL_Rect &rect ) const
	{
		return ( x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h );
	}
	bool isInRect( const Vector2d<float> &topLeft, const Vector2d<float> &bottomRight ) const
	{
		return ( x >= topLeft.x && y >= topLeft.y && x <= bottomRight.x && y <= bottomRight.y );
	}
	bool isInRect( const float &left, const float &top, const float &right, const float &bottom ) const
	{
		return ( x >= left && y >= top && x <= right && y <= bottom );
	}

	/// Operators
	// Assignment
	Vector2d<T> operator=( const Vector2d<T> &v ) {x = v.x; y = v.y; return *this;}
	template <class S>
	Vector2d<T> operator=( const Vector2d<S> &v ) {x = v.x; y = v.y; return *this;}

	// Addition
	Vector2d<T> operator+( const Vector2d<T> &v )const {return Vector2d<T> ( x + v.x, y + v.y );}
	template <class S>
	Vector2d<T> operator+( const Vector2d<S> &v )const {return Vector2d<T> ( x + v.x, y + v.y );}

	// Subtraction
	Vector2d<T> operator-( const Vector2d<T> &v )const {return Vector2d<T> ( x - v.x, y - v.y );}
	template <class S>
	Vector2d<T> operator-( const Vector2d<S> &v )const {return Vector2d<T> ( x - v.x, y - v.y );}

	// add and assign
	Vector2d<T> operator+=( const Vector2d<T> &v ) {return Vector2d<T> ( x += v.x, y += v.y );}
	template <class S>
	Vector2d<T> operator+=( const Vector2d<S> &v ) {return Vector2d<T> ( x += v.x, y += v.y );}

	// subtract and assign
	Vector2d<T> operator-=( const Vector2d<T> &v ) {return Vector2d<T> ( x -= v.x, y -= v.y );}
	template <class S>
	Vector2d<T> operator-=( const Vector2d<S> &v ) {return Vector2d<T> ( x -= v.x, y -= v.y );}

	// Multiplication
	Vector2d<T> operator*( const Vector2d<T> &v )const {return Vector2d<T>( x * v.x, y * v.y );}
	template <class S>
	Vector2d<T> operator*( const Vector2d<S> &v )const {return Vector2d<T>( x * v.x, y * v.y );}
	Vector2d<T> operator*( const T &s )const {return Vector2d<T>( x * s, y * s );}
	template <class S>
	Vector2d<T> operator*( const S &s )const {return Vector2d<T>( x * s, y * s );}

	// Division
	Vector2d<T> operator/( const Vector2d<T> &v )const {return Vector2d<T>( x / v.x, y / v.y );}
	template <class S>
	Vector2d<T> operator/( const Vector2d<S> &v )const {return Vector2d<T>( x / v.x, y / v.y );}
	Vector2d<T> operator/( const T &s )const {return Vector2d<T>( x / s, y / s );}
	template <class S>
	Vector2d<T> operator/( const S &s )const {return Vector2d<T>( x / s, y / s );}

	// Multiplay and assign
	Vector2d<T> operator*=( const Vector2d<T> &v ) {return Vector2d<T> ( x *= v.x, y *= v.y );}
	template <class S>
	Vector2d<T> operator*=( const Vector2d<S> &v ) {return Vector2d<T> ( x *= v.x, y *= v.y );}
	Vector2d<T> operator*=( const T &s ) {return Vector2d<T>( x *= s, y *= s );}
	template <class S>
	Vector2d<T> operator*=( const S &s ) {return Vector2d<T>( x *= s, y *= s );}

	// Division and assign
	Vector2d<T> operator/=( const Vector2d<T> &v ) {return Vector2d<T> ( x /= v.x, y /= v.y );}
	template <class S>
	Vector2d<T> operator/=( const Vector2d<S> &v ) {return Vector2d<T> ( x /= v.x, y /= v.y );}
	Vector2d<T> operator/=( const T &s ) {return Vector2d<T>( x /= s, y /= s );}
	template <class S>
	Vector2d<T> operator/=( const S &s ) {return Vector2d<T>( x /= s, y /= s );}

	// Comparison
	bool operator==( const Vector2d<T> &v ) const { return ( x == v.x ) && ( y == v.y ); }
	template <class S>
	bool operator==( const Vector2d<S> &v ) const { return ( x == v.x ) && ( y == v.y ); }

	// Negator
	Vector2d<T> operator-() const { return Vector2d<T>(-x,-y); }

	T x;
	T y;
};


#endif // _VECTOR2D_H

