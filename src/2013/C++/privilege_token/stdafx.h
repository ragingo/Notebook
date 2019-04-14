
#pragma once

#include "targetver.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <tchar.h>

#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>

#include <Windows.h>
#include <NTSecAPI.h>

#if defined(UNICODE) || defined(_UNICODE)
#define tstring std::wstring
#define tcout std::wcout
#define tformat boost::wformat
#else
#define tstring std::string
#define tcout std::cout
#define tformat boost::format
#endif