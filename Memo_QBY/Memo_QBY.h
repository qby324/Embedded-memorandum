// Memo_QBY.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CMemo_QBYApp:
// 有关此类的实现，请参阅 Memo_QBY.cpp
//

class CMemo_QBYApp : public CWinApp
{
public:
	CMemo_QBYApp();
	
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMemo_QBYApp theApp;
