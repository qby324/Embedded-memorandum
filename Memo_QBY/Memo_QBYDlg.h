// Memo_QBYDlg.h : ͷ�ļ�
//

#pragma once
#include "afxdtctl.h"
#include "afxtempl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <map> 
#define MAX 200


class Memo{
public:
	const static int LEVEL_NAL = 0;			//��������
	const static int LEVEL_IMP = 1;
	const static int TYPE_ONE = 0;			//ʱ������
	const static int TYPE_EVR = 1;
	const static int TYPE_SUN = 2;
	const static int TYPE_MON = 3;
	const static int TYPE_TUE = 4;
	const static int TYPE_WED = 5;
	const static int TYPE_THU = 6;
	const static int TYPE_FRI = 7;
	const static int TYPE_SAT = 8;

	CTime m_cTime;							//����ʱ��
	int m_iLevel;							//��Ҫ������
	int m_iType;							//���Ѵ�������
	BOOL m_bAlarm;							//�Ƿ���������
	CString m_strEdit;						//��������

	BOOL m_bSettimer;						//���趨ʱ
	BOOL m_bChange;							//��Ϊ�޸ı�ʶ��
};





// CMemo_QBYDlg �Ի���
class CMemo_QBYDlg : public CDialog
{
// ����
public:
	CMemo_QBYDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MEMO_QBY_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEdit();
	afx_msg void OnEnSetfocusEdit();
	afx_msg void OnBnClickedButtonAdd();
	CDateTimeCtrl m_SetDate;
	CArray<Memo> m_Arr_Memo;
	std::map<UINT_PTR,int> m_timers;
	CComboBox m_ChooseDays;
	CEdit m_Edit;
	CDateTimeCtrl m_DateFrom;
	CDateTimeCtrl m_DateTo;
	CListCtrl m_MemoList;
	CComboBox m_Quick;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void read(void);
	void save(void);
	void ListShow(int i, Memo& node);
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCleanmemo();
	afx_msg void OnBnClickedButtonCleanedit();
	afx_msg void OnBnClickedButtonDeleteQuick();
	afx_msg void OnBnClickedButtonAddtoQuick();
	afx_msg void OnClose();
};
