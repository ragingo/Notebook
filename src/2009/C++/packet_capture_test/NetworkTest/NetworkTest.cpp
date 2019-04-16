// NetworkTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "header.h"

using namespace std;

#ifdef UNICODE
	#define tcout wcout
#else
	#define tcout cout
#endif

typedef void (*MethodInvoker)();

void CleanUp(SOCKET s)
{
	closesocket(s);
	if(WSACleanup() != 0)
	{
		tcout << "cleanup error!" << endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA data = {0};
	if(WSAStartup(MAKEWORD(2,0), &data) != 0)
	{
		tcout << "startup error!" << endl;
		return 1;
	}

	char hostname[255];
	gethostname(hostname, 255);
	hostent *phostent = gethostbyname(hostname);
	cout << "Host Name   : " << hostname << endl;
	cout << "h_addrtype  : " << phostent->h_addrtype << endl;
	cout << "h_length    : " << phostent->h_length   << endl;
	cout << "h_name      : " << phostent->h_name     << endl;
	cout << "h_addr_list : " << phostent->h_addr_list[0] << endl;;

	tcout << "szDescription  : " << data.szDescription  << endl;
	tcout << "szSystemStatus : " << data.szSystemStatus << endl;
	tcout << "wHighVersion   : " << data.wHighVersion   << endl;
	tcout << "wVersion       : " << data.wVersion       << endl;

	SOCKET sock;
	if((sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == INVALID_SOCKET)
	{
		tcout << "invalid socket" << endl;
	}

	char lpvOutBuffer[1024];
	DWORD lpcvBytesReturned;
	if(WSAIoctl(sock, SIO_ADDRESS_LIST_QUERY, NULL, 0, lpvOutBuffer, 1024, &lpcvBytesReturned, NULL, NULL) != 0)
	{
		tcout << "wsaioctl error : " << WSAGetLastError() << endl;
	}

	SOCKET_ADDRESS_LIST *pAddrList = (SOCKET_ADDRESS_LIST*)lpvOutBuffer;
	for(int i=0; i < pAddrList->iAddressCount; i++)
	{
		tcout << "iSockaddrLength       : " << pAddrList->Address[i].iSockaddrLength << endl;
		tcout << "lpSockaddr->sa_data   : ";
		for(int ch = 0; ch < sizeof(pAddrList->Address[i].lpSockaddr->sa_data); ch++)
		{
			tcout << pAddrList->Address[i].lpSockaddr->sa_data[ch];
		}
		tcout << endl;
		tcout << "lpSockaddr->sa_family : " << pAddrList->Address[i].lpSockaddr->sa_family << endl;
	}

	SOCKADDR_IN addr_in = {0};
	addr_in.sin_addr.s_addr = ((SOCKADDR_IN*)pAddrList->Address[0].lpSockaddr)->sin_addr.s_addr;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(0);

	if(bind(sock, (SOCKADDR*)&addr_in, sizeof(addr_in)) == SOCKET_ERROR)
	{
		tcout << "bind error" << endl;
		CleanUp(sock);
		return 1;
	}

	// 非ブロッキングモード
	DWORD argp;
	if(ioctlsocket(sock, FIONBIO, &argp) != 0)
	{
		tcout << "non bloking setting error" << endl;
		CleanUp(sock);
		return 1;
	}

	// プロミスキャスモード
#define SIO_RCVALL   _WSAIOW(IOC_VENDOR,1)
	DWORD optval = 1;
	if(ioctlsocket(sock, SIO_RCVALL, &optval) != 0)
	{
		tcout << "promisecuous mode setting error" << endl;
		CleanUp(sock);
		return 1;
	}

	while(!(GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		Sleep(100);

		const DWORD MAX_SIZE = 65535;
		char buff[MAX_SIZE] = {0};
		DWORD receiveSize = 0;
		DWORD flags = 0;
		WSABUF wsaBuf = {0};
		wsaBuf.buf = buff;
		wsaBuf.len = MAX_SIZE;

		if(WSARecv(sock, &wsaBuf, 1, &receiveSize, &flags, NULL, NULL) == SOCKET_ERROR)
		{
			tcout << WSAGetLastError() << endl;
			continue;
		}

		IPHeader *pheader = (IPHeader*)wsaBuf.buf;
		in_addr src, dest;
		src.s_addr = pheader->source_address;
		dest.s_addr = pheader->destination_address;

		tcout << "protocol:" << pheader->protocol << endl;;
		tcout << inet_ntoa(src) << " ---> " << inet_ntoa(dest) << endl;
		for(DWORD i=0; i < receiveSize; i++)
		{
			cout << pheader->option[i];
		}
		cout << endl;
	}

	CleanUp(sock);
	return 0;
}
