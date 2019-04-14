
#ifndef __FILE_H__
#define __FILE_H__

#pragma once
#include <Windows.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlfile.h>

namespace Sample
{
using namespace ATL;

class File : public CAtlFile
{
public:
	File() throw(): CAtlFile()
	{
	}
	explicit File(CAtlTransactionManager* pTM) throw() : CAtlFile(pTM)
	{
	}
	File(File& file) throw() : CAtlFile(file)
	{
	}
	explicit File(HANDLE hFile) throw() : CAtlFile(hFile)
	{
	}

	HRESULT Move(LPCTSTR lpOldFileName, LPCTSTR lpNewFileName) const
	{
		BOOL b = 
			m_pTM == NULL ? 
			::MoveFileEx(lpOldFileName, lpNewFileName, MOVEFILE_COPY_ALLOWED) : 
			m_pTM->MoveFile(lpOldFileName, lpNewFileName);
		if(!b)
		{
			return AtlHresultFromLastError();
		}
		return S_OK;
	}

	HRESULT Delete(LPCTSTR lpFileName) const
	{
		BOOL b = 
			m_pTM == NULL ? 
			::DeleteFile(lpFileName) :
			m_pTM->DeleteFile(lpFileName);
		if(!b)
		{
			return AtlHresultFromLastError();
		}
		return S_OK;
	}
};

}

#endif
