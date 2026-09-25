// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "OptionsDlg.h"


// COptionsDlg dialog

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
	, m_bAutoReload(theApp.m_bAutoReload)
	, m_uReloadSpace(theApp.m_uReloadSpace)
{
	MultiByteToWideChar(65001, 0, theApp.m_pcServer, (int)strlen(theApp.m_pcServer)+1, m_strServer.GetBuffer(TOL_MAXSTR-1), TOL_MAXSTR);
	m_strServer.ReleaseBuffer();
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_SERVER, m_strServer);
	DDX_Check(pDX, IDC_CH_AUTO_RELOAD, m_bAutoReload);
	DDX_Text(pDX, IDC_EB_RELOAD_SPACE, m_uReloadSpace);
	DDV_MinMaxUInt(pDX, m_uReloadSpace, 1, 60);
	DDX_Control(pDX, IDC_EB_RELOAD_SPACE, m_ebReloadSpace);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDC_CH_AUTO_RELOAD, OnBnClickedChkAutoReload)
END_MESSAGE_MAP()


// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ebReloadSpace.EnableWindow(m_bAutoReload);
	UpdateData(FALSE);
	return TRUE;
}

void COptionsDlg::OnBnClickedChkAutoReload()
{
	m_ebReloadSpace.EnableWindow(IsDlgButtonChecked(IDC_CH_AUTO_RELOAD));
}

void COptionsDlg::OnOK()
{
	if(!UpdateData())
		return;

	m_strServer.Trim();
	if(m_strServer.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_SERVER, this);
		return;
	}

	BOOL bChanged=FALSE;
	CHAR pc[TOL_MAXSTR];

	WideCharToMultiByte(65001, 0, m_strServer, m_strServer.GetLength()+1, pc, TOL_MAXSTR, NULL, NULL);
	if(strcmp(theApp.m_pcServer, pc))
	{
		bChanged=TRUE;
		strcpy(theApp.m_pcServer, pc);
		theApp.WriteProfileString(_T(""), _T("Server"), m_strServer);
	}

	if(theApp.m_bAutoReload!=m_bAutoReload)
	{
		bChanged=TRUE;
		theApp.m_bAutoReload=m_bAutoReload;
		theApp.WriteProfileInt(_T(""), _T("AutoReload"), m_bAutoReload);
	}

	if(theApp.m_uReloadSpace!=m_uReloadSpace)
	{
		bChanged=TRUE;
		theApp.m_uReloadSpace=m_uReloadSpace;
		theApp.WriteProfileInt(_T(""), _T("ReloadSpace"), m_uReloadSpace);
	}

	bChanged?CDialog::OnOK():CDialog::OnCancel();
}
