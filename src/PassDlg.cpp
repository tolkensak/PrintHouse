// PassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "PassDlg.h"


// CPassDlg dialog

IMPLEMENT_DYNAMIC(CPassDlg, CDialog)

CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassDlg::IDD, pParent)
	, m_strPassOld(_T(""))
	, m_strPass(_T(""))
	, m_strPassRe(_T(""))
{
}

CPassDlg::~CPassDlg()
{
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_PASS_OLD, m_strPassOld);
	DDX_Text(pDX, IDC_EB_PASS, m_strPass);
	DDX_Text(pDX, IDC_EB_PASS_RE, m_strPassRe);
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialog)
END_MESSAGE_MAP()


// CPassDlg message handlers

BOOL CPassDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CPassDlg::OnOK()
{
	UpdateData();

	if(m_strPassOld.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_PASS_OLD, this);
		return;
	}

	if(m_strPass.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_PASS, this);
		return;
	}

	if(m_strPass!=m_strPassRe)
	{
		theApp.FieldInfo(IDC_EB_PASS_RE, this);
		return;
	}

	CHAR pc1[TOL_MAXSTR], pc2[TOL_MAXSTR];
	WideCharToMultiByte(65001, 0, m_strPassOld, m_strPassOld.GetLength()+1, pc1, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strPass, m_strPass.GetLength()+1, pc2, TOL_MAXSTR, NULL, NULL);

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	sprintf(pcQuery, "UPDATE `user` SET pass=\'%s\' WHERE uid=%d AND pass=\'%s\'", pc2, theApp.m_sess.m_uUID, pc1);

	int n=0;
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0)
		n=(int)pConn->AffectedRows();

	theApp.MessageBox(IDS_INF_PASS_CHANGE_FAILED, MB_ICONINFORMATION, this);

	if(n>0)
		CDialog::OnOK();
}
