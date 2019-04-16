#include "StdAfx.h"
#include "Exception.h"

Exception::Exception()
{
}

Exception::Exception(const std::string& message)
	: m_Message(message)
{
}

Exception::~Exception()
{
}

const char* Exception::what() const
{
	return m_Message.c_str();
}

const std::string& Exception::GetMessage() const
{
	return m_Message;
}

