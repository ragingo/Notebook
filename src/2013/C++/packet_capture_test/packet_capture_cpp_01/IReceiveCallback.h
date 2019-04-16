#pragma once

class IReceiveCallback
{
public:
	virtual void Invoke(const boost::shared_array<BYTE>& data, const int& size) = 0;
};
