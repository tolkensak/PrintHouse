// JobDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "JobDlg.h"


// CJobDlg dialog

IMPLEMENT_DYNAMIC(CJobDlg, CDialog)

CJobDlg::CJobDlg(CWorkArray* pWorks, CWnd* pParent /*=NULL*/)
	: CDialog(CJobDlg::IDD, pParent)
	, m_pWorks(pWorks)
{
}

CJobDlg::~CJobDlg()
{
}

void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_WORK, m_lbWork);
	DDX_Control(pDX, IDC_LB_VALUE, m_lbValue);
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LB_WORK, OnLbnSelchangeLbWork)
END_MESSAGE_MAP()


// CJobDlg message handlers

BOOL CJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pWorks)
	{
		int n=(int)m_pWorks->GetCount();
		for(int i=0; i<n; i++)
			m_lbWork.AddString(m_pWorks->GetAt(i)->m_strName);
	}

	return TRUE;
}

void CJobDlg::OnLbnSelchangeLbWork()
{
	m_lbValue.ResetContent();

	int i=m_lbWork.GetCurSel();
	if(i<0)
		return;

	CWorkValueArray* pValues=&m_pWorks->GetAt(i)->m_Values;
	int n=(int)pValues->GetCount();
	for(i=0; i<n; i++)
		m_lbValue.AddString(pValues->GetAt(i)->m_strName);
}

void CJobDlg::OnOK()
{
	int i=m_lbWork.GetCurSel();
	if(i<0)
		return;

	int j=m_lbValue.GetCurSel();
	if(j<0)
		return;

	GetParent()->SendMessage(WM_ADD_JOB, i, j);
}
