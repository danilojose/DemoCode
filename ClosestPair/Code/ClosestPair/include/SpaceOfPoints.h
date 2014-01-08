//===============================================================================
// @ SpaceOfPoints.h
// 
// Description
// ------------------------------------------------------------------------------
// CopyRight Danilo José Guerrero Rodríguez
//
//
//
//===============================================================================

#ifndef __SpaceOfPoints__h__
#define __SpaceOfPoints__h__
//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------
#include "Point.h"
#include <vector>
#include <map>
//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class SpaceOfPoints
{
private:

struct cmpVersusOrigin {
    inline bool operator()(const Point& a, const Point& b) const {

        // compute the cross product of vectors (center -> a) x (center -> b)

        float detX = (a.GetY()-Point::origin.GetY()) * (b.GetZ()-Point::origin.GetZ()) 
            
                    - (b.GetY() - Point::origin.GetY()) * (a.GetZ() - Point::origin.GetZ());

        float detY = (a.GetX()-Point::origin.GetX()) * (b.GetZ()-Point::origin.GetZ()) 
            
                    - (b.GetX() - Point::origin.GetX()) * (a.GetZ() - Point::origin.GetZ());

        float detZ = (a.GetX()-Point::origin.GetX()) * (b.GetY()-Point::origin.GetY()) 
            
                    - (b.GetX() - Point::origin.GetX()) * (a.GetY() - Point::origin.GetY());


        float det=detX-detY+detZ;

        if (det < 0)
            return true;
        if (det > 0)
            return false;

        // points a and b are on the same line from the center
        // check which point is closer to the center
        float d1 = (a.GetX()-Point::origin.GetX()) * (a.GetX()-Point::origin.GetX()) + (a.GetY()-Point::origin.GetY()) * (a.GetY()-Point::origin.GetY())+(a.GetZ()-Point::origin.GetZ()) * (a.GetZ()-Point::origin.GetZ());

        float d2 = (b.GetX()-Point::origin.GetX()) * (b.GetX()-Point::origin.GetX()) + (b.GetY()-Point::origin.GetY()) * (b.GetY()-Point::origin.GetY())+ (b.GetZ()-Point::origin.GetZ()) * (b.GetZ()-Point::origin.GetZ());
        return d1 > d2;

    }
};

    std::vector<Point> pointsContainer;
public:
    // constructor/destructor
    inline SpaceOfPoints() {
       
    }
    inline ~SpaceOfPoints() {}

    void AddPoint(Point& aPoint);

    std::pair<const Point&,const Point&> ReturnMinimumDistanceBruteForce();

	std::pair<const Point&, const Point&> ReturnMinimumDistanceSortingAndProning();


	std::pair<Point,Point> StripClosest(int indexStart, int indexEnd, int midPoint,float &distance);

	std::pair<Point,Point> RecursiveDivideAndConquer(int indexStart, int indexEnd, float &distance);

	std::pair<Point, Point> ReturnMinimumDistanceRecursiveDivideAndConquer();

	std::pair<Point, Point> CalculateMinimum(int indexStart, int indexEnd, float &distance);
	std::pair<Point,Point> ReturnMinimumDistanceIterativeDivideAndConquer();
    

protected:

};

#endif