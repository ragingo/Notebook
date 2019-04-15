
#define SECURITY_WIN32

#pragma once
#include <windows.h>
#include <Security.h>
#include <Sspi.h>
#pragma comment (lib, "secur32.lib")

static void TestNtlm()
{
	CredHandle hCred;
	TimeStamp tsExpiry;
	SECURITY_STATUS status;

	status = AcquireCredentialsHandle(
		NULL, 
		NTLMSP_NAME, 
		SECPKG_CRED_BOTH, 
		NULL, 
		NULL, 
		NULL, 
		NULL, 
		&hCred, 
		&tsExpiry);

	LPTSTR lpszName = NULL;
	QueryCredentialsAttributes(&hCred, SECPKG_CRED_ATTR_NAMES, &lpszName);

	MessageBox(0, lpszName, 0, 0);

	FreeContextBuffer(lpszName);
	FreeCredentialsHandle(&hCred);
}