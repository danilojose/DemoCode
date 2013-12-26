#include "stdafx.h"
#include <FileReader.h>
#include <ContentsParser.h>
#include <SpaceOfPoints.h>
#include <string>
#include <ctime>
#include <iostream>
#include <cerrno>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const char * sphereTestFile = "..\\..\\data\\sphere.txt";
const char * volumeTestFile = "..\\..\\data\\volume.txt";
const char * volumeShortTestFile = "..\\..\\data\\volumeshort.txt";


namespace ClosestPointsTest
{
	TEST_CLASS(ClosestPointsClass)
	{
	public:

		TEST_METHOD(TestDistance)
		{
			// TODO: Your test code here
			Point a(1.0f, 0.0f, 0.0f);
			Assert::IsTrue(a.Distance(Point::origin) == 1);
		}


		TEST_METHOD(TestDistanceRealNumbers)
		{
			// TODO: Your test code here
			Point a(-687.201f, 9.86516f, -360.643f);
			Point b(-684.009f, 5.5393f, -360.989f);
			float outputDistance = a.Distance(b);
			Assert::IsTrue(outputDistance == 5.38718462f);
		}


		TEST_METHOD(CheckTestPointsBruteForceSphere)
		{
			time_t tstart, tend;
			tstart = time(0);
			string contents = FileReader::GetFileContents(sphereTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceBruteForce();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = time(0);

			cout << "Minimum points are " << endl;
			cout << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			cout << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			cout << "Distance between points is " << distance << endl;
			cout << "It took" << difftime(tend, tstart) << "second(s)." << endl;

		}




	};
}