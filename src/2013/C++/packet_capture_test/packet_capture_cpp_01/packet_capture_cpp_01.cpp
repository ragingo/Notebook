
#include "stdafx.h"
#include "netdef.h"
#include "String.h"
#include "IReceiveCallback.h"
#include "Socket.h"
#include "NetworkInterface.h"
#include "InvalidSocketException.h"

struct NetworkInterfaceInfo
{
	NetworkInterfacePtr AdapterPtr;
	std::vector<PIP_ADAPTER_UNICAST_ADDRESS> Addresses;
};

std::vector<NetworkInterfaceInfo> LoadNetworkInterfaces()
{
	std::vector<NetworkInterfaceInfo> adapters;
	
	BOOST_FOREACH(auto& nic, NetworkInterface::GetAllNetworkInterfaces())
	{
		auto adapter = nic->GetAdapterInfo();

		switch (adapter->IfType)
		{
		// 有線
		case IF_TYPE_ETHERNET_CSMACD:
		// 無線
		case IF_TYPE_IEEE80211:
			NetworkInterfaceInfo info;
			info.AdapterPtr = nic;

			auto addr = adapter->FirstUnicastAddress;

			while (addr)
			{	info.Addresses.push_back(addr);
				addr = addr->Next;
			}

			adapters.push_back(info);
			break;
		}
	}

	return adapters;
}

struct SelectedNetworkInterfaceInfo
{
	NetworkInterfacePtr AdapterPtr;
	PIP_ADAPTER_UNICAST_ADDRESS Addr;
};

SelectedNetworkInterfaceInfo SelectNetworkInterface()
{
	int selectedAddrNo = -1;
	int addrNo = 0;
	std::map<int, PIP_ADAPTER_UNICAST_ADDRESS> addr_table;
	std::map<int, NetworkInterfacePtr> adapter_table;

	BOOST_FOREACH(auto& nic, LoadNetworkInterfaces())
	{
		std::wcout
			<< boost::wformat(L"%s (%s)")
				% nic.AdapterPtr->GetAdapterInfo()->FriendlyName
				% nic.AdapterPtr->GetAdapterInfo()->Description
			<< std::endl;

		BOOST_FOREACH(auto addr, nic.Addresses)
		{
			std::wcout
				<< boost::wformat(L"\t%d. %s")
					% (++addrNo)
					% inet_ntoa(((sockaddr_in*)addr->Address.lpSockaddr)->sin_addr)
				<< std::endl;
			addr_table[addrNo] = addr;
			adapter_table[addrNo] = nic.AdapterPtr;
		}

	}

	std::wcout << L"どれを使用しますか？" << std::endl;

	bool isValidInput = false;
	const int INPUT_LIMIT = 2;

	while (!isValidInput)
	{
		char input_buf[INPUT_LIMIT+1] = {0};
		std::cin >> std::setw(INPUT_LIMIT) >> input_buf;

		try
		{
			selectedAddrNo = boost::lexical_cast<int>(input_buf);

			auto it = addr_table.find(selectedAddrNo);
			if (it != addr_table.end())
			{
				isValidInput = true;
			}
		}
		catch (const boost::bad_lexical_cast&)
		{
		}

		if (!isValidInput)
		{
			std::wcout << L"もう一度入力して下さい。" << std::endl;
			std::cin.clear();
			std::cin.ignore(INPUT_LIMIT+1, '\n');
		}
	}

	std::wcout
		<< boost::wformat(L"%d. ですね。")
			% selectedAddrNo;

	std::wcout
		<< boost::wformat(L"キャプチャを開始します。")
		<< std::endl;

	SelectedNetworkInterfaceInfo info;
	info.AdapterPtr = adapter_table[selectedAddrNo];
	info.Addr = addr_table[selectedAddrNo];

	return info;
}

class ReceiveCallback : public IReceiveCallback
{
public:
	void Invoke(const boost::shared_array<BYTE>& data, const int& size)
	{
		auto ipv4 = reinterpret_cast<IPv4Header*>(data.get());
		printf("---------------------------\n");
		printf("ver   : %d\n", ipv4->Version);
		printf("len   : %d\n", ipv4->HeaderLength);
		printf("proto : %d\n", ipv4->Protocol);
	}
};

int _tmain0()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	_tsetlocale(LC_ALL, _T(""));

	WSAData wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	auto selectedNic = SelectNetworkInterface();
	auto adapter = selectedNic.AdapterPtr->GetAdapterInfo();
	auto addr = selectedNic.Addr->Address;

	Socket sock(addr.lpSockaddr->sa_family, SOCK_RAW, IPPROTO_IP);
	sock.Bind((sockaddr_in*)addr.lpSockaddr, addr.iSockaddrLength);
	sock.SetNonBlockingMode(true);
	sock.SetPromiscuousMode(true);

	ReceiveCallback callback;

	while (!GetAsyncKeyState(VK_SPACE))
	{
		try
		{
			int len = sock.Receive(callback);
		}
		catch(const InvalidSocketException& e)
		{
			std::cout << e.GetMessage() << std::endl;
			break;
		}

		Sleep(10);
	}

	WSACleanup();

	return 0;
}

namespace Excel {

class Application
{
private:
	CComPtr<IDispatch> m_App;
public:
	Application()
	{
		m_App.CoCreateInstance(L"Excel.Application", NULL, CLSCTX_LOCAL_SERVER);
	}
	~Application()
	{
		m_App.Invoke0(L"Quit");
	}

	bool GetVisible()
	{
		CComVariant v;
		m_App.GetPropertyByName(L"Visible", &v);
		return static_cast<bool>(v.boolVal);
	}

	void SetVisible(bool visible)
	{
		CComVariant v(visible);
		m_App.PutPropertyByName(L"Visible", &v);
	}

	bool GetDisplayAlerts()
	{
		CComVariant v;
		m_App.GetPropertyByName(L"DisplayAlerts", &v);
		return static_cast<bool>(v.boolVal);
	}

	void SetDisplayAlerts(bool displayAlerts)
	{
		CComVariant v(displayAlerts);
		m_App.PutPropertyByName(L"DisplayAlerts", &v);
	}

	CComPtr<IDispatch> GetWorkbooks()
	{
		CComVariant v;
		m_App.GetPropertyByName(L"Workbooks", &v);
		return v.pdispVal;
	}

	CComPtr<IDispatch> GetActiveSheet()
	{
		CComVariant v;
		m_App.GetPropertyByName(L"ActiveSheet", &v);
		return v.pdispVal;
	}
};

}

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CoInitialize(NULL);
	{
		// Application
		CComPtr<IDispatch> pXlApp;
		pXlApp.CoCreateInstance(L"Excel.Application", NULL, CLSCTX_LOCAL_SERVER);
		{
			CComVariant v(true);
			pXlApp.PutPropertyByName(L"Visible", &v);

			v = false;
			pXlApp.PutPropertyByName(L"DisplayAlerts", &v);
		}

		//Excel::Application pXlApp;
		//pXlApp.SetDisplayAlerts(false);
		//pXlApp.SetVisible(true);

		// Workbooks
		CComPtr<IDispatch> pXlBooks;// = pXlApp.GetWorkbooks();
		{
			CComVariant v;
			pXlApp.GetPropertyByName(L"Workbooks", &v);
			pXlBooks = v.pdispVal;
		}
		
		// Workbook
		CComPtr<IDispatch> pXlBook;
		{
			CComVariant v;
			pXlBooks.GetPropertyByName(L"Add", &v);
			pXlBook = v.pdispVal;
		}

		// Worksheet
		CComPtr<IDispatch> pXlSheet;// = pXlApp.GetActiveSheet();
		{
			CComVariant v;
			pXlApp.GetPropertyByName(L"ActiveSheet", &v);
			pXlSheet = v.pdispVal;
		}

		int rowIndex = 0;

		while (true)
		{


			// Range
			CComPtr<IDispatch> pXlRange;
			{
				LPTSTR cellAddr = new TCHAR[64];
				_stprintf_s(cellAddr, sizeof(TCHAR)*64, _T("A%d:O%d"), rowIndex, rowIndex);

				CComVariant v(cellAddr);
				CComVariant result;
				DISPID      dispid = NULL;
				DISPPARAMS  dp = { &v, NULL, 1, 0 };
				pXlSheet.GetIDOfName(L"Range", &dispid);
				pXlSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
					DISPATCH_PROPERTYGET | DISPATCH_METHOD, &dp, &result, NULL, NULL);
				pXlRange = result.pdispVal;
				delete[] cellAddr;
			}


		}
		
		
		pXlApp.Invoke0(L"Quit");
	}
	
	CoUninitialize();

	return 0;
}

