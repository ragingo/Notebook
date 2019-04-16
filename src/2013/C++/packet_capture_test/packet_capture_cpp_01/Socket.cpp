#include "StdAfx.h"
#include "Socket.h"
#include "InvalidSocketException.h"

Socket::Socket(int af, int type, int protocol)
	: m_AddressFamily(af),
		m_Type(type),
		m_Protocol(protocol),
		m_IsNonBlockingMode(false),
		m_IsPromiscuousMode(false),
		m_ReceiveBufferSize(RECEIVE_BUF_SIZE)
{
	m_Sock = socket(af, type, protocol);
	if (m_Sock == INVALID_SOCKET)
	{
		throw InvalidSocketException(INVALID_SOCKET, "�\�P�b�g�̍쐬�Ɏ��s���܂����B");
	}
}

Socket::~Socket()
{
	closesocket(m_Sock);
}

const SOCKET& Socket::GetHandle() const
{
	return m_Sock;
}

void Socket::Bind(sockaddr_in* addr, int len)
{
	int ret = bind(m_Sock, (sockaddr*)addr, len);
	if (ret == SOCKET_ERROR)
	{
		throw InvalidSocketException(SOCKET_ERROR, "�o�C���h�Ɏ��s���܂����B");
	}
}

void Socket::SetNonBlockingMode(bool flag)
{
	m_IsNonBlockingMode = flag;

	u_long mode = flag ? 1L : 0L;
	int ret = ioctlsocket(m_Sock, FIONBIO, &mode);
	if (ret != 0)
	{
		throw Exception("�m���u���b�L���O���[�h�̐ݒ�Ɏ��s���܂����B");
	}
}

void Socket::SetPromiscuousMode(bool flag)
{
	m_IsPromiscuousMode = flag;

	u_long mode = flag ? 1L : 0L;
	BYTE in_buf[] = {1,0,0,0};
	BYTE out_buf[] = {0,0,0,0};
	DWORD bytes_transferred = 0;
	int ret = WSAIoctl(m_Sock, SIO_RCVALL, in_buf, sizeof(in_buf), out_buf, sizeof(out_buf), &bytes_transferred, NULL, NULL);
	if (ret != 0)
	{
		throw Exception("�v���~�X�L���X���[�h�̐ݒ�Ɏ��s���܂����B");
	}
}

const int& Socket::GetReceiveBufferSize() const
{
	return m_ReceiveBufferSize;
}

void Socket::SetReceiveBufferSize(int size)
{
	m_ReceiveBufferSize = size;
}

int Socket::Receive(IReceiveCallback& callback)
{
	char* buf = new char[m_ReceiveBufferSize];
	SecureZeroMemory(buf, m_ReceiveBufferSize);

	int len = recv(m_Sock, buf, m_ReceiveBufferSize, 0);
	boost::shared_array<BYTE> data(reinterpret_cast<BYTE*>(buf));

	if (len == SOCKET_ERROR)
	{
		if (!m_IsNonBlockingMode)
		{
			throw InvalidSocketException(len, "��M�Ɏ��s���܂����B");
		}
		else if (WSAEWOULDBLOCK != WSAGetLastError())
		{
			throw InvalidSocketException(len, "��M�Ɏ��s���܂����B");
		}
	}

	if (len > 0)
	{
		callback.Invoke(data, len);
	}

	return len;
}
