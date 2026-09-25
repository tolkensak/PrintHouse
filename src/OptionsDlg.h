
#pragma once

// COptionsDlg dialog

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONS };
	CString m_strServer;
	BOOL m_bAutoReload;
	UINT m_uReloadSpace;
	CEdit m_ebReloadSpace;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkAutoReload();

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
