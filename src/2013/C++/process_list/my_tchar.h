#ifndef MY_TCHAR_H
#define MY_TCHAR_H

#if defined(UNICODE) || defined(_UNICODE)
#define tstring std::wstring
#define tcout std::wcout
#define tregex std::wregex
#define tformat boost::wformat
#else
#define tstring std::string
#define tcout std::cout
#define tregex std::regex
#define tformat boost::format
#endif

#endif
