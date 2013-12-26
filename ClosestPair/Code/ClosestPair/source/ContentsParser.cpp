//===============================================================================
// @ ContentsParser.cpp
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
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>
#include <ContentsParser.h>
#include <Point.h>
#include <SpaceOfPoints.h>
#include <vector>
using namespace FileUtils;

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ ContentsParser::Parse(const string &contents, SpacePoints aSpace)
//-------------------------------------------------------------------------------
// Parsing Methods in charge of reading a string read with the format of Point.X Point.Y Point.Z
//-------------------------------------------------------------------------------
void ContentsParser::Parse(const string &contents,SpaceOfPoints& aSpace)
{
    std::istringstream tokenizer(contents);
    
    float px=0;
    float py=0;
    float pz=0;
    string line;
    std::istringstream linestream;
    while (getline(tokenizer,line))
    {
        linestream.clear();
        linestream.str(line);
        linestream >> px;
        linestream >> py;
        linestream >> pz;
        Point newPoint (px,py,pz);
        aSpace.AddPoint(newPoint);
    }
}