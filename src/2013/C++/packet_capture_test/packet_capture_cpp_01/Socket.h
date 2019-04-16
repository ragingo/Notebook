#pragma once
#include "IReceiveCallback.h"

class Socket
{
private:
	SOCKET m_Sock;
	int m_AddressFamily;
	int m_Type;
	int m_Protocol;
	bool m_IsNonBlockingMode;
	bool m_IsPromiscuousMode;
	int m_ReceiveBufferSize;

public:

	static const int RECEIVE_BUF_SIZE = 4096;

	Socket(int af, int type, int protocol);

	~Socket();

	const SOCKET& GetHandle() const;

	void Bind(sockaddr_in* addr, int len);

	void SetNonBlockingMode(bool flag);

	void SetPromiscuousMode(bool flag);

	const int& GetReceiveBufferSize() const;

	void SetReceiveBufferSize(int size);

	int Receive(IReceiveCallback& callback);

};
