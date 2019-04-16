
#pragma once

#include "targetver.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <locale>
#include <map>
#include <memory>
#include <tchar.h>
#include <vector>

#ifdef _DEBUG
#include <crtdbg.h>
#endif


#include <WinSock2.h>
#include <IPHlpApi.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <MSTcpIP.h>
#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "Iphlpapi.lib")

#include <Windows.h>

#include <Ole2.h>
#include <atlbase.h>
#include <atlsafe.h>
#include <atlstr.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
