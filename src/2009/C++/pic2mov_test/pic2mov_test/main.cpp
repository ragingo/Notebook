
#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <windows.h>
#pragma comment(lib, "vfw32.lib")
#include <vfw.h>

#pragma comment(lib, "gdiplus.lib")
#include <gdiplus.h>

// 拡張子の変更
void ChangeFileExtension(std::wstring& filename, const std::wstring& ext)
{
	int pos = filename.rfind(L".");
	int len = filename.length();
	filename = filename.replace(pos, len, ext);
}

// http://msdn.microsoft.com/en-us/library/ms533843(VS.85).aspx
int GetEncoderClsid(const wchar_t* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT num = 0;
	UINT size = 0;
	
	ImageCodecInfo* pImageCodecInfo = NULL;
	
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;
	
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;
	
	GetImageEncoders(num, size, pImageCodecInfo);
	
	for(UINT j = 0; j < num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}

// http://msdn.microsoft.com/en-us/library/ms533837(VS.85).aspx
void image2bmp(std::wstring filename)
{
	using namespace Gdiplus;
	GdiplusStartupInput input;
	ULONG_PTR           token;
	GdiplusStartup(&token, &input, NULL);
	
	CLSID  encoderClsid;
	Image* image = new Image(filename.c_str()); // image
	GetEncoderClsid(L"image/bmp", &encoderClsid);

	std::wcout << filename << L" 変換中..." <<  std::endl;
	ChangeFileExtension(filename, L".bmp");

	Status stat = image->Save(filename.c_str(), &encoderClsid, NULL);
	
	delete image;
	GdiplusShutdown(token);

}

// 画面出力
template<typename T>
class Print
{
public:
	void operator()(T value)
	{
		std::wcout << value << std::endl;
	}
};

// ファイル列挙
void GetFiles(const wchar_t* dir, const wchar_t* wildcard, std::vector<std::wstring>& files)
{
	HANDLE          hFind;
	WIN32_FIND_DATA find_data;
	std::wstring    path;
	path.append(dir).append(wildcard);

	hFind = FindFirstFile(path.c_str(), &find_data);
	path.clear();
	path.append(dir);
	do
	{
		if(hFind != INVALID_HANDLE_VALUE)
		{
			files.push_back(path + find_data.cFileName);
		}
	} while(FindNextFile(hFind, &find_data));
	FindClose(hFind);
}

// 対象フォルダの中のJPEGを一括でBMPに変換してから、それを基にAVIを作成。
int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	using namespace std;

	const wchar_t *DIR = L"C:\\Windows\\Web\\Wallpaper\\";
	wcout.imbue(locale("japanese"));
	vector<wstring> files;

	// 変換前
	GetFiles(DIR, L"*.jpg", files);
	for_each(files.begin(), files.end(), image2bmp);

	// 変換後
	files.clear();
	GetFiles(DIR, L"*.bmp", files);

	// 対象ファイル
	for_each(files.begin(), files.end(), Print<wstring>());
	wcout << L"ファイル数 : " << files.size() << endl;

	// AVIファイル作成 ------------------------------------------------------------------
	IAVIFile   *pAviFile   = NULL;
	IAVIStream *pAviStream = NULL;

	AVISTREAMINFO asi = {0};
	asi.fccHandler = mmioFOURCC('M','S','V','C');
	asi.fccType    = streamtypeVIDEO;
	asi.dwScale    = 1;
	asi.dwRate     = 1;
	asi.dwQuality  = -1; // default
	asi.dwLength   = files.size();

	AVIFileInit();
	if(AVIFileOpen(&pAviFile, L"d:\\test.avi", OF_CREATE | OF_WRITE | OF_SHARE_DENY_NONE, NULL) != 0)
	{
		wcout << L"AVIFileOpen error" << endl;
		return -1;
	}

	if(AVIFileCreateStream(pAviFile, &pAviStream, &asi) != 0)
	{
		wcout << L"AVIFileCreateStream error" << endl;
		return -1;
	}

	BITMAPINFOHEADER bih = {0};
	//bih.biBitCount = 32;
	//bih.biHeight   = 600;
	//bih.biPlanes   = 1;
	//bih.biSize     = sizeof(BITMAPINFOHEADER);
	//bih.biWidth    = 800;
	{
		char *p = new char[sizeof(BITMAPINFOHEADER)];
		ifstream ifs(files[0].c_str(), ios::binary);
		ifs.seekg(sizeof(BITMAPFILEHEADER));
		ifs.read(p, sizeof(BITMAPINFOHEADER));
		bih = *(BITMAPINFOHEADER*)p;
		delete[] p;
	}

	if(AVIStreamSetFormat(pAviStream, 0, &bih, sizeof(BITMAPINFOHEADER)) != 0)
	{
		wcout << L"AVIStreamSetFormat error" << endl;
		return -1;
	}

	const int SEEK_SIZE = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	for(int i = 0; i < (int)files.size(); i++)
	{
		const long filesize = ifstream(files[i].c_str()).seekg(0, ios::end).tellg();
		const long readsize = filesize - SEEK_SIZE;
		char *buf = new char[readsize];

		ifstream ifs(files[i].c_str(), ios::binary);
		ifs.seekg(SEEK_SIZE);
		ifs.read(buf, readsize);

		if(AVIStreamWrite(pAviStream, i, 1, buf, readsize, AVIIF_KEYFRAME, NULL, NULL) != 0)
		{
			wcout << L"失敗..." << endl;
		}
		wcout << (i+1) << L"枚目..." << endl;

		delete[] buf;
	}

	AVIStreamRelease(pAviStream);
	AVIFileRelease(pAviFile);
	AVIFileExit();

	wcout << L"完了！" << endl;

	return 0;
}