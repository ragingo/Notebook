
#include <string>
#include <list>

static void split(
	const std::string& str,
	const std::string& delim,
	std::list<std::string>& result)
{
	std::string::size_type offset = 0;
	std::string::size_type pos = 0;
	std::string::const_iterator begin = str.begin();
	std::string::const_iterator end = str.end();

	while((pos = str.find_first_of(delim, offset)) != str.npos)
	{
		if(offset != pos)
		{
			result.push_back(std::string(begin + offset, begin + pos));
		}
		offset = pos + 1;
	}
	result.push_back(std::string(begin + offset, end));
}

#include <atlbase.h>
#include <atlcoll.h>
#include <atlstr.h>

static void split(
	const CString& str,
	const CString& delim,
	CAtlArray<CString>& result)
{
	int pos = 0;
	CString temp;

	while((temp = str.Tokenize(delim, pos)).GetLength() > 0)
	{
		result.Add(temp);
	}
}