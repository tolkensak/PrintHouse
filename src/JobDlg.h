#pragma once

#include "TaskDlg.h"
#include "afxwin.h"

// CJobDlg dialog

class CJobDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobDlg)

public:
	CJobDlg(CWorkArray* pWorks, CWnd* pParent = NULL);   // standard constructor
	virtual ~CJobDlg();

// Dialog Data
	enum { IDD = IDD_JOB };
	CListBox m_lbWork;
	CListBox m_lbValue;

	CWorkArray* m_pWorks;

	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeLbWork();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
