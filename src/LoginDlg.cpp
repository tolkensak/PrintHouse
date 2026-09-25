// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "LoginDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_strLogin(_T(""))
	, m_strPass(_T(""))
{
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_LOGIN, m_strLogin);
	DDX_Text(pDX, IDC_EB_PASS, m_strPass);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
END_MESSAGE_MAP()


// CLoginDlg message handlers

void CLoginDlg::OnOK()
{
	UpdateData();

	m_strLogin.Trim();
	if(m_strLogin.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_LOGIN, this);
		return;
	}

	if(m_strPass.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_PASS, this);
		return;
	}

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	CHAR pcLogin[TOL_MAXSTR];
	CHAR pcPass[TOL_MAXSTR];

	WideCharToMultiByte(65001, 0, m_strLogin, m_strLogin.GetLength()+1, pcLogin, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strPass, m_strPass.GetLength()+1, pcPass, TOL_MAXSTR, NULL, NULL);

	sprintf(pcQuery, "SELECT u.uid, u.rid, u.wid FROM `user` u WHERE u.login=\'%s\' AND u.pass=\'%s\'", pcLogin, pcPass);
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	MYSQL_ROW row=pRes->FetchRow();
	if(row)
		m_sess(atoi(row[0]), atoi(row[1]), atoi(row[2]));

	if(m_sess.m_uUID==0)
		theApp.MessageBox(IDS_WRN_LOGIN_FAILED, MB_ICONWARNING, this);
	else
		CDialog::OnOK();
}
