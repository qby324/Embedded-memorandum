// Memo_QBYDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Memo_QBY.h"
#include "Memo_QBYDlg.h"
#include <fstream>
#include <sstream>
#include <string.h>
#include <sipapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMemo_QBYDlg �Ի���

CMemo_QBYDlg::CMemo_QBYDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemo_QBYDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemo_QBYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_SetDate);
	DDX_Control(pDX, IDC_COMBO_CHOOSEDAY, m_ChooseDays);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, DC_DATETIMEPICKER_STARTDATE, m_DateFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ENDDATE, m_DateTo);
	DDX_Control(pDX, IDC_LIST, m_MemoList);
	DDX_Control(pDX, IDC_COMBO_QUICK, m_Quick);
}

BEGIN_MESSAGE_MAP(CMemo_QBYDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT, &CMemo_QBYDlg::OnEnKillfocusEdit)
	ON_EN_SETFOCUS(IDC_EDIT, &CMemo_QBYDlg::OnEnSetfocusEdit)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMemo_QBYDlg::OnBnClickedButtonAdd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CMemo_QBYDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CMemo_QBYDlg::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMemo_QBYDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEANMEMO, &CMemo_QBYDlg::OnBnClickedButtonCleanmemo)
	ON_BN_CLICKED(IDC_BUTTON_CLEANEDIT, &CMemo_QBYDlg::OnBnClickedButtonCleanedit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_QUICK, &CMemo_QBYDlg::OnBnClickedButtonDeleteQuick)
	ON_BN_CLICKED(IDC_BUTTON_ADDTO_QUICK, &CMemo_QBYDlg::OnBnClickedButtonAddtoQuick)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMemo_QBYDlg ��Ϣ�������

BOOL CMemo_QBYDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//����ʾ��ʽ���Date+Time
	m_SetDate.SetFormat(L"''yyy'-'MM'-'dd'   'HH':'mm':'ss''");
	m_SetDate.SetTime(&CTime::GetCurrentTime()); 

	//����Ĭ��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_ONE))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_QUICK))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_ALARM))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_ALL))->SetCheck(TRUE);
	
	//��������ѡ��
	m_ChooseDays.AddString(L"����");
	m_ChooseDays.AddString(L"��һ");
	m_ChooseDays.AddString(L"�ܶ�");
	m_ChooseDays.AddString(L"����");
	m_ChooseDays.AddString(L"����");
	m_ChooseDays.AddString(L"����");
	m_ChooseDays.AddString(L"����");
	m_ChooseDays.SetCurSel(0);

	//���ÿ������
	m_Quick.AddString(L"��");
	m_Quick.AddString(L"����");
	m_Quick.AddString(L"��������");
	m_Quick.AddString(L"�Ͽ�");
	m_Quick.SetCurSel(0);

	m_Edit.SetLimitText(MAX);//�涨�ַ����ȣ����ڶ�ȡ  

	//�����б�ĸ�ʽ
	LONG lStyle;
	lStyle=GetWindowLong(m_MemoList.m_hWnd,GWL_STYLE);				//��ȡ��ǰ����style
	lStyle|=LVS_SHOWSELALWAYS;										//����style
	SetWindowLong(m_MemoList.m_hWnd,GWL_STYLE, lStyle);				//����style
	m_MemoList.SetExtendedStyle(m_MemoList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //����+������ 
	
	//���÷ָ���
	RECT rect;
	m_MemoList.GetClientRect(&rect);
	LONG width=rect.right-rect.left;
	width/=10;
	m_MemoList.InsertColumn(0,L"�������",LVCFMT_LEFT,0);			//��ʾ�����������е�λ��,����ɾ��
	m_MemoList.InsertColumn(0,L"����",LVCFMT_LEFT,3*width);
	m_MemoList.InsertColumn(0,L"����",LVCFMT_LEFT,1.5*width);
	m_MemoList.InsertColumn(0,L"����",LVCFMT_LEFT,width);
	m_MemoList.InsertColumn(0,L"����",LVCFMT_LEFT,width);
	m_MemoList.InsertColumn(0,L"ʱ��",LVCFMT_LEFT,3.5*width);

	read();
	SetTimer(1,10*1000,NULL);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CMemo_QBYDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_MEMO_QBY_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_MEMO_QBY_DIALOG));
	}
}
#endif


void CMemo_QBYDlg::OnEnKillfocusEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SipShowIM(SIPF_OFF);//ȡ�������
}

void CMemo_QBYDlg::OnEnSetfocusEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SipShowIM(SIPF_ON);//���������
}

void CMemo_QBYDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Memo Node;
	
	CTime tmpTime;
	m_SetDate.GetTime(tmpTime);

	//������������
	if(((CButton *)GetDlgItem(IDC_RADIO_ONE))->GetCheck())
		Node.m_iType=Memo::TYPE_ONE;
	else if(((CButton *)GetDlgItem(IDC_RADIO_WEEK))->GetCheck())
		Node.m_iType=Memo::TYPE_SUN+m_ChooseDays.GetCurSel();
	else
		Node.m_iType=Memo::TYPE_EVR;

	//�����Ƿ�������
	if(((CButton *)GetDlgItem(IDC_CHECK_ALARM))->GetCheck())
		Node.m_bAlarm=true;
	else
		Node.m_bAlarm=false;
	
	//������Ҫ������
	if(((CButton *)GetDlgItem(IDC_RADIO_IMPORTANT_SET))->GetCheck())
		Node.m_iLevel=Memo::LEVEL_IMP;
	else
		Node.m_iLevel=Memo::LEVEL_NAL;
	
	//��������
	if(((CButton *)GetDlgItem(IDC_RADIO_QUICK))->GetCheck())
		m_Quick.GetWindowTextW(Node.m_strEdit);
	else{
		m_Edit.GetWindowTextW(Node.m_strEdit);
		m_Edit.SetWindowText(L"");
	}
		
	Node.m_bSettimer=false;			//��ʱ��
	Node.m_bChange=false;			//�Ƿ񱻸ı�

	//���ʱ���Ƿ�Ϸ�
	if((tmpTime<CTime::GetCurrentTime()) && (Node.m_iType==Memo::TYPE_ONE)){
		MessageBox(L"ʱ���ѹ�!!");
		return;
	}
	else
		Node.m_cTime=tmpTime;
	
	//��֮ǰ��Ŀ���޸�
	for(int i=0;i<m_Arr_Memo.GetCount();i++){   
		Memo n=m_Arr_Memo.GetAt(i);
		if(n.m_bChange==TRUE)
		{
			m_Arr_Memo.GetAt(i).m_bChange=false;
			m_Arr_Memo.GetAt(i).m_cTime=Node.m_cTime;
			m_Arr_Memo.GetAt(i).m_iLevel=Node.m_iLevel;
			m_Arr_Memo.GetAt(i).m_iType=Node.m_iType;
			m_Arr_Memo.GetAt(i).m_bAlarm=Node.m_bAlarm;
			m_Arr_Memo.GetAt(i).m_strEdit=Node.m_strEdit;
			save();
			return;
		}
	}

	//�¼��Ƿ��ظ�
	for(int i=0;i<m_Arr_Memo.GetCount();i++){
		Memo n=m_Arr_Memo.GetAt(i);
		if((	n.m_iType==Memo::TYPE_EVR)								
			&& (Node.m_cTime.GetHour()==n.m_cTime.GetHour())
			&& (Node.m_cTime.GetMinute()==n.m_cTime.GetMinute())
			&& (Node.m_cTime.GetSecond()==n.m_cTime.GetSecond())
			&& (Node.m_iLevel==n.m_iLevel)							
			&& (Node.m_strEdit==n.m_strEdit)
			){
				MessageBox(L"������ͬ��Ŀ!!");
				return;
		}
		else if((	n.m_iType==Memo::TYPE_ONE)
				&& (Node.m_cTime.GetTime()==n.m_cTime.GetTime())
				&& (Node.m_iLevel==n.m_iLevel)					
				&& (Node.m_strEdit==n.m_strEdit)
				){
				MessageBox(L"������ͬ��Ŀ!!");
				return;
		}
		else if((n.m_iType>=Memo::TYPE_SUN)
			&& (Node.m_cTime.GetDayOfWeek()==n.m_cTime.GetDayOfWeek())
			&& (Node.m_cTime.GetHour()==n.m_cTime.GetHour())
			&& (Node.m_cTime.GetMinute()==n.m_cTime.GetMinute())
			&& (Node.m_cTime.GetSecond()==n.m_cTime.GetSecond())
			&& (Node.m_iLevel==n.m_iLevel)
			&& (Node.m_strEdit==n.m_strEdit)
			){
				MessageBox(L"������ͬ��Ŀ!!");
				return;
		}
	}
	m_Arr_Memo.Add(Node);
	save();
}

void CMemo_QBYDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CTime t=CTime::GetCurrentTime();
	switch(nIDEvent){
		case 1:												//1�Ŷ�ʱ����10��һ�α������顣���쵽ʱ����Ŀ���뵹����ʱ��
				for(int i=0;i<m_Arr_Memo.GetCount();i++){
					Memo& n=m_Arr_Memo.GetAt(i);
				
					if(n.m_bSettimer || (!n.m_bAlarm))					
						continue;

					if(n.m_iType==Memo::TYPE_EVR){                 
						if((t.GetHour()==n.m_cTime.GetHour()) && (t.GetMinute()==n.m_cTime.GetMinute()+1)){	//�ൽʱ1min+-9s֮�ڣ����뵹����ʱ��
							for(UINT_PTR id=2;true;id++){
								if(m_timers.count(id)!=0)
									continue;
								SetTimer(id,60*1000,NULL);
								n.m_bSettimer=true;
								m_timers[id]=i;
								break;
							}
						}
					}//if(n.m_iType==Memo::TYPE_EVR)����

					else if(n.m_iType==Memo::TYPE_ONE){
						if(n.m_cTime.GetTime()+10<t.GetTime()){
							for(int j=0;j<m_Arr_Memo.GetSize();j++){
								if(		m_Arr_Memo.GetAt(j).m_iLevel==n.m_iLevel
									&&	m_Arr_Memo.GetAt(j).m_cTime.GetTime()==n.m_cTime.GetTime()
									&&	m_Arr_Memo.GetAt(j).m_iType==n.m_iType
									&&	m_Arr_Memo.GetAt(j).m_bAlarm==n.m_bAlarm
									&&	m_Arr_Memo.GetAt(j).m_strEdit==n.m_strEdit){
										m_Arr_Memo.RemoveAt(j);
										break;
								}
							}
							continue;
						}
						else{									
							if(n.m_cTime.GetTime()>t.GetTime()+30)
								continue;
							for(UINT_PTR id=2;true;id++){
								if(m_timers.count(id)!=0)
									continue;
								SetTimer(id,(n.m_cTime-t).GetTotalSeconds()*1000,NULL);
								n.m_bSettimer=true;
								m_timers[id]=i;
								break;
							}
						}
					}//if(n.m_iType==Memo::TYPE_ONE)����

					else if(t.GetDayOfWeek()==(n.m_iType-1)){
						if((t.GetHour()==n.m_cTime.GetHour()) && (t.GetMinute()==n.m_cTime.GetMinute()+1)){
							for(UINT_PTR id=2;true;id++){
								if(m_timers.count(id)!=0)
									continue;
								SetTimer(id,60*1000,NULL);
								n.m_bSettimer=true;
								m_timers[id]=i;
								break;
							}
						}
					}//if(t.GetDayOfWeek()+Memo::TYPE_SUN-1==n.m_iType)����
				}//for(int i=0;i<m_Arr_Memo.GetCount();i++)����
				break;
		default:
			int index=m_timers[nIDEvent];

			if(m_Arr_Memo[index].m_iType==Memo::TYPE_ONE)
				MessageBox(t.Format(L"%c")+L" "+m_Arr_Memo[index].m_strEdit,L"����", MB_ICONWARNING);
			else
				MessageBox(t.Format(L"%X")+L" "+m_Arr_Memo[index].m_strEdit,L"����", MB_ICONWARNING);

			PlaySound(L"c:\\WINDOWS\\Media\\notify.wav",NULL,SND_ASYNC);		//��������
			m_Arr_Memo.GetAt(index).m_bSettimer=false;
			if(KillTimer(nIDEvent))
				m_timers.erase(m_timers.find(nIDEvent));
	}


	CDialog::OnTimer(nIDEvent);
}

void CMemo_QBYDlg::read(void)
{
	m_Arr_Memo.RemoveAll();
	std::wifstream in(L"\\My Documents\\memo.txt");
	if(!in){			//û���ļ��򴴽�һ��
		save();
		return;
	}
	Memo Node;
	while(!in.eof()){
		__time64_t tmpt;
		WCHAR str[MAX+1];
		in>>tmpt>>Node.m_iLevel>>Node.m_iType>>Node.m_bAlarm>>Node.m_bSettimer;
		in.getline(str,MAX+1);
		Node.m_cTime=CTime(tmpt);
		Node.m_strEdit=CString(str);
		if(Node.m_iType==Memo::TYPE_ONE){
			if(Node.m_cTime<CTime::GetCurrentTime())			
				continue;
		}
		if(in)
			m_Arr_Memo.Add(Node);
	}
	in.close();
}

void CMemo_QBYDlg::save(void)
{
	std::wofstream out(L"\\My Documents\\memo.txt");
	CTime t = CTime::GetCurrentTime();

	for(int i=0;i<m_Arr_Memo.GetCount();i++){
		if(		m_Arr_Memo.GetAt(i).m_cTime.GetTime()<t.GetTime()
			&&	m_Arr_Memo.GetAt(i).m_iType==Memo::TYPE_ONE)
					m_Arr_Memo.RemoveAt(i);
		else{
			Memo n=m_Arr_Memo.GetAt(i);
			out<<n.m_cTime.GetTime()<<L" "<<n.m_iLevel<<L" "<<n.m_iType<<L" "<<n.m_bAlarm<<L" "<<n.m_bSettimer<< n.m_strEdit.GetBuffer()<<"\n";
		}
	}
	out.close();
}

void CMemo_QBYDlg::ListShow(int i, Memo& node)				//�б���ʾ
{
	int nRow;
	CTime t = CTime::GetCurrentTime();
	if(node.m_iType==Memo::TYPE_ONE){
		if(node.m_cTime.GetTime()<t.GetTime()){
				m_Arr_Memo.RemoveAt(i);
				node=m_Arr_Memo.GetAt(i);
				ListShow(i,node);
				return;
		}
		else
			nRow=m_MemoList.InsertItem(0,node.m_cTime.Format(L"%c"));
	}
	else
		nRow=m_MemoList.InsertItem(0,node.m_cTime.Format(L"%X"));

	m_MemoList.SetItemText(nRow,1,(node.m_iLevel==Memo::LEVEL_IMP)?L"��Ҫ":L"һ��");

	if(node.m_iType==Memo::TYPE_EVR)	
		m_MemoList.SetItemText(nRow, 2, L"ÿ��");
	else if(node.m_iType==Memo::TYPE_ONE) 
		m_MemoList.SetItemText(nRow,2,L"һ��");
	else if(node.m_iType==Memo::TYPE_SUN) 
		m_MemoList.SetItemText(nRow,2,L"����");
	else if(node.m_iType==Memo::TYPE_MON) 
		m_MemoList.SetItemText(nRow,2,L"��һ");
	else if(node.m_iType==Memo::TYPE_TUE) 
		m_MemoList.SetItemText(nRow,2,L"�ܶ�");
	else if(node.m_iType==Memo::TYPE_WED) 
		m_MemoList.SetItemText(nRow,2,L"����");
	else if(node.m_iType==Memo::TYPE_THU) 
		m_MemoList.SetItemText(nRow,2,L"����");
	else if(node.m_iType==Memo::TYPE_FRI) 
		m_MemoList.SetItemText(nRow,2,L"����");
	else if(node.m_iType==Memo::TYPE_SAT) 
		m_MemoList.SetItemText(nRow,2,L"����");

	m_MemoList.SetItemText(nRow,3,(node.m_bAlarm)?L"��":L"��");

	m_MemoList.SetItemText(nRow,4,node.m_strEdit);
	
	CString s;
	s.Format(L"%d",i);
	m_MemoList.SetItemText(nRow,5,s);			//��ע�ڵ��������е�λ�ã��Ա�ɾ��
}

void CMemo_QBYDlg::OnBnClickedButtonShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_MemoList.DeleteAllItems();

	//ѡ��ȫ������
	if(((CButton *)GetDlgItem(IDC_RADIO_ALL))->GetCheck()){
		int i=0;
		while(i<m_Arr_Memo.GetSize()){
			Memo n=m_Arr_Memo.GetAt(i);
			ListShow(i,n);
			i++;
		}
	}

	//ѡ��ʱ�䷶Χ
	else if(((CButton *)GetDlgItem(IDC_RADIO_DATE))->GetCheck()){  
		CTime tmpFrom,tmpTo;
		m_DateFrom.GetTime(tmpFrom);
		m_DateTo.GetTime(tmpTo);
		if(tmpFrom > tmpTo){
			MessageBox(L"ʱ�䷶Χ����!!");
			return;
		}
		for(int i=0;i<m_Arr_Memo.GetCount();i++){
			Memo n=m_Arr_Memo.GetAt(i);
			if((tmpTo.GetTime()-tmpFrom.GetTime())>=7 * 24 * 3600)
				ListShow(i,n);
			else if(n.m_iType==Memo::TYPE_EVR)					
				ListShow(i,n);
			else if(	n.m_iType ==Memo::TYPE_ONE
					&&	n.m_cTime.GetTime()>=tmpFrom.GetTime()
					&&	n.m_cTime.GetTime()<=tmpTo.GetTime()
					)
				ListShow(i,n);
			else if(tmpTo.GetDayOfWeek()<=tmpFrom.GetDayOfWeek())
				if((n.m_iType-1)>=tmpFrom.GetDayOfWeek() || (n.m_iType-1)<=tmpTo.GetDayOfWeek())
					ListShow(i,n);
			else if(	tmpTo.GetDayOfWeek()>tmpFrom.GetDayOfWeek()
					&&	(n.m_iType-1)<=tmpTo.GetDayOfWeek()					
					&&	(n.m_iType-1)>=tmpFrom.GetDayOfWeek()
					)
				ListShow(i,n);
		}
	}

	//ѡ����ձ���
	else if(((CButton *)GetDlgItem(IDC_RADIO_TODAY))->GetCheck())				   
		for(int i=0;i<m_Arr_Memo.GetCount();i++){
			Memo n=m_Arr_Memo.GetAt(i);
			CTime today=CTime::GetCurrentTime();
			if(n.m_iType == Memo::TYPE_EVR)
				ListShow(i,n);
			else if(	n.m_iType==Memo::TYPE_ONE
					&&	n.m_cTime.GetYear()==today.GetYear()
					&&	n.m_cTime.GetMonth()==today.GetMonth()
					&&	n.m_cTime.GetDay()==today.GetDay()
					)
				ListShow(i,n);
			else if((n.m_iType-1)==today.GetDayOfWeek())
				ListShow(i,n);
		}

	//ѡ����Ҫ����
	else if(((CButton *)GetDlgItem(IDC_RADIO_IMPORTANT_SEARCH))->GetCheck())
		for(int i=0;i<m_Arr_Memo.GetCount();i++){
			Memo n=m_Arr_Memo.GetAt(i);
			if(n.m_iLevel==Memo::LEVEL_IMP)
				ListShow(i,n);
		}
}

void CMemo_QBYDlg::OnBnClickedButtonChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	POSITION pos=m_MemoList.GetFirstSelectedItemPosition();
	if (pos==NULL)
		return;
	else{
		int nItem=m_MemoList.GetNextSelectedItem(pos);

		CString index=m_MemoList.GetItemText(nItem,5);

		//����ʱ��
		m_SetDate.SetFormat(L"''yyy'-'MM'-'dd'   'HH':'mm':'ss''");
		m_SetDate.SetTime(&m_Arr_Memo.GetAt(_ttoi(index)).m_cTime);

		//��������
		if(m_Arr_Memo.GetAt(_ttoi(index)).m_bAlarm)
			((CButton *)GetDlgItem(IDC_CHECK_ALARM))->SetCheck(TRUE);
		else
			((CButton *)GetDlgItem(IDC_CHECK_ALARM))->SetCheck(FALSE);

		//���ü���
		if(m_Arr_Memo.GetAt(_ttoi(index)).m_iLevel)
			((CButton *)GetDlgItem(IDC_RADIO_IMPORTANT_SET))->SetCheck(TRUE);
		else
			((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(TRUE);

		//�������ѷ�ʽ
		if(m_Arr_Memo.GetAt(_ttoi(index)).m_iType==Memo::TYPE_ONE)
			((CButton *)GetDlgItem(IDC_RADIO_ONE))->SetCheck(TRUE);
		else if(m_Arr_Memo.GetAt(_ttoi(index)).m_iType==Memo::TYPE_EVR)
			((CButton *)GetDlgItem(IDC_RADIO_EVERYDAY))->SetCheck(TRUE);
		else{
			((CButton *)GetDlgItem(IDC_RADIO_WEEK))->SetCheck(TRUE);
			m_ChooseDays.SetCurSel(m_Arr_Memo.GetAt(_ttoi(index)).m_iType-Memo::TYPE_SUN);
		}
		
		//��������
		((CButton *)GetDlgItem(IDC_RADIO_EDIT))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_QUICK))->SetCheck(FALSE);
		m_Edit.SetWindowText(m_Arr_Memo.GetAt(_ttoi(index)).m_strEdit);
		m_Arr_Memo.GetAt(_ttoi(index)).m_bChange=true; 
	}
}

void CMemo_QBYDlg::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	POSITION pos=m_MemoList.GetFirstSelectedItemPosition();
	if (pos==NULL)
		return;
	else{
		int nItem=m_MemoList.GetNextSelectedItem(pos);
		CString index=m_MemoList.GetItemText(nItem,5);
		m_MemoList.DeleteItem(nItem);
		m_Arr_Memo.RemoveAt(_ttoi(index));
		save();
	}
}

void CMemo_QBYDlg::OnBnClickedButtonCleanmemo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_MemoList.DeleteAllItems();
}

void CMemo_QBYDlg::OnBnClickedButtonCleanedit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_Edit.SetWindowText(L"");
}

void CMemo_QBYDlg::OnBnClickedButtonDeleteQuick()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(m_Quick.GetCount()==0)
		return;
	m_Quick.DeleteString(m_Quick.GetCurSel());
	m_Quick.SetCurSel(0);
}

void CMemo_QBYDlg::OnBnClickedButtonAddtoQuick()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString temp;
	m_Edit.GetWindowTextW(temp);
	if(temp.GetLength()>=5){
		MessageBox(L"����!!");
		return;
	}
	else if(temp.GetLength()==0){
		MessageBox(L"�����룡");
		return;
	}
	else
		m_Quick.AddString(temp);
	m_Edit.SetWindowText(L"");
}

void CMemo_QBYDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	save();
	CDialog::OnClose();
}
