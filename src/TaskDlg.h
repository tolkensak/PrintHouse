
#pragma once

// CWorkValue class

class CWorkValue : public SmartObject
{
public:
	CWorkValue();
	CWorkValue(UINT uID, LPCTSTR pcName);
	CWorkValue(CWorkValue& value);
	CWorkValue& operator=(CWorkValue& value);

	UINT m_uID;
	CString m_strName;
};

typedef SmartPointer<CWorkValue> CWorkValuePtr;
typedef	CArray<CWorkValuePtr, CWorkValuePtr&> CWorkValueArray;

// CWork class

class CWork : public SmartObject
{
public:
	CWork();
	CWork(UINT uID, LPCTSTR pcName);
	CWork(CWork& work);
	CWork& operator=(CWork& work);

	UINT m_uID;
	CString m_strName;

	CWorkValueArray m_Values;
};

typedef SmartPointer<CWork> CWorkPtr;
typedef	CArray<CWorkPtr, CWorkPtr&> CWorkArray;

// CJobValue class

class CJobValue : public SmartObject
{
public:
	CJobValue();
	CJobValue(UINT uWID, UINT uVID);
	CJobValue(CJobValue& value);
	CJobValue& operator=(CJobValue& value);

	UINT m_uWID;
	UINT m_uVID;
};

typedef SmartPointer<CJobValue> CJobValuePtr;
typedef	CArray<CJobValuePtr, CJobValuePtr&> CJobValueArray;

// CJob class

class CJob : public SmartObject
{
public:
	CJob();
	CJob(CJob& job);
	CJob& operator=(CJob& job);

	CString m_strSize;
	CString m_strPaperFmt;
	UINT m_uPaperNum;
	CString m_strPrintFmt;
	CString m_strColor;
	UINT m_uPlastNum;

	CString m_strPrior;
	CString m_strSID;

	CJobValueArray m_Values;
};

typedef SmartPointer<CJob> CJobPtr;
typedef	CArray<CJobPtr, CJobPtr&> CJobArray;


// CCompany class

class CCompany : public SmartObject
{
public:
	CCompany(CString strName=_T(""));
	CCompany(CCompany& com);
	CCompany& operator=(CCompany& com);

	CString m_strName;
	CStringArray m_Tasks;
};

typedef SmartPointer<CCompany> CCompanyPtr;
typedef	CArray<CCompanyPtr, CCompanyPtr&> CCompanyArray;


// CTaskDlg dialog

class CTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CTaskDlg)

public:
	CTaskDlg(UINT uTID=0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskDlg();

	enum { IDD = IDD_TASK };


	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabJobs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg LRESULT OnAddJob(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnDelJob();
	afx_msg void OnCbnSelchangeCbCompany();
	afx_msg void OnCbnEditchangeCbCompany();
	afx_msg void OnBnClickedBtnHistory();
	afx_msg void OnBnClickedBtnDupJob();

protected:
	CString m_strCode;
	CString m_strCompany;
	CString m_strTask;
	COleDateTime m_dtReferDate;
	UINT m_uEdition;
	CString m_strNote;

	UINT m_uTID;
	int m_nCurJob;
	CJobArray m_Jobs;
	CWorkArray m_Works;
	CCompanyArray m_Companies;
	CTabCtrl m_tabJobs;
	CComboBox m_cbCompany;
	CComboBox m_cbTask;
	CButton m_btnHistory;
	CButton m_btnDelJob;
	CListBox m_lbValues;

	void GetWorks();
	void GetCompanies();
	void GetJobs();
	void GetJobsHistory();
	void Reset();
	void InsertJob(int i, CJobPtr pJob=NULL);
	void LoadJob(int iJob);
	void SaveJob(int iJob);
	BOOL ValidateJobs();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
