// Memo_QBYDlg.cpp : 实现文件
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

// CMemo_QBYDlg 对话框

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


// CMemo_QBYDlg 消息处理程序

BOOL CMemo_QBYDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//将显示格式设成Date+Time
	m_SetDate.SetFormat(L"''yyy'-'MM'-'dd'   'HH':'mm':'ss''");
	m_SetDate.SetTime(&CTime::GetCurrentTime()); 

	//设置默认选项
	((CButton *)GetDlgItem(IDC_RADIO_ONE))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_QUICK))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_ALARM))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_ALL))->SetCheck(TRUE);
	
	//设置日期选项
	m_ChooseDays.AddString(L"周日");
	m_ChooseDays.AddString(L"周一");
	m_ChooseDays.AddString(L"周二");
	m_ChooseDays.AddString(L"周三");
	m_ChooseDays.AddString(L"周四");
	m_ChooseDays.AddString(L"周五");
	m_ChooseDays.AddString(L"周六");
	m_ChooseDays.SetCurSel(0);

	//设置快捷内容
	m_Quick.AddString(L"起床");
	m_Quick.AddString(L"会议");
	m_Quick.AddString(L"朋友来访");
	m_Quick.AddString(L"上课");
	m_Quick.SetCurSel(0);

	m_Edit.SetLimitText(MAX);//规定字符长度，易于读取  

	//设置列表的格式
	LONG lStyle;
	lStyle=GetWindowLong(m_MemoList.m_hWnd,GWL_STYLE);				//获取当前窗口style
	lStyle|=LVS_SHOWSELALWAYS;										//设置style
	SetWindowLong(m_MemoList.m_hWnd,GWL_STYLE, lStyle);				//设置style
	m_MemoList.SetExtendedStyle(m_MemoList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //高亮+网格线 
	
	//设置分隔段
	RECT rect;
	m_MemoList.GetClientRect(&rect);
	LONG width=rect.right-rect.left;
	width/=10;
	m_MemoList.InsertColumn(0,L"备忘序号",LVCFMT_LEFT,0);			//显示内容在数组中的位置,方便删除
	m_MemoList.InsertColumn(0,L"内容",LVCFMT_LEFT,3*width);
	m_MemoList.InsertColumn(0,L"闹铃",LVCFMT_LEFT,1.5*width);
	m_MemoList.InsertColumn(0,L"提醒",LVCFMT_LEFT,width);
	m_MemoList.InsertColumn(0,L"类型",LVCFMT_LEFT,width);
	m_MemoList.InsertColumn(0,L"时间",LVCFMT_LEFT,3.5*width);

	read();
	SetTimer(1,10*1000,NULL);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	// TODO: 在此添加控件通知处理程序代码
	SipShowIM(SIPF_OFF);//取消软键盘
}

void CMemo_QBYDlg::OnEnSetfocusEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	SipShowIM(SIPF_ON);//调用软件盘
}

void CMemo_QBYDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	Memo Node;
	
	CTime tmpTime;
	m_SetDate.GetTime(tmpTime);

	//设置提醒类型
	if(((CButton *)GetDlgItem(IDC_RADIO_ONE))->GetCheck())
		Node.m_iType=Memo::TYPE_ONE;
	else if(((CButton *)GetDlgItem(IDC_RADIO_WEEK))->GetCheck())
		Node.m_iType=Memo::TYPE_SUN+m_ChooseDays.GetCurSel();
	else
		Node.m_iType=Memo::TYPE_EVR;

	//设置是否有闹钟
	if(((CButton *)GetDlgItem(IDC_CHECK_ALARM))->GetCheck())
		Node.m_bAlarm=true;
	else
		Node.m_bAlarm=false;
	
	//设置重要度类型
	if(((CButton *)GetDlgItem(IDC_RADIO_IMPORTANT_SET))->GetCheck())
		Node.m_iLevel=Memo::LEVEL_IMP;
	else
		Node.m_iLevel=Memo::LEVEL_NAL;
	
	//设置内容
	if(((CButton *)GetDlgItem(IDC_RADIO_QUICK))->GetCheck())
		m_Quick.GetWindowTextW(Node.m_strEdit);
	else{
		m_Edit.GetWindowTextW(Node.m_strEdit);
		m_Edit.SetWindowText(L"");
	}
		
	Node.m_bSettimer=false;			//定时器
	Node.m_bChange=false;			//是否被改变

	//检查时间是否合法
	if((tmpTime<CTime::GetCurrentTime()) && (Node.m_iType==Memo::TYPE_ONE)){
		MessageBox(L"时间已过!!");
		return;
	}
	else
		Node.m_cTime=tmpTime;
	
	//对之前条目的修改
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

	//事件是否重复
	for(int i=0;i<m_Arr_Memo.GetCount();i++){
		Memo n=m_Arr_Memo.GetAt(i);
		if((	n.m_iType==Memo::TYPE_EVR)								
			&& (Node.m_cTime.GetHour()==n.m_cTime.GetHour())
			&& (Node.m_cTime.GetMinute()==n.m_cTime.GetMinute())
			&& (Node.m_cTime.GetSecond()==n.m_cTime.GetSecond())
			&& (Node.m_iLevel==n.m_iLevel)							
			&& (Node.m_strEdit==n.m_strEdit)
			){
				MessageBox(L"已有相同条目!!");
				return;
		}
		else if((	n.m_iType==Memo::TYPE_ONE)
				&& (Node.m_cTime.GetTime()==n.m_cTime.GetTime())
				&& (Node.m_iLevel==n.m_iLevel)					
				&& (Node.m_strEdit==n.m_strEdit)
				){
				MessageBox(L"已有相同条目!!");
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
				MessageBox(L"已有相同条目!!");
				return;
		}
	}
	m_Arr_Memo.Add(Node);
	save();
}

void CMemo_QBYDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CTime t=CTime::GetCurrentTime();
	switch(nIDEvent){
		case 1:												//1号定时器，10秒一次遍历数组。将快到时的条目放入倒数定时器
				for(int i=0;i<m_Arr_Memo.GetCount();i++){
					Memo& n=m_Arr_Memo.GetAt(i);
				
					if(n.m_bSettimer || (!n.m_bAlarm))					
						continue;

					if(n.m_iType==Memo::TYPE_EVR){                 
						if((t.GetHour()==n.m_cTime.GetHour()) && (t.GetMinute()==n.m_cTime.GetMinute()+1)){	//距到时1min+-9s之内，进入倒数定时器
							for(UINT_PTR id=2;true;id++){
								if(m_timers.count(id)!=0)
									continue;
								SetTimer(id,60*1000,NULL);
								n.m_bSettimer=true;
								m_timers[id]=i;
								break;
							}
						}
					}//if(n.m_iType==Memo::TYPE_EVR)结束

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
					}//if(n.m_iType==Memo::TYPE_ONE)结束

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
					}//if(t.GetDayOfWeek()+Memo::TYPE_SUN-1==n.m_iType)结束
				}//for(int i=0;i<m_Arr_Memo.GetCount();i++)结束
				break;
		default:
			int index=m_timers[nIDEvent];

			if(m_Arr_Memo[index].m_iType==Memo::TYPE_ONE)
				MessageBox(t.Format(L"%c")+L" "+m_Arr_Memo[index].m_strEdit,L"提醒", MB_ICONWARNING);
			else
				MessageBox(t.Format(L"%X")+L" "+m_Arr_Memo[index].m_strEdit,L"提醒", MB_ICONWARNING);

			PlaySound(L"c:\\WINDOWS\\Media\\notify.wav",NULL,SND_ASYNC);		//播放声音
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
	if(!in){			//没有文件则创建一个
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

void CMemo_QBYDlg::ListShow(int i, Memo& node)				//列表显示
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

	m_MemoList.SetItemText(nRow,1,(node.m_iLevel==Memo::LEVEL_IMP)?L"重要":L"一般");

	if(node.m_iType==Memo::TYPE_EVR)	
		m_MemoList.SetItemText(nRow, 2, L"每天");
	else if(node.m_iType==Memo::TYPE_ONE) 
		m_MemoList.SetItemText(nRow,2,L"一次");
	else if(node.m_iType==Memo::TYPE_SUN) 
		m_MemoList.SetItemText(nRow,2,L"周日");
	else if(node.m_iType==Memo::TYPE_MON) 
		m_MemoList.SetItemText(nRow,2,L"周一");
	else if(node.m_iType==Memo::TYPE_TUE) 
		m_MemoList.SetItemText(nRow,2,L"周二");
	else if(node.m_iType==Memo::TYPE_WED) 
		m_MemoList.SetItemText(nRow,2,L"周三");
	else if(node.m_iType==Memo::TYPE_THU) 
		m_MemoList.SetItemText(nRow,2,L"周四");
	else if(node.m_iType==Memo::TYPE_FRI) 
		m_MemoList.SetItemText(nRow,2,L"周五");
	else if(node.m_iType==Memo::TYPE_SAT) 
		m_MemoList.SetItemText(nRow,2,L"周六");

	m_MemoList.SetItemText(nRow,3,(node.m_bAlarm)?L"有":L"无");

	m_MemoList.SetItemText(nRow,4,node.m_strEdit);
	
	CString s;
	s.Format(L"%d",i);
	m_MemoList.SetItemText(nRow,5,s);			//标注节点在数组中的位置，以便删除
}

void CMemo_QBYDlg::OnBnClickedButtonShow()
{
	// TODO: 在此添加控件通知处理程序代码

	m_MemoList.DeleteAllItems();

	//选择全部备忘
	if(((CButton *)GetDlgItem(IDC_RADIO_ALL))->GetCheck()){
		int i=0;
		while(i<m_Arr_Memo.GetSize()){
			Memo n=m_Arr_Memo.GetAt(i);
			ListShow(i,n);
			i++;
		}
	}

	//选择时间范围
	else if(((CButton *)GetDlgItem(IDC_RADIO_DATE))->GetCheck()){  
		CTime tmpFrom,tmpTo;
		m_DateFrom.GetTime(tmpFrom);
		m_DateTo.GetTime(tmpTo);
		if(tmpFrom > tmpTo){
			MessageBox(L"时间范围错误!!");
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

	//选择今日备忘
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

	//选择重要记事
	else if(((CButton *)GetDlgItem(IDC_RADIO_IMPORTANT_SEARCH))->GetCheck())
		for(int i=0;i<m_Arr_Memo.GetCount();i++){
			Memo n=m_Arr_Memo.GetAt(i);
			if(n.m_iLevel==Memo::LEVEL_IMP)
				ListShow(i,n);
		}
}

void CMemo_QBYDlg::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos=m_MemoList.GetFirstSelectedItemPosition();
	if (pos==NULL)
		return;
	else{
		int nItem=m_MemoList.GetNextSelectedItem(pos);

		CString index=m_MemoList.GetItemText(nItem,5);

		//设置时间
		m_SetDate.SetFormat(L"''yyy'-'MM'-'dd'   'HH':'mm':'ss''");
		m_SetDate.SetTime(&m_Arr_Memo.GetAt(_ttoi(index)).m_cTime);

		//设置闹钟
		if(m_Arr_Memo.GetAt(_ttoi(index)).m_bAlarm)
			((CButton *)GetDlgItem(IDC_CHECK_ALARM))->SetCheck(TRUE);
		else
			((CButton *)GetDlgItem(IDC_CHECK_ALARM))->SetCheck(FALSE);

		//设置级别
		if(m_Arr_Memo.GetAt(_ttoi(index)).m_iLevel)
			((CButton *)GetDlgItem(IDC_RADIO_IMPORTANT_SET))->SetCheck(TRUE);
		else
			((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(TRUE);

		//设置提醒方式
		if(m_Arr_Memo.GetAt(_ttoi(index)).m_iType==Memo::TYPE_ONE)
			((CButton *)GetDlgItem(IDC_RADIO_ONE))->SetCheck(TRUE);
		else if(m_Arr_Memo.GetAt(_ttoi(index)).m_iType==Memo::TYPE_EVR)
			((CButton *)GetDlgItem(IDC_RADIO_EVERYDAY))->SetCheck(TRUE);
		else{
			((CButton *)GetDlgItem(IDC_RADIO_WEEK))->SetCheck(TRUE);
			m_ChooseDays.SetCurSel(m_Arr_Memo.GetAt(_ttoi(index)).m_iType-Memo::TYPE_SUN);
		}
		
		//设置内容
		((CButton *)GetDlgItem(IDC_RADIO_EDIT))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_QUICK))->SetCheck(FALSE);
		m_Edit.SetWindowText(m_Arr_Memo.GetAt(_ttoi(index)).m_strEdit);
		m_Arr_Memo.GetAt(_ttoi(index)).m_bChange=true; 
	}
}

void CMemo_QBYDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码

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
	// TODO: 在此添加控件通知处理程序代码

	m_MemoList.DeleteAllItems();
}

void CMemo_QBYDlg::OnBnClickedButtonCleanedit()
{
	// TODO: 在此添加控件通知处理程序代码

	m_Edit.SetWindowText(L"");
}

void CMemo_QBYDlg::OnBnClickedButtonDeleteQuick()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_Quick.GetCount()==0)
		return;
	m_Quick.DeleteString(m_Quick.GetCurSel());
	m_Quick.SetCurSel(0);
}

void CMemo_QBYDlg::OnBnClickedButtonAddtoQuick()
{
	// TODO: 在此添加控件通知处理程序代码

	CString temp;
	m_Edit.GetWindowTextW(temp);
	if(temp.GetLength()>=5){
		MessageBox(L"过长!!");
		return;
	}
	else if(temp.GetLength()==0){
		MessageBox(L"请输入！");
		return;
	}
	else
		m_Quick.AddString(temp);
	m_Edit.SetWindowText(L"");
}

void CMemo_QBYDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	save();
	CDialog::OnClose();
}
