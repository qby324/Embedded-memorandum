// Memo_QBY.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CMemo_QBYApp:
// �йش����ʵ�֣������ Memo_QBY.cpp
//

class CMemo_QBYApp : public CWinApp
{
public:
	CMemo_QBYApp();
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMemo_QBYApp theApp;
