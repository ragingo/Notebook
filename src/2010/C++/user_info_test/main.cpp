
#include <stdio.h>
#include <windows.h>
#include <lm.h>
#pragma comment(lib, "Netapi32.lib")
#include <iostream>

using namespace std;

int main()
{
   LPUSER_INFO_11 info = NULL;
   NET_API_STATUS nStatus = NetUserGetInfo(L"localhost", L"user1", 10, (LPBYTE *)&info);

   if (nStatus == NERR_Success)
   {
	   if (info != NULL)
	   {
		   wcout << info->usri11_name << endl;
		   wcout << info->usri11_password_age << endl;
	   }
   }
   else
   {
	   fprintf(stderr, "A system error has occurred: %d\n", nStatus);
   }

   if (info != NULL)
   {
	   NetApiBufferFree(info);
   }
   return 0;
}

