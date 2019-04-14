// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						


#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <windows.h>
#include <crtdbg.h>


/* _com_util::ConvertBSTRToString(BSTR)の使用に必要 */
#pragma comment(lib, "comsupp")
#pragma comment(lib, "comsuppw")
#include <comutil.h>

#pragma comment(lib, "strmiids")
#pragma comment(lib, "quartz")
#include <dshow.h>
#include <qedit.h>


#include "DirectShow.h"

#ifdef _DEBUG
void* operator new(size_t size, const char *fileName, int lineNumber)
{
   return _malloc_dbg(size, _NORMAL_BLOCK, fileName, lineNumber);
}
void operator delete(void *p, const char *fileName, int lineNumber)
{
   _free_dbg(p, _NORMAL_BLOCK);
}
#endif