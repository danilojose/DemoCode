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

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <SpaceOfPoints.h>
#include <algorithm>
#include <math.h>
//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------


/// <summary>
/// Adds the point.
/// </summary>
/// <param name="aPoint">A point.</param>
void SpaceOfPoints::AddPoint( Point& aPoint )
{
	pointsContainer.push_back(aPoint);
}   

/// <summary>
/// Returns the minimum distance brute force.
/// </summary>
/// <returns></returns>
std::pair<const Point&, const Point&> SpaceOfPoints::ReturnMinimumDistanceBruteForce()
{

    float minimumDistance=FLT_MAX;
    auto source=pointsContainer.begin();
    auto target=(pointsContainer.end());
    target--;
    auto itTarget=pointsContainer.begin();
    itTarget++;

	for (auto itSource = pointsContainer.begin(); itSource != pointsContainer.end(); ++itSource)
	{
		for (auto itTarget = itSource + 1; itTarget != pointsContainer.end(); ++itTarget)
		{
			float localDistance = (*itSource).Distance(*itTarget);
			if (localDistance<minimumDistance)
			{
				source = itSource;
				target = itTarget;
				minimumDistance = localDistance;

			}

		}

	}
    return std::pair<const Point&,const Point&>((*source),(*target));    
}   



/// <summary>
/// Returns the minimum distance sorting and proning.
/// </summary>
/// <returns></returns>
std::pair<const Point&, const Point&> SpaceOfPoints::ReturnMinimumDistanceSortingAndProning()
{
	std::sort(pointsContainer.begin(), pointsContainer.end(),
		
		[](Point a, Point b) { return a.GetX() < b.GetX(); });


	float minimumDistance = FLT_MAX;
	auto source = pointsContainer.begin();
	auto lowestIteration = pointsContainer.begin();
	auto target =--(pointsContainer.end());


	for (auto itSource = pointsContainer.begin(); itSource != pointsContainer.end(); ++itSource)
	{

		auto temporaryIteration = lowestIteration;
		while (temporaryIteration != itSource)
		{
			if ( 
				(fabs(temporaryIteration->GetX() - itSource->GetX())<minimumDistance)
				)
			{
				break;
			}
			++temporaryIteration;
		}
		lowestIteration = temporaryIteration;

		for (auto itTarget = lowestIteration; itTarget != itSource; ++itTarget)
		{
			float localDistance = (*itSource).Distance(*itTarget);
			if (localDistance<minimumDistance)
			{
				source = itSource;
				target = itTarget;
				minimumDistance = localDistance;

			}

		}

	}

	return std::pair<const Point &, const Point &> (*source,*target);
}

/// <summary>
/// Returns the minimum distance recursive divide and conquer.
/// </summary>
/// <returns></returns>
std::pair<const Point&, const Point&> SpaceOfPoints::ReturnMinimumDistanceRecursiveDivideAndConquer()
{
	return std::pair<const Point &, const Point &>(Point(0, 0, 0), Point(1, 1, 1));
}

/// <summary>
/// Returns the minimum distance iterative divide and conquer.
/// </summary>
/// <returns></returns>
std::pair<const Point&, const Point&> SpaceOfPoints::ReturnMinimumDistanceIterativeDivideAndConquer()
{
	return std::pair<const Point &, const Point &>(Point(0, 0, 0), Point(1, 1, 1));
}