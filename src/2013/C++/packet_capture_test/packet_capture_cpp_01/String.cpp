#include "StdAfx.h"
#include "String.h"
#include "CharCodeConverter.h"

String::String()
{
}

String::~String()
{
}

String::String(const char* str)
#ifdef _UNICODE
	: string_type(CharCodeConverter::ToWideString(str))
#else
	: string_type(str)
#endif
{
}

String::String(const wchar_t* str)
#ifdef _UNICODE
	: string_type(str)
#else
	: string_type(CharCodeConverter::ToWideString(str))
#endif
{
}

String::String(const std::string& str)
#ifdef _UNICODE
	: string_type(CharCodeConverter::ToWideString(str))
#else
	: string_type(str)
#endif
{
}

String::String(const std::wstring& str)
#ifdef _UNICODE
	: string_type(str)
#else
	: string_type(CharCodeConverter::ToWideString(str))
#endif
{
}
