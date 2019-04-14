
#ifndef __TRANSACTIONMANAGER_H__
#define __TRANSACTIONMANAGER_H__

#pragma once
#include <Windows.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlfile.h>

namespace Sample
{
using namespace ATL;

class TransactionManager : public CAtlTransactionManager
{
protected:
	void SetHandle(HANDLE hTransaction)
	{
		ATLASSERT(m_hTransaction == NULL);
		m_hTransaction = hTransaction;
	}
public:
	TransactionManager(BOOL bFallback = TRUE, BOOL bAutoCreateTransaction = TRUE) : 
	  CAtlTransactionManager(bFallback, bAutoCreateTransaction)
	{
	}

	static PVOID LoadFunction(LPCTSTR lpModuleName, LPCSTR lpFunctionName)
	{
		HMODULE hModule = AtlLoadSystemLibraryUsingFullPath(lpModuleName);
		ATLASSERT(hModule != NULL);
		PVOID pFunc = GetProcAddress(hModule, lpFunctionName);
		ATLASSERT(pFunc != NULL);
		return pFunc;
	}

	BOOL GetId(LPGUID lpGuid) const
	{
		if (m_hTransaction == NULL)
		{
			ATLASSERT(FALSE);
			return FALSE;
		}

		typedef BOOL (WINAPI *PFNGETTRANSACTIONID)(HANDLE, LPGUID);
		static PFNGETTRANSACTIONID pfnGetTransactionId = 
			static_cast<PFNGETTRANSACTIONID>(LoadFunction(_T("ktmw32.dll"), "GetTransactionId"));

		return (*pfnGetTransactionId)(m_hTransaction, lpGuid);
	}

	static BOOL Open(TransactionManager *pTransactionManager, DWORD dwDesiredAccess, LPGUID TransactionId)
	{
		if(pTransactionManager->GetHandle() != NULL)
		{
			ATLASSERT(FALSE);
			return FALSE;
		}

		typedef HANDLE (WINAPI *PFNOPENTRANSACTION)(DWORD, LPGUID);
		static PFNOPENTRANSACTION pfnOpenTransaction = 
			static_cast<PFNOPENTRANSACTION>(LoadFunction(_T("ktmw32.dll"), "OpenTransaction"));

		HANDLE hTx = (*pfnOpenTransaction)(dwDesiredAccess, TransactionId);
		ATLASSERT(hTx != INVALID_HANDLE_VALUE);
		pTransactionManager->SetHandle(hTx);
		return pTransactionManager->GetHandle() != NULL;
	}
};

}

#endif
