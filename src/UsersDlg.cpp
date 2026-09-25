// UserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "UsersDlg.h"
#include "UserDlg.h"

// CUsersDlg dialog

IMPLEMENT_DYNAMIC(CUsersDlg, CDialog)

CUsersDlg::CUsersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsersDlg::IDD, pParent)
	, m_nColNum(4)
{
	m_Users.SetSize(0);
}

CUsersDlg::~CUsersDlg()
{
}

void CUsersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_USERS, m_lcUsers);
}


BEGIN_MESSAGE_MAP(CUsersDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBnClickedBtnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_USERS, OnNMDblclkLcUsers)
END_MESSAGE_MAP()


void CUsersDlg::Reload()
{
	m_Users.RemoveAll();

	m_lcUsers.DeleteAllItems();

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LPCSTR pcQuery="SELECT u.uid, u.rid, u.wid, u.login, u.pass, u.fname, u.mname, u.lname, r.name, w.name FROM user u INNER JOIN role r ON r.rid=u.rid INNER JOIN `work` w ON w.wid=u.wid ORDER BY u.login";
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i;
	CUserPtr pUser;
	PULONG len;
	MYSQL_ROW row;
	TCHAR pc[TOL_MAXSTR];

	while(row=pRes->FetchRow())
	{
		len=pRes->FetchLengths();

		pUser=new CUser;

		i=0;
		pUser->m_uUID=atoi(row[i]);

		i++;
		pUser->m_uRID=atoi(row[i]);

		i++;
		pUser->m_uWID=atoi(row[i]);

		i++;
		MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
		pUser->m_strLogin=pc;

		i++;
		MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
		pUser->m_strPass=pc;

		i++;
		if(row[i])
		{
			MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
			pUser->m_strFName=pc;
		}
		else
			pUser->m_strFName.Empty();

		i++;
		if(row[i])
		{
			MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
			pUser->m_strMName=pc;
		}
		else
			pUser->m_strMName.Empty();

		i++;
		if(row[i])
		{
			MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
			pUser->m_strLName=pc;
		}
		else
			pUser->m_strLName.Empty();

		i++;
		MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
		pUser->m_strRole=pc;

		i++;
		MultiByteToWideChar(65001, 0, row[i], len[i]+1, pc, TOL_MAXSTR);
		pUser->m_strWork=pc;

		m_Users.Add(pUser);
	}


	CString str;
	int n=(int)m_Users.GetCount();

	LVITEM lvi;
	lvi.mask=LVIF_TEXT;

	for(lvi.iItem=0; lvi.iItem<n; lvi.iItem++)
	{
		pUser=m_Users[lvi.iItem];

		lvi.mask|=LVIF_PARAM;
		lvi.lParam=pUser->m_uUID;

		lvi.iSubItem=0;
		lvi.pszText=pUser->m_strLogin.GetBuffer();
		m_lcUsers.InsertItem(&lvi);

		lvi.mask&=~LVIF_PARAM;

		lvi.iSubItem++;
		str=pUser->m_strLName;

		if(!pUser->m_strFName.IsEmpty())
		{
			if(!str.IsEmpty())
				str+=_T(" ");

			str+=pUser->m_strFName;
		}

		if(!pUser->m_strMName.IsEmpty())
		{
			if(!str.IsEmpty())
				str+=_T(" ");

			str+=pUser->m_strMName;
		}

		lvi.pszText=str.GetBuffer();
		m_lcUsers.SetItem(&lvi);

		lvi.iSubItem++;
		lvi.pszText=pUser->m_strRole.GetBuffer();
		m_lcUsers.SetItem(&lvi);

		lvi.iSubItem++;
		lvi.pszText=pUser->m_strWork.GetBuffer();
		m_lcUsers.SetItem(&lvi);
	}
}


BOOL CUsersDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lcUsers.SetExtendedStyle(m_lcUsers.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	int i;
	CString str;
	LVCOLUMN lvc;

	lvc.mask=LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	lvc.cx=120;

	for(i=0; i<m_nColNum; i++)
	{
		lvc.iSubItem=i;
		str.LoadString(IDS_COL_LOGIN+i);
		lvc.pszText=str.GetBuffer();
		m_lcUsers.InsertColumn(i, &lvc);
	}

	Reload();
	return TRUE;
}

void CUsersDlg::OnBnClickedBtnAdd()
{
	CUserDlg dlg;
	if(dlg.DoModal()==IDOK)
		Reload();
}

void CUsersDlg::OnBnClickedBtnEdit()
{
	if(!m_lcUsers.GetSelectedCount())
		return;

	LVITEM lvi;
	lvi.iItem=m_lcUsers.GetNextItem(-1, LVNI_SELECTED);

	lvi.iSubItem=0;
	lvi.mask=LVIF_PARAM;
	m_lcUsers.GetItem(&lvi);
	EditUser((UINT)lvi.lParam);
}

void CUsersDlg::EditUser(UINT uUID)
{
	if(uUID==0)
		return;

	int n=(int)m_Users.GetCount();
	for(int i=0; i<n; i++)
		if(m_Users[i]->m_uUID==uUID)
		{
			CUserDlg dlg(m_Users[i]);
			if(dlg.DoModal()==IDOK)
				Reload();

			break;
		}
}

void CUsersDlg::OnNMDblclkLcUsers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVHITTESTINFO hti;
	hti.flags=LVHT_ONITEM;
	GetCursorPos(&hti.pt);
	m_lcUsers.ScreenToClient(&hti.pt);

	m_lcUsers.HitTest(&hti);
	if(hti.iItem!=-1)
	{
		LVITEM lvi;
        lvi.mask=LVIF_PARAM;
		lvi.iItem=hti.iItem;
		lvi.iSubItem=0;
		m_lcUsers.GetItem(&lvi);
		EditUser((UINT)lvi.lParam);
	}

	*pResult=0;
}

void CUsersDlg::OnBnClickedBtnDel()
{
	if(!m_lcUsers.GetSelectedCount())
		return;

	LVITEM lvi;
	lvi.iItem=m_lcUsers.GetNextItem(-1, LVNI_SELECTED);

	TCHAR pcFullName[TOL_MAXSTR];
	lvi.pszText=pcFullName;
	lvi.cchTextMax=TOL_MAXSTR;

	lvi.iSubItem=1;
	lvi.mask=LVIF_TEXT;
	m_lcUsers.GetItem(&lvi);

	CString str;
	str.Format(IDS_QST_DELETE_USER, pcFullName);
	if(theApp.MessageBox(str, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	lvi.iSubItem=0;
	lvi.mask=LVIF_PARAM;
	m_lcUsers.GetItem(&lvi);

	UINT uUID=(UINT)lvi.lParam;
	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	sprintf(pcQuery, "DELETE FROM user WHERE uid=%d", uUID);
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0)
	{
		m_lcUsers.DeleteItem(lvi.iItem);

		int n=(int)m_Users.GetCount();
		for(int i=0; i<n; i++)
			if(m_Users[i]->m_uUID==uUID)
			{
				m_Users.RemoveAt(i);
				break;
			}
	}
}
