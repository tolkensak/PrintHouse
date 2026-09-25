// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "InfoDlg.h"


// CInfoDlg dialog

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(UINT uTID, UINT uType, CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
	, m_uTID(uTID)
	, m_uType(uType)
	, m_strContact(_T(""))
{
}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_CONTACT, m_strContact);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
END_MESSAGE_MAP()


// CInfoDlg message handlers

BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_uTID)
		return TRUE;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	switch(m_uType)
	{
	case ID_VIEW_QUEUE:
		sprintf(pcQuery, "SELECT contact FROM `task` WHERE tid=%d", m_uTID);
		break;
	case ID_VIEW_HISTORY:
        sprintf(pcQuery, "SELECT contact FROM `taskh` WHERE tid=%d", m_uTID);
		((CEdit*)GetDlgItem(IDC_EB_CONTACT))->SetReadOnly();
		break;
	default:
		return TRUE;
	}

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return TRUE;

	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return TRUE;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return TRUE;

	MYSQL_ROW row;
	if(row=pRes->FetchRow())
	{
		if(row[0])
		{
			TCHAR pc[TOL_MAXSTR];
			PULONG len=pRes->FetchLengths();
			MultiByteToWideChar(65001, 0, row[0], len[0]+1, pc, TOL_MAXSTR);
			m_strContact=pc;
		}
	}

	UpdateData(FALSE);
	return TRUE;
}

void CInfoDlg::OnOK()
{
	if(m_uType==ID_VIEW_HISTORY)
	{
		CDialog::OnCancel();
		return;
	}

	CString str=m_strContact;
	UpdateData();

	m_strContact.Trim();
	if(m_strContact!=str)
	{
		MySQLConn* pConn=theApp.GetConn();
		if(!pConn)
			return;

		CHAR pcQuery[MAX_QRY_BUFF_LEN];
		CHAR pc[TOL_MAXSTR];

		WideCharToMultiByte(65001, 0, m_strContact, m_strContact.GetLength()+1, pc, TOL_MAXSTR, NULL, NULL);
		sprintf(pcQuery, "UPDATE `task` SET contact=\'%s\' WHERE tid=%d", pc, m_uTID);
		pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery));
	}

	CDialog::OnOK();
}
