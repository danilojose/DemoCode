//===============================================================================
// @ Point.h
// 
// Description
// ------------------------------------------------------------------------------
// CopyRight Danilo José Guerrero Rodríguez
//
//
//
//===============================================================================

#ifndef __Point__h__
#define __Point__h__
//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------
#include <math.h>
#include <assert.h>

//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class Point
{
    
public:
    // constructor/destructor
    inline Point() {}
    inline Point( float _x, float _y, float _z ) :
        x(_x), y(_y), z(_z)
    {
    }
    inline ~Point() {}

    // copy operations
    Point(const Point& other);
    Point& operator=(const Point& other);

    // operators overload 

	bool operator==(const Point & other) const
	{
		return (x == other.GetX()) && (y == other.GetY()) && (z == other.GetZ());
	}


    float Distance( const Point& anotherPoint );
	float CuadraticDistance(const Point& anotherPoint);

    // manipulators
    inline void Set( float _x, float _y, float _z );
    inline void SetX( float _x ) { x = _x; }
    inline void SetY( float _y ) { y = _y; }
    inline void SetZ( float _z ) { z = _z; }
    inline float GetX() const { return x; }
    inline float GetY() const { return y; }
    inline float GetZ() const { return z; }

    // useful defaults
    static Point    xAxis;
    static Point    yAxis;
    static Point    zAxis;
    static Point    origin;
    static Point    cornerPoint;

protected:
    // member variables
    float x, y, z;

};

#endif