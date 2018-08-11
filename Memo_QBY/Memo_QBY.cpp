// Memo_QBY.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Memo_QBY.h"
#include "Memo_QBYDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemo_QBYApp

BEGIN_MESSAGE_MAP(CMemo_QBYApp, CWinApp)
END_MESSAGE_MAP()


// CMemo_QBYApp 构造
CMemo_QBYApp::CMemo_QBYApp()
	: CWinApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMemo_QBYApp 对象
CMemo_QBYApp theApp;

// CMemo_QBYApp 初始化

BOOL CMemo_QBYApp::InitInstance()
{

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CMemo_QBYDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
