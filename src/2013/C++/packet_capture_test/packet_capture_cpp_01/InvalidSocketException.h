#pragma once
#include "Exception.h"

class InvalidSocketException : public Exception
{
private:
	int m_ErrorCode;
public:
	InvalidSocketException(int errorCode);

	InvalidSocketException(const std::string& message);

	InvalidSocketException(int errorCode, const std::string& message);
};
