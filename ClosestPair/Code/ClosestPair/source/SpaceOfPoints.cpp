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
#include <iterator>
#include <stack>
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
			float localDistance = (*itSource).CuadraticDistance(*itTarget);
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
				(temporaryIteration->GetX() - itSource->GetX())
				*
				(temporaryIteration->GetX() - itSource->GetX())
				<minimumDistance
				)
			{
				break;
			}
			++temporaryIteration;
		}
		lowestIteration = temporaryIteration;

		for (auto itTarget = lowestIteration; itTarget != itSource; ++itTarget)
		{
			float localDistance = (*itSource).CuadraticDistance(*itTarget);
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

// A utility function to find the distance beween the closest points of
// strip of given size. All points in strip[] are sorted accordint to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
std::pair<Point,Point> SpaceOfPoints::StripClosest(int indexStart, int indexEnd, int midPoint,float &distance)
{

	std::vector<Point> stripVector;

	std::copy_if(pointsContainer.begin() + indexStart, pointsContainer.begin() + indexEnd, std::back_inserter(stripVector),
		[&](const Point &a) {

		return (a.GetX() - pointsContainer[midPoint].GetX())
			*
			(a.GetX() - pointsContainer[midPoint].GetX())
			< distance;
	}
	);

	auto source = stripVector.begin();
	auto target = stripVector.end()-1;


	std::sort(stripVector.begin() , stripVector.end(),

		[](Point a, Point b) { return a.GetY() < b.GetY(); });


	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times

	for (auto itSource=stripVector.begin(); itSource != stripVector.end();++itSource)
	{

		if (
			(itSource->GetX() - pointsContainer[midPoint].GetX())
			*
			(itSource->GetX() - pointsContainer[midPoint].GetX())
			< distance)
		{
			for (auto itTarget = itSource + 1; itTarget != stripVector.end() && (((itTarget->GetY() - itSource->GetY())* (itTarget->GetY() - itSource->GetY())
				) < distance); ++itTarget)
			{
				float localDistance = itSource->CuadraticDistance(*itTarget);
				if (localDistance < distance)
				{
					source = itSource;
					target = itTarget;
					distance = localDistance;
				}
			}
		}

	}
	return std::pair<Point, Point >(*source,*target);
}

/// <summary>
/// Returns the minimum distance recursive divide and conquer.
/// </summary>
/// <param name="indexStart">The index start.</param>
/// <param name="indexEnd">The index end.</param>
/// <param name="distance">The distance.</param>
/// <returns></returns>
std::pair<Point, Point> SpaceOfPoints::RecursiveDivideAndConquer(int indexStart, int indexEnd,float &distance)
{

	//End condition to finish
	if ((indexEnd - indexStart) < 4)
	{
		int source = indexStart;
		int target = indexEnd;
		for (int itSource = indexStart; itSource != indexEnd - 1; ++itSource)
		{
			for (auto itTarget = itSource + 1; itTarget <= indexEnd; ++itTarget)
			{
				float localDistance = pointsContainer[itSource].CuadraticDistance(pointsContainer[itTarget]);
				if (localDistance < distance)
				{

					source = itSource;
					target = itTarget;
					distance = localDistance;

				}

			}
		}

		return std::pair<Point,Point >(pointsContainer[source], pointsContainer[target]);
	}

	else
	{
		float distanceLeft=FLT_MAX;
		float distanceRight = FLT_MAX;
		int midPoint = (indexEnd + indexStart) / 2;
	
		std::pair<Point, Point> minimumLeft = RecursiveDivideAndConquer(indexStart, midPoint, distanceLeft);

		std::pair<Point, Point> minimumRight = RecursiveDivideAndConquer(midPoint+1, indexEnd, distanceRight);

		std::pair<Point ,Point> finalMinimum = (distanceLeft < distanceRight) ? minimumLeft : minimumRight;

		distance = (distanceLeft < distanceRight) ? distanceLeft : distanceRight;

		float stripDistance = distance;
		
		std::pair<Point,Point > stripMinimum=StripClosest(indexStart, indexEnd, (indexEnd + indexStart) / 2, stripDistance);
		if (stripDistance != distance)
		{
			distance = stripDistance;
			if (distance == 0)
			{
				printf("what is happening");
			}

			return stripMinimum;
		}
		else
		{
			if (distance == 0)
			{
				printf("what is happening");
			}

			return finalMinimum;
		}
	}

}
/// <summary>
/// Returns the minimum distance recursive divide and conquer.
/// </summary>
/// <returns></returns>
std::pair<Point, Point> SpaceOfPoints::ReturnMinimumDistanceRecursiveDivideAndConquer()
{

	std::sort(pointsContainer.begin(), pointsContainer.end(),

		[](Point a, Point b) { return a.GetX() < b.GetX(); });

	float maxDistance = FLT_MAX;
	return RecursiveDivideAndConquer(0, pointsContainer.size() - 1, maxDistance);
}

/// <summary>
/// Returns the minimum distance recursive divide and conquer.
/// </summary>
/// <returns></returns>
std::pair<Point, Point> SpaceOfPoints::CalculateMinimum(int indexStart, int indexEnd,float &distance)
{
	int source = indexStart;
	int target = indexEnd;
	for (int itSource = indexStart; itSource != indexEnd - 1; ++itSource)
	{
		for (auto itTarget = itSource + 1; itTarget <= indexEnd; ++itTarget)
		{
			float localDistance = pointsContainer[itSource].CuadraticDistance(pointsContainer[itTarget]);
			if (localDistance < distance)
			{

				source = itSource;
				target = itTarget;
				distance = localDistance;

			}

		}
	}
	return std::pair<const Point&, const Point &>(pointsContainer[source], pointsContainer[target]);
}
/// <summary>
/// Returns the minimum distance iterative divide and conquer.
/// </summary>
/// <returns></returns>
std::pair<Point,Point> SpaceOfPoints::ReturnMinimumDistanceIterativeDivideAndConquer()
{

	return std::pair<Point, Point>(Point(FLT_MAX, FLT_MAX, FLT_MAX), Point(FLT_MAX, FLT_MAX, FLT_MAX));
}