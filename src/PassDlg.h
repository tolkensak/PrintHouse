#pragma once


// CPassDlg dialog

class CPassDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassDlg();

	enum { IDD = IDD_PASS };
	CString m_strPassOld;
	CString m_strPass;
	CString m_strPassRe;

	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
