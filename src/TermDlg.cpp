// TermDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "TermDlg.h"


// CTermDlg dialog

IMPLEMENT_DYNAMIC(CTermDlg, CDialog)
CTermDlg::CTermDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTermDlg::IDD, pParent)
	, m_strCodeC(_T(""))
	, m_strCodeE(_T(""))
	, m_strJobCF(_T(""))
	, m_uJobEF(0)
	, m_strJobCT(_T(""))
	, m_uJobET(0)
	, m_strReferDateCF(_T(">="))
	, m_dtReferDateDF(COleDateTime::GetCurrentTime())
	, m_strReferDateCT(_T(""))
	, m_dtReferDateDT(COleDateTime::GetCurrentTime())
	, m_strCompanyC(_T(""))
	, m_strCompanyE(_T(""))
	, m_strTaskC(_T(""))
	, m_strTaskE(_T(""))
	, m_strEditionCF(_T(""))
	, m_uEditionEF(0)
	, m_strEditionCT(_T(""))
	, m_uEditionET(0)
	, m_strSizeC(_T(""))
	, m_strSizeE(_T(""))
	, m_strPaperFmtC(_T(""))
	, m_strPaperFmtE(_T(""))
	, m_strPaperNumCF(_T(""))
	, m_uPaperNumEF(0)
	, m_strPaperNumCT(_T(""))
	, m_uPaperNumET(0)
	, m_strPrintFmtC(_T(""))
	, m_strPrintFmtE(_T(""))
	, m_strColorC(_T(""))
	, m_strColorE(_T(""))
	, m_strPlastNumCF(_T(""))
	, m_uPlastNumEF(0)
	, m_strPlastNumCT(_T(""))
	, m_uPlastNumET(0)
	, m_strNoteC(_T(""))
	, m_strNoteE(_T(""))
	, m_iCode(0)
	, m_iJob(0)
	, m_iReferDate(1)
	, m_iCompany(0)
	, m_iTask(0)
	, m_iEdition(0)
	, m_iSize(0)
	, m_iPaperFmt(0)
	, m_iPaperNum(0)
	, m_iPrintFmt(0)
	, m_iColor(0)
	, m_iPlastNum(0)
	, m_iNote(0)
	, m_bCode(FALSE)
	, m_bJob(FALSE)
	, m_bReferDate(TRUE)
	, m_bCompany(FALSE)
	, m_bTask(FALSE)
	, m_bEdition(FALSE)
	, m_bSize(FALSE)
	, m_bPaperFmt(FALSE)
	, m_bPaperNum(FALSE)
	, m_bPrintFmt(FALSE)
	, m_bColor(FALSE)
	, m_bPlastNum(FALSE)
	, m_bNote(FALSE)
{
	m_dtReferDateDF-=COleDateTimeSpan(30);
}

CTermDlg::~CTermDlg()
{
}

void CTermDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CB_COL_CODE, m_strCodeC);
	DDX_Text(pDX, IDC_EB_COL_CODE, m_strCodeE);
	DDX_CBString(pDX, IDC_CB_COL_JOB_FROM, m_strJobCF);
	DDX_Text(pDX, IDC_EB_COL_JOB_FROM, m_uJobEF);
	DDX_CBString(pDX, IDC_CB_COL_JOB_TO, m_strJobCT);
	DDX_Text(pDX, IDC_EB_COL_JOB_TO, m_uJobET);
	DDX_CBString(pDX, IDC_CB_COL_REFER_DATE_FROM, m_strReferDateCF);
	DDX_DateTimeCtrl(pDX, IDC_DT_COL_REFER_DATE_FORM, m_dtReferDateDF);
	DDX_CBString(pDX, IDC_CB_COL_REFER_DATE_TO, m_strReferDateCT);
	DDX_DateTimeCtrl(pDX, IDC_DT_COL_REFER_DATE_TO, m_dtReferDateDT);
	DDX_CBString(pDX, IDC_CB_COL_COMPANY, m_strCompanyC);
	DDX_Text(pDX, IDC_EB_COL_COMPANY, m_strCompanyE);
	DDX_CBString(pDX, IDC_CB_COL_TASK, m_strTaskC);
	DDX_Text(pDX, IDC_EB_COL_TASK, m_strTaskE);
	DDX_CBString(pDX, IDC_CB_COL_EDITION_FROM, m_strEditionCF);
	DDX_Text(pDX, IDC_EB_COL_EDITION_FROM, m_uEditionEF);
	DDX_CBString(pDX, IDC_CB_COL_EDITION_TO, m_strEditionCT);
	DDX_Text(pDX, IDC_EB_COL_EDITION_TO, m_uEditionET);
	DDX_CBString(pDX, IDC_CB_COL_SIZE, m_strSizeC);
	DDX_Text(pDX, IDC_EB_COL_SIZE, m_strSizeE);
	DDX_CBString(pDX, IDC_CB_COL_PAPER_FMT, m_strPaperFmtC);
	DDX_Text(pDX, IDC_EB_COL_PAPER_FMT, m_strPaperFmtE);
	DDX_CBString(pDX, IDC_CB_COL_PAPER_NUM_FROM, m_strPaperNumCF);
	DDX_Text(pDX, IDC_EB_COL_PAPER_NUM_FROM, m_uPaperNumEF);
	DDX_CBString(pDX, IDC_CB_COL_PAPER_NUM_TO, m_strPaperNumCT);
	DDX_Text(pDX, IDC_EB_COL_PAPER_NUM_TO, m_uPaperNumET);
	DDX_CBString(pDX, IDC_CB_COL_PRINT_FMT, m_strPrintFmtC);
	DDX_Text(pDX, IDC_EB_COL_PRINT_FMT, m_strPrintFmtE);
	DDX_CBString(pDX, IDC_CB_COL_COLOR, m_strColorC);
	DDX_Text(pDX, IDC_EB_COL_COLOR, m_strColorE);
	DDX_CBString(pDX, IDC_CB_COL_PLAST_NUM_FROM, m_strPlastNumCF);
	DDX_Text(pDX, IDC_EB_COL_PLAST_NUM_FROM, m_uPlastNumEF);
	DDX_CBString(pDX, IDC_CB_COL_PLAST_NUM_TO, m_strPlastNumCT);
	DDX_Text(pDX, IDC_EB_COL_PLAST_NUM_TO, m_uPlastNumET);
	DDX_CBString(pDX, IDC_CB_COL_NOTE, m_strNoteC);
	DDX_Text(pDX, IDC_EB_COL_NOTE, m_strNoteE);
	DDX_CBIndex(pDX, IDC_CB_COL_CODE_O, m_iCode);
	DDX_CBIndex(pDX, IDC_CB_COL_JOB_O, m_iJob);
	DDX_CBIndex(pDX, IDC_CB_COL_REFER_DATE_O, m_iReferDate);
	DDX_CBIndex(pDX, IDC_CB_COL_COMPANY_O, m_iCompany);
	DDX_CBIndex(pDX, IDC_CB_COL_TASK_O, m_iTask);
	DDX_CBIndex(pDX, IDC_CB_COL_EDITION_O, m_iEdition);
	DDX_CBIndex(pDX, IDC_CB_COL_SIZE_O, m_iSize);
	DDX_CBIndex(pDX, IDC_CB_COL_PAPER_FMT_O, m_iPaperFmt);
	DDX_CBIndex(pDX, IDC_CB_COL_PAPER_NUM_O, m_iPaperNum);
	DDX_CBIndex(pDX, IDC_CB_COL_PRINT_FMT_O, m_iPrintFmt);
	DDX_CBIndex(pDX, IDC_CB_COL_COLOR_O, m_iColor);
	DDX_CBIndex(pDX, IDC_CB_COL_PLAST_NUM_O, m_iPlastNum);
	DDX_CBIndex(pDX, IDC_CB_COL_NOTE_O, m_iNote);
	DDX_Check(pDX, IDC_CH_COL_CODE, m_bCode);
	DDX_Check(pDX, IDC_CH_COL_JOB, m_bJob);
	DDX_Check(pDX, IDC_CH_COL_REFER_DATE, m_bReferDate);
	DDX_Check(pDX, IDC_CH_COL_COMPANY, m_bCompany);
	DDX_Check(pDX, IDC_CH_COL_TASK, m_bTask);
	DDX_Check(pDX, IDC_CH_COL_EDITION, m_bEdition);
	DDX_Check(pDX, IDC_CH_COL_SIZE, m_bSize);
	DDX_Check(pDX, IDC_CH_COL_PAPER_FMT, m_bPaperFmt);
	DDX_Check(pDX, IDC_CH_COL_PAPER_NUM, m_bPaperNum);
	DDX_Check(pDX, IDC_CH_COL_PRINT_FMT, m_bPrintFmt);
	DDX_Check(pDX, IDC_CH_COL_COLOR, m_bColor);
	DDX_Check(pDX, IDC_CH_COL_PLAST_NUM, m_bPlastNum);
	DDX_Check(pDX, IDC_CH_COL_NOTE, m_bNote);
}


BEGIN_MESSAGE_MAP(CTermDlg, CDialog)
END_MESSAGE_MAP()


// CTermDlg message handlers

BOOL CTermDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	for(UINT u=IDS_COL_CODE; u<=IDS_COL_NOTE; u++)
	{
		str.LoadString(u);
		SetDlgItemText(u, str);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTermDlg::OnOK()
{
	if(!UpdateData())
		return;

	CDialog::OnOK();
}
