// Memo_QBY.cpp : ����Ӧ�ó��������Ϊ��
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


// CMemo_QBYApp ����
CMemo_QBYApp::CMemo_QBYApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMemo_QBYApp ����
CMemo_QBYApp theApp;

// CMemo_QBYApp ��ʼ��

BOOL CMemo_QBYApp::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMemo_QBYDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
