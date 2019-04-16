#pragma once


class String : public std::basic_string<TCHAR>
{
private:
	typedef std::basic_string<TCHAR> string_type;
public:
	String();
	~String();
	String(const std::string& str);
	String(const std::wstring& str);
	String(const char* str);
	String(const wchar_t* str);
};
