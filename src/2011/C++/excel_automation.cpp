
// http://support.microsoft.com/kb/216686/ja

#pragma once
#include <tchar.h>
#include <ole2.h>
#include <atlbase.h>
#include <atlsafe.h>

static void excel_test()
{
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
		// Workbooks
		CComPtr<IDispatch> pXlBooks;
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
		CComPtr<IDispatch> pXlSheet;
		{
			CComVariant v;
			pXlApp.GetPropertyByName(L"ActiveSheet", &v);
			pXlSheet = v.pdispVal;
		}
		// Range
		CComPtr<IDispatch> pXlRange;
		{
			CComVariant v(L"A1:O15");
			CComVariant result;
			DISPID      dispid = NULL;
			DISPPARAMS  dp = { &v, NULL, 1, 0 };
			pXlSheet.GetIDOfName(L"Range", &dispid);
			pXlSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
				DISPATCH_PROPERTYGET | DISPATCH_METHOD, &dp, &result, NULL, NULL);
			pXlRange = result.pdispVal;
		}
		// ÉfÅ[É^ìäì¸
		{
			CComSafeArray<int> *pArray;
			CComSafeArrayBound sab[2];
			sab[0].SetLowerBound(1L); sab[0].SetCount(15);
			sab[1].SetLowerBound(1L); sab[1].SetCount(15);

			pArray = new CComSafeArray<int>(sab, 2);
			for(int i = 0; i <= 15; i++)
			{
				for(int j = 1; j <= 15; j++)
				{
					long alIndexes[] = { i, j };
					pArray->MultiDimSetAt(alIndexes, i * j);
				}
			}

			CComVariant v(*pArray->GetSafeArrayPtr());
			pXlRange.PutPropertyByName(L"Value", &v);
			delete pArray;
		}
		
		::MessageBox(NULL, L"All done.", L"Notice", 0);
		
		pXlApp.Invoke0(L"Quit");
	}	
	CoUninitialize();
}