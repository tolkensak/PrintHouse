
#pragma once

// CTermDlg dialog

class CTermDlg : public CDialog
{
	DECLARE_DYNAMIC(CTermDlg)

public:
	CTermDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTermDlg();

// Dialog Data
	enum { IDD = IDD_TERM };
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCodeC;
	CString m_strCodeE;
	CString m_strJobCF;
	UINT m_uJobEF;
	CString m_strJobCT;
	UINT m_uJobET;
	CString m_strReferDateCF;
	COleDateTime m_dtReferDateDF;
	CString m_strReferDateCT;
	COleDateTime m_dtReferDateDT;
	CString m_strCompanyC;
	CString m_strCompanyE;
	CString m_strTaskC;
	CString m_strTaskE;
	CString m_strEditionCF;
	UINT m_uEditionEF;
	CString m_strEditionCT;
	UINT m_uEditionET;
	CString m_strSizeC;
	CString m_strSizeE;
	CString m_strPaperFmtC;
	CString m_strPaperFmtE;
	CString m_strPaperNumCF;
	UINT m_uPaperNumEF;
	CString m_strPaperNumCT;
	UINT m_uPaperNumET;
	CString m_strPrintFmtC;
	CString m_strPrintFmtE;
	CString m_strColorC;
	CString m_strColorE;
	CString m_strPlastNumCF;
	UINT m_uPlastNumEF;
	CString m_strPlastNumCT;
	UINT m_uPlastNumET;
	CString m_strNoteC;
	CString m_strNoteE;
	int m_iCode;
	int m_iJob;
	int m_iReferDate;
	int m_iCompany;
	int m_iTask;
	int m_iEdition;
	int m_iSize;
	int m_iPaperFmt;
	int m_iPaperNum;
	int m_iPrintFmt;
	int m_iColor;
	int m_iPlastNum;
	int m_iNote;
	BOOL m_bCode;
	BOOL m_bJob;
	BOOL m_bReferDate;
	BOOL m_bCompany;
	BOOL m_bTask;
	BOOL m_bEdition;
	BOOL m_bSize;
	BOOL m_bPaperFmt;
	BOOL m_bPaperNum;
	BOOL m_bPrintFmt;
	BOOL m_bColor;
	BOOL m_bPlastNum;
	BOOL m_bNote;
};
