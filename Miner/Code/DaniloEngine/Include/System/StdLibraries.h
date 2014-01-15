#pragma once

//// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
//
//
//// TODO: reference additional headers your program requires here
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>


#if defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#	define GCC_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define GCC_NEW new
#endif


#if defined(_DEBUG)
#	define DEBUG_PRINT printf
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
