// UserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "UserDlg.h"

// CUserDlg dialog

IMPLEMENT_DYNAMIC(CUserDlg, CDialog)

CUserDlg::CUserDlg(CUserPtr pUser, CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlg::IDD, pParent)
	, m_pUser(pUser)
	, m_strLogin(_T(""))
	, m_strPass(_T(""))
	, m_strPassRe(_T(""))
	, m_strLName(_T(""))
	, m_strFName(_T(""))
	, m_strMName(_T(""))
	, m_nRole(-1)
	, m_nWork(-1)
{
}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_LOGIN, m_strLogin);
	DDX_Text(pDX, IDC_EB_PASS, m_strPass);
	DDX_Text(pDX, IDC_EB_PASS_RE, m_strPassRe);
	DDX_Text(pDX, IDC_EB_LNAME, m_strLName);
	DDX_Text(pDX, IDC_EB_FNAME, m_strFName);
	DDX_Text(pDX, IDC_EB_MNAME, m_strMName);
	DDX_CBIndex(pDX, IDC_CB_ROLE, m_nRole);
	DDX_CBIndex(pDX, IDC_CB_WORK, m_nWork);
	DDX_Control(pDX, IDC_CB_ROLE, m_cbRole);
	DDX_Control(pDX, IDC_CB_WORK, m_cbWork);
}


BEGIN_MESSAGE_MAP(CUserDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CB_ROLE, OnCbnSelchangeCbRole)
END_MESSAGE_MAP()


// CUserDlg message handlers

BOOL CUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pUser)
	{
		m_strLogin=m_pUser->m_strLogin;
		m_strPass=m_pUser->m_strPass;
		m_strPassRe=m_pUser->m_strPass;
		m_strLName=m_pUser->m_strLName;
		m_strFName=m_pUser->m_strFName;
		m_strMName=m_pUser->m_strMName;

		m_cbWork.EnableWindow(m_pUser->m_uRID==3);
	}
	else
		m_cbWork.EnableWindow(0);

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return TRUE;

	LPCSTR pcQuery="SELECT rid, name FROM `role` ORDER BY 2; SELECT wid, name FROM `work` WHERE wid>1 ORDER BY 2";
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return TRUE;

	int i;
	PULONG len;
	MYSQL_ROW row;
	TCHAR pc[TOL_MAXSTR];
	CComboBox* pCb=&m_cbRole;
	UINT u;

	do {
		MySQLResPtr pRes=pConn->StoreResult();
		if(pRes)
		{
			while(row=pRes->FetchRow())
			{
				len=pRes->FetchLengths();

				MultiByteToWideChar(65001, 0, row[1], len[1]+1, pc, TOL_MAXSTR);
				i=pCb->AddString(pc);

				u=atoi(row[0]);
				pCb->SetItemData(i, u);

				if(m_pUser)
				{
					if(m_nRole<0 && pCb==&m_cbRole)
					{
						if(u==m_pUser->m_uRID)
							m_nRole=i;
					}
					else if(m_nWork<0 && pCb==&m_cbWork)
					{
						if(u==m_pUser->m_uWID)
							m_nWork=i;
					}
				}
			}
		}

		if(pCb==&m_cbRole)
			pCb=&m_cbWork;
		else
			break;
	} while(pConn->NextResult()==0);


	UpdateData(FALSE);
	return TRUE;
}

void CUserDlg::OnCbnSelchangeCbRole()
{
	UpdateData();
	m_cbWork.EnableWindow(m_cbRole.GetItemData(m_nRole)==3);
}

void CUserDlg::OnOK()
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

	if(m_strPass!=m_strPassRe)
	{
		theApp.FieldInfo(IDC_EB_PASS_RE, this);
		return;
	}

	m_strFName.Trim();
	m_strMName.Trim();
	m_strLName.Trim();

	if(m_strFName.IsEmpty()
		&& m_strMName.IsEmpty()
		&& m_strLName.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_FNAME, this);
		return;
	}

	if(m_nRole<0)
	{
		theApp.FieldInfo(IDC_CB_ROLE, this);
		return;
	}

	UINT uWID=1;
	UINT uRID=(UINT)m_cbRole.GetItemData(m_nRole);

	if(uRID==3)
	{
		if(m_nWork<0)
		{
			theApp.FieldInfo(IDC_CB_WORK, this);
			return;
		}
		else
			uWID=(UINT)m_cbWork.GetItemData(m_nWork);

	}

	CHAR pc1[TOL_MAXSTR],
		pc2[TOL_MAXSTR],
		pc3[TOL_MAXSTR],
		pc4[TOL_MAXSTR],
		pc5[TOL_MAXSTR];

	WideCharToMultiByte(65001, 0, m_strLogin, m_strLogin.GetLength()+1, pc1, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strPass, m_strPass.GetLength()+1, pc2, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strFName, m_strFName.GetLength()+1, pc3, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strMName, m_strMName.GetLength()+1, pc4, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strLName, m_strLName.GetLength()+1, pc5, TOL_MAXSTR, NULL, NULL);

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	if(m_pUser)
		sprintf(pcQuery, "REPLACE INTO `user`(uid, rid, wid, login, pass, fname, mname, lname) VALUES(%d, %d, %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')", m_pUser->m_uUID, uRID, uWID, pc1, pc2, pc3, pc4, pc5);
	else
		sprintf(pcQuery, "INSERT INTO `user`(rid, wid, login, pass, fname, mname, lname) VALUES(%d, %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')", uRID, uWID, pc1, pc2, pc3, pc4, pc5);

	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	CDialog::OnOK();
}
