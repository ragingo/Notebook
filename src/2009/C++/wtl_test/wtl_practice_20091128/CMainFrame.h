
#include "header.h"

// メイン画面
class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>,
	public CMessageFilter,
	public CIdleHandler,
	public CThemeImpl<CMainFrame>
{
private:
	enum
	{
		ID_BTN_OK,
	};
	CButton m_BtnOk;

public:
	DECLARE_FRAME_WND_CLASS(TEXT("CMainFrame"), IDR_MENU1)
	BEGIN_MSG_MAP(CMainFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		//MSG_WM_PAINT(OnPaint)
		//CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, OnFileExit)
		COMMAND_ID_HANDLER_EX(ID_BTN_OK, OnOkClick)
	END_MSG_MAP()

	CMainFrame()
	{
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return 0;
	}

	LRESULT OnCreate(LPCREATESTRUCT)
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		InitControls();

		return 0;
	}

	void OnDestroy()
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
		PostQuitMessage(0);
	}

	//void OnPaint(HDC)
	//{
	//	//CPaintDC dc(m_hWnd);
	//	//MARGINS margins = { -1, -1, -1, -1 };
	//	//BP_PAINTPARAMS params = { sizeof(BP_PAINTPARAMS), BPPF_ERASE, NULL, NULL };
	//	//HDC hDCPaint = NULL;

	//	//DwmExtendFrameIntoClientArea(m_hWnd, &margins);
	//	//OpenThemeData(L"ControlPanelStyle");

	//	//CBufferedPaint bp;
	//	//bp.Begin(dc.m_hDC, &dc.m_ps.rcPaint, BPBF_TOPDOWNDIB, &params, &hDCPaint);
	//	//// 描画コード...
	//	//bp.End();
	//	//CloseThemeData();
	//}

	void OnFileOpen(UINT, int, HWND)
	{
		CFileDialog dlg(
			TRUE,
			TEXT("avi"),
			NULL,
			OFN_OVERWRITEPROMPT,
			TEXT("avi\0*.avi\0mp4\0*.mp4"));
		
		if(dlg.DoModal() == IDOK)
		{
			CString msg;
			msg += dlg.m_szFileTitle;
			msg += TEXT(", ");
			msg += dlg.m_szFileName;
			::MessageBox(0, msg, 0, 0);
		}
	}

	void OnFileExit(UINT, int, HWND)
	{
		PostMessage(WM_CLOSE);
	}

	void OnOkClick(UINT, int, HWND)
	{
		::MessageBox(0,0,0,0);
	}

	void InitControls()
	{
		RECT r;

		r.left = 10, r.top = 10, r.right = 100, r.bottom = 100;
		m_BtnOk.Create(this->m_hWnd, r, TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, (HMENU)ID_BTN_OK);
	}
};
