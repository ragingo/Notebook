#pragma once

class Exception : public std::exception
{
private:
	const std::string m_Message;

public:
	Exception();

	Exception(const std::string& message);

	virtual ~Exception();

	virtual const char* what() const;

	virtual const std::string& GetMessage() const;
};
