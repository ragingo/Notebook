// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B                   
#define _WIN32_WINNT 0x0501	// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif						


#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <windows.h>
#include <crtdbg.h>


/* _com_util::ConvertBSTRToString(BSTR)�̎g�p�ɕK�v */
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