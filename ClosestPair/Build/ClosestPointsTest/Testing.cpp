#include "stdafx.h"
#include "CppUnitTest.h"
#include "stdafx.h"
#include <FileReader.h>
#include <ContentsParser.h>
#include <SpaceOfPoints.h>
#include <string>
#include <chrono>
#include <iostream>
#include <cerrno>
#include <sstream>


using namespace FileUtils;
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
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(sphereTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceBruteForce();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(4);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;

			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.138299f);

			Assert::IsTrue(returnedPar.first == (Point(-77.3131409f, -222.542023f, 9997.22461f)));

			Assert::IsTrue(returnedPar.second == (Point(-77.3083115f, -222.680206f, 9997.22168f)));

		}

		TEST_METHOD(CheckTestPointsBruteForceVolume)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceBruteForce();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.909200907f);

			Assert::IsTrue(returnedPar.first == (Point(-486.013336f, 571.570618f, 966.630005f)));

			Assert::IsTrue(returnedPar.second == (Point(-485.765381f, 572.437988f, 966.743286f)));


		}


		TEST_METHOD(CheckTestPointsBruteForceVolumeShort)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeShortTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceBruteForce();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 5.40938807f);

			Assert::IsTrue(returnedPar.first == (Point(771.872742f, 755.730591f, -213.203262f)));

			Assert::IsTrue(returnedPar.second == (Point(771.761597f, 753.115662f, -217.937317f)));


		}


		TEST_METHOD(CheckTestPointsSortingAndProningSphere)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(sphereTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceSortingAndProning();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(4);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;

			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.138299f, (wchar_t*)"Distance found is not the minimum");

			Assert::IsTrue(returnedPar.first == (Point(-77.3083115f, -222.680206f, 9997.22168f)), (wchar_t*)"Point A is not the expected");

			Assert::IsTrue(returnedPar.second == (Point(-77.3131409f, -222.542023f, 9997.22461f)), (wchar_t*)"Point B is not the expected");

			Assert::IsTrue(micros.count() < 300247, (wchar_t*)"We are not reducing the time constraint 163353808");



		}

		TEST_METHOD(CheckTestPointsSortingAndProningVolume)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceSortingAndProning();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.909200907f);

			Assert::IsTrue(returnedPar.first == (Point(-485.765381f, 572.437988f, 966.743286f)));
			Assert::IsTrue(returnedPar.second == (Point(-486.013336f, 571.570618f, 966.630005f)));


			Assert::IsTrue(micros.count() < 1500000, (wchar_t*)"We are not reducing the time constraint 163353808");

		}


		TEST_METHOD(CheckTestPointsSortingAndProningVolumeShort)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeShortTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceSortingAndProning();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 5.40938807f);

			Assert::IsTrue(returnedPar.first == (Point(771.872742f, 755.730591f, -213.203262f)));

			Assert::IsTrue(returnedPar.second == (Point(771.761597f, 753.115662f, -217.937317f)));

			Assert::IsTrue(micros.count() < (11409942 / 10), (wchar_t*)"We are not reducing the time constraint 163353808");

		}


		TEST_METHOD(CheckTestPointsRecursiveDivideAndConquerSphere)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(sphereTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceRecursiveDivideAndConquer();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(4);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;

			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.138299f);

			Assert::IsTrue(returnedPar.first == (Point(-77.3131409f, -222.542023f, 9997.22461f)));

			Assert::IsTrue(returnedPar.second == (Point(-77.3083115f, -222.680206f, 9997.22168f)));


		}

		TEST_METHOD(CheckTestPointsRecursiveAndDivideAndConquerVolume)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceRecursiveDivideAndConquer();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.909200907f);

			Assert::IsTrue(returnedPar.first == (Point(-486.013336f, 571.570618f, 966.630005f)));

			Assert::IsTrue(returnedPar.second == (Point(-485.765381f, 572.437988f, 966.743286f)));

			Assert::IsTrue(micros.count() < (649467422 / 10), (wchar_t*)"We are not reducing the time constraint 163353808");


		}


		TEST_METHOD(CheckTestPointsRecursiveAndDivideAndConquerVolumeShort)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeShortTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<Point, Point> returnedPar = space.ReturnMinimumDistanceRecursiveDivideAndConquer();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 5.40938807f);

			Assert::IsTrue(returnedPar.second == (Point(771.872742f, 755.730591f, -213.203262f)));

			Assert::IsTrue(returnedPar.first == (Point(771.761597f, 753.115662f, -217.937317f)));

			Assert::IsTrue(micros.count() < (649467422 / 10), (wchar_t*)"We are not reducing the time constraint 163353808");


		}
		TEST_METHOD(CheckTestPointsIterativeDivideAndConquerSphere)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(sphereTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceIterativeDivideAndConquer();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(4);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;

			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.138299f);

			Assert::IsTrue(returnedPar.first == (Point(-77.3083115f, -222.680206f, 9997.22168f)));

			Assert::IsTrue(returnedPar.second == (Point(-77.3131409f, -222.542023f, 9997.22461f)));

		}

		TEST_METHOD(CheckTestPointsIterativeAndDivideAndConquerVolume)
		{
			std::chrono::high_resolution_clock::time_point tstart, tend;
			string contents = FileReader::GetFileContents(volumeTestFile);

			SpaceOfPoints space;
			ContentsParser::Parse(contents, space);

			tstart = std::chrono::high_resolution_clock::now();
			std::pair<const Point &, const Point&> returnedPar = space.ReturnMinimumDistanceIterativeDivideAndConquer();
			float distance = const_cast<Point&>(returnedPar.first).Distance(returnedPar.second);
			tend = std::chrono::high_resolution_clock::now();

			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
			stringstream sbuffer;
			sbuffer.precision(6);
			sbuffer << "Minimum points are : " << endl;
			sbuffer << "Point A (" << returnedPar.first.GetX() << "," << returnedPar.first.GetY() << "," << returnedPar.first.GetZ() << ")" << endl;
			sbuffer << "Point B (" << returnedPar.second.GetX() << "," << returnedPar.second.GetY() << "," << returnedPar.second.GetZ() << ")" << endl;
			sbuffer << "Distance between points is " << distance << endl;
			sbuffer << "It took  " << micros.count() << "  microsecond(s).  " << endl;
			std::string s = sbuffer.str();
			Logger::WriteMessage(s.c_str());

			Assert::IsTrue(distance == 0.909200907f);

			Assert::IsTrue(returnedPar.first == (Point(-486.013336f, 571.570618f, 966.630005f)));

			Assert::IsTrue(returnedPar.second == (Point(-485.765381f, 572.437988f, 966.743286f)));

			Assert::IsTrue(micros.count() < (649467422 / 10), (wchar_t*)"We are not reducing the time constraint 163353808");


		}


	};
}