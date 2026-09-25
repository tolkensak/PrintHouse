
#pragma once

#include "User.h"

// CUsersDlg dialog

class CUsersDlg : public CDialog
{
	DECLARE_DYNAMIC(CUsersDlg)

public:
	CUsersDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUsersDlg();

	enum { IDD = IDD_USERS };

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnNMDblclkLcUsers(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	int m_nColNum;
	CListCtrl m_lcUsers;
	CUserArray m_Users;

	void Reload();
	void EditUser(UINT uUID);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){};
	DECLARE_MESSAGE_MAP()
};
