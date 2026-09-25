
#pragma once

#include "User.h"

// CUserDlg dialog

class CUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserDlg)

public:
	CUserDlg(CUserPtr pUser=NULL, CWnd* pParent=NULL);   // standard constructor
	virtual ~CUserDlg();

// Dialog Data
	enum { IDD = IDD_USER };

	CUserPtr m_pUser;

	int m_nRole;
	int m_nWork;
	CString m_strLogin;
	CString m_strPass;
	CString m_strPassRe;
	CString m_strLName;
	CString m_strFName;
	CString m_strMName;

	CComboBox m_cbRole;
	CComboBox m_cbWork;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCbRole();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
