//===============================================================================
// @ ContentsParser.h
// 
// Description ContentsParser into 3 coordinates points
// ------------------------------------------------------------------------------
// CopyRight Danilo Jos� Guerrero Rodr�guez
//
//
//
//===============================================================================

#ifndef __ContentsParser__h__
#define __ContentsParser__h__
//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------
#include <string>
#include <vector>
#include <Point.h>
#include <SpaceOfPoints.h>
using namespace std;


//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------


namespace FileUtils
{
class ContentsParser
{
public:
    ContentsParser(){}
    static void Parse(const string &contents,SpaceOfPoints& aSpace);

};
}
#endif