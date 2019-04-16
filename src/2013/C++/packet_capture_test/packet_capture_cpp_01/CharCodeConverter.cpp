#include "StdAfx.h"
#include "CharCodeConverter.h"


CharCodeConverter::CharCodeConverter()
{
}


CharCodeConverter::~CharCodeConverter()
{
}

std::wstring CharCodeConverter::ToWideString(const std::string& str)
{
	return std::wstring(L"");
}

std::string CharCodeConverter::ToMultiByteString(const std::wstring& str)
{
	return std::string("");
}
