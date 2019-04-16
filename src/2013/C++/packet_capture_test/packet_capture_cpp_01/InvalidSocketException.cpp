#include "StdAfx.h"
#include "InvalidSocketException.h"


InvalidSocketException::InvalidSocketException(int errorCode)
	: m_ErrorCode(errorCode)
{
}

InvalidSocketException::InvalidSocketException(const std::string& message)
	: Exception(message)
{
}

InvalidSocketException::InvalidSocketException(int errorCode, const std::string& message)
	: m_ErrorCode(errorCode)
	, Exception(message)
{
}
