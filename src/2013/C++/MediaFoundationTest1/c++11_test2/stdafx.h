// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <conio.h>

#include <cstdlib>
#include <iostream>

#include <Windows.h>
#include <ShObjIdl.h>
#include <Shlwapi.h>

#include <mfapi.h>
#include <mfidl.h>
#include <evr.h>
#include <mfreadwrite.h>
//#include <mfplay.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib,"Mfplat.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"Mfuuid.lib")
#pragma comment(lib,"Strmiids.lib")

#include <atlbase.h>
#include <atlconv.h>
#include <atlcom.h>