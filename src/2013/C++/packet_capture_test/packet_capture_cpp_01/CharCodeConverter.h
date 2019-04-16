#pragma once

class CharCodeConverter
{
public:
	CharCodeConverter();
	~CharCodeConverter();

	static std::wstring ToWideString(const std::string& str);
	static std::string ToMultiByteString(const std::wstring& str);
};

