#include <tchar.h>
#include <ole2.h>
#include <atlbase.h>
#include <atlsafe.h>

int _tmain()
{
	const int msoShapeHeart = 21;
	const int msoShapeSmileyFace = 17;

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

		// Shapes
		CComPtr<IDispatch> pXlShapes;
		{
			CComVariant v;
			pXlSheet.GetPropertyByName(L"Shapes", &v);
			pXlShapes = v.pdispVal;
		}

		//pXlShapes.InvokeN(L"AddShape", 0, 5);

		pXlBook.Invoke0(L"Close");
		pXlApp.Invoke0(L"Quit");
	}

	CoUninitialize();
	return 0;
}
