// TaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "TaskDlg.h"
#include "JobDlg.h"


CWorkValue::CWorkValue()
	: m_uID(0)
	, m_strName(_T(""))
{
}

CWorkValue::CWorkValue(UINT uID, LPCTSTR pcName)
	: m_uID(uID)
	, m_strName(pcName)
{
}

CWorkValue::CWorkValue(CWorkValue& value)
{
	operator=(value);
}

CWorkValue& CWorkValue::operator=(CWorkValue& value)
{
	m_uID=value.m_uID;
	m_strName=value.m_strName;

	return *this;
}

CWork::CWork()
	: m_uID(0)
	, m_strName(_T(""))
{
	m_Values.SetSize(0);
}

CWork::CWork(UINT uID, LPCTSTR pcName)
	: m_uID(uID)
	, m_strName(pcName)
{
	m_Values.SetSize(0);
}

CWork::CWork(CWork& work)
{
	operator=(work);
}

CWork& CWork::operator=(CWork& work)
{
	m_uID=work.m_uID;
	m_strName=work.m_strName;

	int n=(int)work.m_Values.GetCount();
	m_Values.SetSize(n);

	for(n--; n>=0; n--)
		m_Values[n]=work.m_Values[n];

	return *this;
}

CJobValue::CJobValue()
	: m_uWID(0)
	, m_uVID(0)
{
}

CJobValue::CJobValue(UINT uWID, UINT uVID)
	: m_uWID(uWID)
	, m_uVID(uVID)
{
}

CJobValue::CJobValue(CJobValue& value)
{
	operator=(value);
}

CJobValue& CJobValue::operator=(CJobValue& value)
{
	m_uWID=value.m_uWID;
	m_uVID=value.m_uVID;

	return *this;
}

CJob::CJob()
	: m_strSize(_T(""))
	, m_strPaperFmt(_T(""))
	, m_uPaperNum(0)
	, m_strPrintFmt(_T(""))
	, m_strColor(_T(""))
	, m_uPlastNum(0)
	, m_strPrior(_T("NULL"))
	, m_strSID(_T("DEFAULT"))
{
	m_Values.SetSize(0);
}

CJob::CJob(CJob& job)
{
	operator=(job);
}

CJob& CJob::operator=(CJob& job)
{
	m_strSize=job.m_strSize;
	m_strPaperFmt=job.m_strPaperFmt;
	m_uPaperNum=job.m_uPaperNum;
	m_strPrintFmt=job.m_strPrintFmt;
	m_strColor=job.m_strColor;
	m_uPlastNum=job.m_uPlastNum;
	m_strPrior=job.m_strPrior;
	m_strSID=job.m_strSID;

	int n=(int)job.m_Values.GetCount();
	m_Values.SetSize(n);

	for(n--; n>=0; n--)
		m_Values[n]=job.m_Values[n];

	return *this;
}

CCompany::CCompany(CString strName)
	: m_strName(strName)
{
	m_Tasks.SetSize(0);
}

CCompany::CCompany(CCompany& com)
{
	operator=(com);
}

CCompany& CCompany::operator=(CCompany& com)
{
	m_strName=com.m_strName;

	int i=(int)com.m_Tasks.GetCount();

	m_Tasks.SetSize(i);

	for(i--; i>=0; i--)
		m_Tasks[i]=com.m_Tasks[i];

	return *this;
}

// CTaskDlg dialog

IMPLEMENT_DYNAMIC(CTaskDlg, CDialog)

CTaskDlg::CTaskDlg(UINT uTID, CWnd* pParent /*=NULL*/)
	: CDialog(CTaskDlg::IDD, pParent)
	, m_uTID(uTID)
{
	m_Works.SetSize(0);
	m_Companies.SetSize(0);
	Reset();
}

void CTaskDlg::Reset()
{
	m_strCode=_T("");
	m_strCompany=_T("");
	m_strTask=_T("");
	m_dtReferDate=COleDateTime::GetCurrentTime();
	m_uEdition=0;
	m_strNote=_T("");
	m_nCurJob=0;
	m_Jobs.SetSize(0);
}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_CODE, m_strCode);
	DDX_CBString(pDX, IDC_CB_COMPANY, m_strCompany);
	DDX_CBString(pDX, IDC_CB_TASK, m_strTask);
	DDX_DateTimeCtrl(pDX, IDC_DT_REFER_DATE, m_dtReferDate);
	DDX_Text(pDX, IDC_EB_EDITION, m_uEdition);
	DDV_MinMaxUInt(pDX, m_uEdition, 0, 1000000000);
	DDX_Text(pDX, IDC_EB_NOTE, m_strNote);
	DDX_Control(pDX, IDC_TAB_JOBS, m_tabJobs);
	DDX_Control(pDX, IDC_CB_COMPANY, m_cbCompany);
	DDX_Control(pDX, IDC_CB_TASK, m_cbTask);
	DDX_Control(pDX, IDC_BTN_HISTORY, m_btnHistory);
	DDX_Control(pDX, IDC_BTN_DEL_JOB, m_btnDelJob);
	DDX_Control(pDX, IDC_LB_VALUES, m_lbValues);
}


BEGIN_MESSAGE_MAP(CTaskDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_JOBS, OnTcnSelchangeTabJobs)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_UP, OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBnClickedBtnDown)
	ON_MESSAGE(WM_ADD_JOB, OnAddJob)
	ON_BN_CLICKED(IDC_BTN_DEL_JOB, OnBnClickedBtnDelJob)
	ON_CBN_SELCHANGE(IDC_CB_COMPANY, OnCbnSelchangeCbCompany)
	ON_CBN_EDITCHANGE(IDC_CB_COMPANY, OnCbnEditchangeCbCompany)
	ON_BN_CLICKED(IDC_BTN_HISTORY, OnBnClickedBtnHistory)
	ON_BN_CLICKED(IDC_BTN_DUP_JOB, OnBnClickedBtnDupJob)
END_MESSAGE_MAP()


// CTaskDlg message handlers

void CTaskDlg::GetWorks()
{
	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LPCSTR pcQuery="SELECT w.wid, w.name, v.vid, v.name FROM `work` w LEFT JOIN `value` v ON w.wid=v.wid WHERE w.wid>1 ORDER BY 2, 4";
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i;
	PULONG len;
	MYSQL_ROW row;
	UINT uWID, uPrevWID=0;
	TCHAR pc[TOL_MAXSTR];
	CWorkValueArray* pValues=NULL;
	CWorkPtr pWork=NULL;

	while(row=pRes->FetchRow())
	{
		len=pRes->FetchLengths();

		uWID=atoi(row[0]);

		if(uWID!=uPrevWID)
		{
			MultiByteToWideChar(65001, 0, row[1], len[1]+1, pc, TOL_MAXSTR);
			pWork=new CWork(uWID, pc);
			pValues=&pWork->m_Values;
			i=(int)m_Works.Add(pWork);
			uPrevWID=uWID;
		}

		if(pValues && row[2])
		{
			MultiByteToWideChar(65001, 0, row[3], len[3]+1, pc, TOL_MAXSTR);
			pValues->Add((CWorkValuePtr)new CWorkValue(atoi(row[2]), pc));
		}
	}
}

void CTaskDlg::GetCompanies()
{
	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LPCSTR pcQuery="SELECT DISTINCT t.company, t.name FROM `taskh` t ORDER BY 1, 2";
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i;
	PULONG len;
	MYSQL_ROW row;
	CString str, strPrevTID;
	TCHAR pc[TOL_MAXSTR];
	CStringArray* pTasks=NULL;
	CCompanyPtr pCompany;

	while(row=pRes->FetchRow())
	{
		len=pRes->FetchLengths();

		MultiByteToWideChar(65001, 0, row[0], len[0]+1, pc, TOL_MAXSTR);
		str=pc;

		if(str!=strPrevTID)
		{
			pCompany=new CCompany(str);
			i=(int)m_Companies.Add(pCompany);
			pTasks=&m_Companies[i]->m_Tasks;
			strPrevTID=str;
			m_cbCompany.AddString(str);
		}

		if(pTasks)
		{
			MultiByteToWideChar(65001, 0, row[1], len[1]+1, pc, TOL_MAXSTR);
			pTasks->Add(pc);
		}
	}
}

void CTaskDlg::GetJobs()
{
	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	sprintf(pcQuery, "SELECT t.code, t.company, t.name, DATE_FORMAT(t.refer_date, '%s'), t.edition, t.note, j.jid, j.size, j.paper_fmt, j.paper_num, j.print_fmt, j.color, j.plast_num, j.prior, j.sid, p.wid, p.vid FROM task AS t LEFT JOIN job AS j ON j.tid=t.tid LEFT JOIN proc AS p ON p.tid=j.tid AND p.jid=j.jid WHERE t.tid=%d ORDER BY j.prior, j.jid, p.ord", APP_SQL_DATE_FORMAT, m_uTID);
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i, j;
	CString str;
	PULONG len;
	MYSQL_ROW row;
	UINT uJID, uPrevJID=0;
	TCHAR pc[TOL_MAXSTR];
	CJobPtr pJob;
	CJobValueArray* pValues=NULL;

	for(i=0; row=pRes->FetchRow(); i++)
	{
		len=pRes->FetchLengths();

		if(i==0)
		{
			if(len[0])
			{
				MultiByteToWideChar(65001, 0, row[0], len[0]+1, pc, TOL_MAXSTR);
				m_strCode=pc;
			}

			if(len[1])
			{
				MultiByteToWideChar(65001, 0, row[1], len[1]+1, pc, TOL_MAXSTR);
				m_strCompany=pc;
			}

			if(len[2])
			{
				MultiByteToWideChar(65001, 0, row[2], len[2]+1, pc, TOL_MAXSTR);
				m_strTask=pc;
			}

			if(len[3])
			{
				MultiByteToWideChar(65001, 0, row[3], len[3]+1, pc, TOL_MAXSTR);
				m_dtReferDate.ParseDateTime(pc, VAR_DATEVALUEONLY);
			}

			if(len[4])
				m_uEdition=atoi(row[4]);

			if(len[5])
			{
				MultiByteToWideChar(65001, 0, row[5], len[5]+1, pc, TOL_MAXSTR);
				m_strNote=pc;
			}
		}

		if(!len[6])
			continue;

		uJID=atoi(row[6]);
		if(uJID!=uPrevJID)
		{
			pJob=new CJob;
			pValues=&pJob->m_Values;
			j=(int)m_Jobs.Add(pJob);

			if(len[7])
			{
				MultiByteToWideChar(65001, 0, row[7], len[7]+1, pc, TOL_MAXSTR);
				pJob->m_strSize=pc;
			}

			if(len[8])
			{
				MultiByteToWideChar(65001, 0, row[8], len[8]+1, pc, TOL_MAXSTR);
				pJob->m_strPaperFmt=pc;
			}

			if(len[9])
				pJob->m_uPaperNum=atoi(row[9]);

			if(len[10])
			{
				MultiByteToWideChar(65001, 0, row[10], len[10]+1, pc, TOL_MAXSTR);
				pJob->m_strPrintFmt=pc;
			}

			if(len[11])
			{
				MultiByteToWideChar(65001, 0, row[11], len[11]+1, pc, TOL_MAXSTR);
				pJob->m_strColor=pc;
			}

			if(len[12])
				pJob->m_uPlastNum=atoi(row[12]);

			pJob->m_strPrior=row[13];
			pJob->m_strSID=row[14];

			str.Format(IDS_TAB_JOB, j+1);
			m_tabJobs.InsertItem(j, str);
			uPrevJID=uJID;
		}

		if(pValues && len[15] && len[16])
		{
			CJobValuePtr p=new CJobValue(atoi(row[15]), atoi(row[16]));
			pValues->Add(p);
		}
	}
}

BOOL CTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int i=0;
	CString str;

	GetWorks();
	GetCompanies();

	if(m_uTID)
	{
		str.LoadString(IDD_TASK);
		SetWindowText(str);

		m_btnHistory.EnableWindow(0);

		GetJobs();
		i=(int)m_Jobs.GetCount();
	}

	if(!i)
	{
		CJobPtr p=new CJob;
		m_Jobs.InsertAt(0, p);
		str.Format(IDS_TAB_JOB, 1);
		m_tabJobs.InsertItem(0, str);
		i=1;
	}

	LoadJob(0);

	str.LoadString(IDS_TAB_ADD);
	m_tabJobs.InsertItem(i, str);

	m_btnDelJob.EnableWindow(i>1);
	UpdateData(FALSE);
	return TRUE;
}

void CTaskDlg::SaveJob(int iJob)
{
	if(iJob<0)
		return;

	CJobPtr pJob=m_Jobs[iJob];
	GetDlgItemText(IDC_EB_SIZE, pJob->m_strSize);
	GetDlgItemText(IDC_EB_PAPER_FMT, pJob->m_strPaperFmt);
	pJob->m_uPaperNum=GetDlgItemInt(IDC_EB_PAPER_NUM, 0, 0);
	GetDlgItemText(IDC_EB_PRINT_FMT, pJob->m_strPrintFmt);
	GetDlgItemText(IDC_EB_COLOR, pJob->m_strColor);
	pJob->m_uPlastNum=GetDlgItemInt(IDC_EB_PALST_NUM, 0, 0);
}

void CTaskDlg::LoadJob(int iJob)
{
	CJobPtr pJob=m_Jobs[iJob];
	SetDlgItemText(IDC_EB_SIZE, pJob->m_strSize);
	SetDlgItemText(IDC_EB_PAPER_FMT, pJob->m_strPaperFmt);
	SetDlgItemInt(IDC_EB_PAPER_NUM, pJob->m_uPaperNum, 0);
	SetDlgItemText(IDC_EB_PRINT_FMT, pJob->m_strPrintFmt);
	SetDlgItemText(IDC_EB_COLOR, pJob->m_strColor);
	SetDlgItemInt(IDC_EB_PALST_NUM, pJob->m_uPlastNum, 0);

	m_lbValues.ResetContent();
	m_lbValues.SetCurSel(-1);

	CJobValuePtr pJobValue;
	CWorkValueArray* pWorkValues;
	CJobValueArray* pJobValues=&pJob->m_Values;

	CString str;
	int i, n=(int)pJobValues->GetCount();
	int ii, jj, mm, nn=(int)m_Works.GetCount();

	for(i=0; i<n; i++)
	{
		pJobValue=pJobValues->GetAt(i);

		for(ii=0; ii<nn; ii++)
		{
			if(m_Works[ii]->m_uID==pJobValue->m_uWID)
			{
				pWorkValues=&m_Works[ii]->m_Values;

				mm=(int)pWorkValues->GetCount();
				for(jj=0; jj<mm; jj++)
				{
					if(pWorkValues->GetAt(jj)->m_uID==pJobValue->m_uVID)
					{
						str.Format(_T("%s [%s]"), m_Works[ii]->m_strName, pWorkValues->GetAt(jj)->m_strName);
						m_lbValues.InsertString(i, str);
						break;
					}
				}
			}
		}
	}
}

void CTaskDlg::OnBnClickedBtnDelJob()
{
	CString str;
	int n=m_tabJobs.GetItemCount();

	if(n>2)
	{
		int i=m_tabJobs.GetCurSel();

		m_nCurJob=i?i-1:1;
		m_tabJobs.SetCurSel(m_nCurJob);
		LoadJob(m_nCurJob);
		m_nCurJob=-1;

		m_tabJobs.DeleteItem(i);
		m_Jobs.RemoveAt(i);

		TCITEM tci;
		tci.mask=TCIF_TEXT;
		for(i=0; i<n-2; i++)
		{
			str.Format(IDS_TAB_JOB, i+1);
			tci.pszText=str.GetBuffer();
			m_tabJobs.SetItem(i, &tci);
		}

		m_btnDelJob.EnableWindow(n>3);
	}
}

void CTaskDlg::InsertJob(int i, CJobPtr pJob)
{
	CJobPtr p;
	CString str;
	str.Format(IDS_TAB_JOB, i+1);

	if(pJob)
	{
		p=new CJob(*pJob);
		p->m_strPrior=_T("NULL");
		p->m_strSID=_T("DEFAULT");
	}
	else
		p=new CJob;

	m_Jobs.InsertAt(i, p);
	m_tabJobs.InsertItem(i, str);
	m_tabJobs.SetCurSel(i);

	m_btnDelJob.EnableWindow(i>0);
}

void CTaskDlg::OnBnClickedBtnDupJob()
{
	int i=m_tabJobs.GetCurSel();

	SaveJob(i);
	InsertJob(i+1, m_Jobs[i]);
	m_nCurJob=i+1;

	CString str;
	TCITEM tci;
	tci.mask=TCIF_TEXT;

	int n=m_tabJobs.GetItemCount()-1;
	for(i++; i<n; i++)
	{
		str.Format(IDS_TAB_JOB, i+1);
		tci.pszText=str.GetBuffer();
		m_tabJobs.SetItem(i, &tci);
	}
}

void CTaskDlg::OnTcnSelchangeTabJobs(NMHDR *pNMHDR, LRESULT *pResult)
{
	int i=m_tabJobs.GetCurSel();
	int n=m_tabJobs.GetItemCount();

	if(i==n-1)
		InsertJob(i);

	if(i!=m_nCurJob)
	{
		SaveJob(m_nCurJob);
		LoadJob(i);
		m_nCurJob=i;
	}

	*pResult = 0;
}

LRESULT CTaskDlg::OnAddJob(WPARAM wParam, LPARAM lParam)
{
	CWorkPtr pWork=m_Works[wParam];
	CWorkValuePtr pWorkValue=pWork->m_Values[lParam];
	CJobValuePtr pJobValue=new CJobValue(pWork->m_uID, pWorkValue->m_uID);
	CJobValueArray* pJobValues=&m_Jobs.GetAt(m_nCurJob)->m_Values;

	CString str;
	str.Format(_T("%s [%s]"), pWork->m_strName, pWorkValue->m_strName);

	int i=m_lbValues.GetCurSel();

	if(i<0)
	{
		pJobValues->Add(pJobValue);
		m_lbValues.AddString(str);
	}
	else
	{
		pJobValues->InsertAt(i, pJobValue);
		m_lbValues.InsertString(i, str);
	}

	return 0;
}

void CTaskDlg::OnBnClickedBtnAdd()
{
	CJobDlg dlg(&m_Works);
	dlg.DoModal();
}

void CTaskDlg::OnBnClickedBtnDel()
{
	CJobValueArray* pJobValues=&m_Jobs.GetAt(m_nCurJob)->m_Values;

	int i=m_lbValues.GetCurSel();
	if(i<0)
		return;

	pJobValues->RemoveAt(i);
	m_lbValues.DeleteString(i);
}

void CTaskDlg::OnBnClickedBtnUp()
{
	int i=m_lbValues.GetCurSel();
	if(i<=0)
		return;

	CJobValueArray* pValues=&m_Jobs.GetAt(m_nCurJob)->m_Values;
	CJobValuePtr value=pValues->GetAt(i);

	pValues->RemoveAt(i);
	pValues->InsertAt(i-1, value);

	CString str;
	m_lbValues.GetText(i, str);
	m_lbValues.DeleteString(i);
	m_lbValues.InsertString(i-1, str);
	m_lbValues.SetCurSel(i-1);
}

void CTaskDlg::OnBnClickedBtnDown()
{
	int i=m_lbValues.GetCurSel();
	if(i>=m_lbValues.GetCount()-1)
		return;

	CJobValueArray* pValues=&m_Jobs.GetAt(m_nCurJob)->m_Values;
	CJobValuePtr pValue=pValues->GetAt(i);

	pValues->RemoveAt(i);
	pValues->InsertAt(i+1, pValue);

	CString str;
	m_lbValues.GetText(i, str);
	m_lbValues.DeleteString(i);
	m_lbValues.InsertString(i+1, str);
	m_lbValues.SetCurSel(i+1);
}

BOOL CTaskDlg::ValidateJobs()
{
	CJobPtr pJob;
	int n=(int)m_Jobs.GetCount();

	for(int i=0; i<n; i++)
	{
		pJob=m_Jobs[i];
		if(pJob->m_Values.GetCount()==0)
		{
			CString str;
			str.Format(IDS_QST_JOB_DATA_NOT_ENOUGH, i+1);
			if(theApp.MessageBox(str, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
				return FALSE;
		}
	}

	return TRUE;
}

void CTaskDlg::OnOK()
{
	if(!UpdateData())
		return;

	m_strCompany.Trim();
	if(m_strCompany.IsEmpty())
	{
		theApp.FieldInfo(IDC_CB_COMPANY, this);
		return;
	}

	m_strTask.Trim();
	if(m_strTask.IsEmpty())
	{
		theApp.FieldInfo(IDC_CB_TASK, this);
        return;
	}

	m_strCode.Trim();
	if(m_strCode.IsEmpty())
	{
		theApp.FieldInfo(IDC_EB_CODE, this);
		return;
	}

	SaveJob(m_nCurJob);
	if(!ValidateJobs())
		return;

	CHAR pc1[TOL_MAXSTR],
		pc2[TOL_MAXSTR],
		pc3[TOL_MAXSTR],
		pc4[TOL_MAXSTR];

	CString strReferDate=m_dtReferDate.Format(_T(APP_SQL_DATE_FORMAT));

	WideCharToMultiByte(65001, 0, m_strCode, m_strCode.GetLength()+1, pc1, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strCompany, m_strCompany.GetLength()+1, pc2, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strTask, m_strTask.GetLength()+1, pc3, TOL_MAXSTR, NULL, NULL);
	WideCharToMultiByte(65001, 0, m_strNote, m_strNote.GetLength()+1, pc4, TOL_MAXSTR, NULL, NULL);


	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	pConn->AutoCommit(FALSE);

	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	if(m_uTID)
	{
		sprintf(pcQuery, "DELETE FROM `proc` WHERE tid=%d", m_uTID);
		if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		{
			pConn->Rollback();
			return;
		}

		sprintf(pcQuery, "DELETE FROM `job` WHERE tid=%d", m_uTID);
		if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		{
			pConn->Rollback();
			return;
		}

		sprintf(pcQuery, "REPLACE INTO task(tid, code, company, name, refer_date, edition, note) VALUES(%d, \'%s\', \'%s\', \'%s\', \'%S\', %d, \'%s\')", m_uTID, pc1, pc2, pc3, strReferDate, m_uEdition, pc4);
		if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		{
			pConn->Rollback();
			return;
		}
	}
	else
	{
		sprintf(pcQuery, "INSERT INTO task(code, company, name, refer_date, edition, note) VALUES(\'%s\', \'%s\', \'%s\', \'%S\', %d, \'%s\')", pc1, pc2, pc3, strReferDate, m_uEdition, pc4);
		if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		{
			pConn->Rollback();
			return;
		}

		m_uTID=(UINT)pConn->InsertId();
	}

	int i, j, n, m;
	CJobPtr pJob;
	CJobValuePtr pValue;
	CJobValueArray* pValues;

	n=m_tabJobs.GetItemCount()-1;
	for(i=0; i<n; i++)
	{
		pJob=m_Jobs[i];

		WideCharToMultiByte(65001, 0, pJob->m_strSize, pJob->m_strSize.GetLength()+1, pc1, TOL_MAXSTR, NULL, NULL);
		WideCharToMultiByte(65001, 0, pJob->m_strPaperFmt, pJob->m_strPaperFmt.GetLength()+1, pc2, TOL_MAXSTR, NULL, NULL);
		WideCharToMultiByte(65001, 0, pJob->m_strPrintFmt, pJob->m_strPrintFmt.GetLength()+1, pc3, TOL_MAXSTR, NULL, NULL);
		WideCharToMultiByte(65001, 0, pJob->m_strColor, pJob->m_strColor.GetLength()+1, pc4, TOL_MAXSTR, NULL, NULL);

		sprintf(pcQuery, "INSERT INTO job(tid, jid, size, paper_fmt, paper_num, print_fmt, color, plast_num, prior, sid) VALUES(%d, %d, \'%s\', \'%s\', %d, \'%s\', \'%s\', %d, %S, %S)", m_uTID, i+1, pc1, pc2, pJob->m_uPaperNum, pc3, pc4, pJob->m_uPlastNum, pJob->m_strPrior, pJob->m_strSID);
		if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		{
			pConn->Rollback();
			return;
		}

		pValues=&pJob->m_Values;
		m=(int)pValues->GetCount();

		for(j=0; j<m; j++)
		{
			pValue=pValues->GetAt(j);

			sprintf(pcQuery, "INSERT INTO proc(tid, jid, wid, vid, ord) VALUES(%d, %d, %d, %d, %d)", m_uTID, i+1, pValue->m_uWID, pValue->m_uVID, j+1);
			if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
			{
				pConn->Rollback();
				return;
			}
		}
	}


	pConn->Commit();
	pConn->AutoCommit(TRUE);

	CDialog::OnOK();
}

void CTaskDlg::OnCbnSelchangeCbCompany()
{
	m_cbTask.ResetContent();

	CString str;
	m_cbCompany.GetLBText(m_cbCompany.GetCurSel(), str);

	int n=(int)m_Companies.GetCount();
	for(int i=0; i<n; i++)
		if(m_Companies[i]->m_strName==str)
		{
			CStringArray* pTasks=&m_Companies[i]->m_Tasks;

			n=(int)pTasks->GetCount();
			for(i=0; i<n; i++)
				m_cbTask.AddString(pTasks->GetAt(i));

			break;
		}
}

void CTaskDlg::OnCbnEditchangeCbCompany()
{
	CString str;
	m_cbCompany.GetWindowText(str);

	int i=m_cbCompany.FindStringExact(-1, str);
	if(i>=0)
	{
		m_cbCompany.SetCurSel(i);
		OnCbnSelchangeCbCompany();
	}
}

void CTaskDlg::GetJobsHistory()
{
	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	CString str;
	CHAR pcCompany[TOL_MAXSTR];
	CHAR pcTask[TOL_MAXSTR];

	m_cbCompany.GetWindowText(str);
	WideCharToMultiByte(65001, 0, str, str.GetLength()+1, pcCompany, TOL_MAXSTR, NULL, NULL);

	m_cbTask.GetWindowText(str);
	WideCharToMultiByte(65001, 0, str, str.GetLength()+1, pcTask, TOL_MAXSTR, NULL, NULL);

	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	sprintf(pcQuery, "SELECT t.code, t.company, t.name, DATE_FORMAT(t.refer_date, '%s'), t.edition, t.note, j.jid, j.size, j.paper_fmt, j.paper_num, j.print_fmt, j.color, j.plast_num, p.wid, p.vid FROM taskh AS t LEFT JOIN jobh AS j ON j.tid=t.tid LEFT JOIN proch AS p ON p.tid=j.tid AND p.jid=j.jid WHERE t.company=\'%s\' AND t.name=\'%s\' ORDER BY j.jid, j.jid, p.ord", APP_SQL_DATE_FORMAT, pcCompany, pcTask);
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i, j;
	PULONG len;
	MYSQL_ROW row;
	UINT uJID, uPrevJID=0;
	TCHAR pc[TOL_MAXSTR];
	CJobPtr pJob;
	CJobValueArray* pValues=NULL;

	for(i=0; row=pRes->FetchRow(); i++)
	{
		len=pRes->FetchLengths();

		if(i==0)
		{
			if(len[0])
			{
				MultiByteToWideChar(65001, 0, row[0], len[0]+1, pc, TOL_MAXSTR);
				m_strCode=pc;
			}

			if(len[1])
			{
				MultiByteToWideChar(65001, 0, row[1], len[1]+1, pc, TOL_MAXSTR);
				m_strCompany=pc;
			}

			if(len[2])
			{
				MultiByteToWideChar(65001, 0, row[2], len[2]+1, pc, TOL_MAXSTR);
				m_strTask=pc;
			}

			if(len[3])
			{
				MultiByteToWideChar(65001, 0, row[3], len[3]+1, pc, TOL_MAXSTR);
				m_dtReferDate.ParseDateTime(pc, VAR_DATEVALUEONLY);
			}

			if(len[4])
			{
				m_uEdition=atoi(row[4]);
			}

			if(len[5])
			{
				MultiByteToWideChar(65001, 0, row[5], len[5]+1, pc, TOL_MAXSTR);
				m_strNote=pc;
			}
		}

		if(!len[6])
			continue;

		uJID=atoi(row[6]);
		if(uJID!=uPrevJID)
		{
			pJob=new CJob();
			pValues=&pJob->m_Values;
			j=(int)m_Jobs.Add(pJob);

			if(len[7])
			{
				MultiByteToWideChar(65001, 0, row[7], len[7]+1, pc, TOL_MAXSTR);
				pJob->m_strSize=pc;
			}

			if(len[8])
			{
				MultiByteToWideChar(65001, 0, row[8], len[8]+1, pc, TOL_MAXSTR);
				pJob->m_strPaperFmt=pc;
			}

			if(len[9])
				pJob->m_uPaperNum=atoi(row[9]);

			if(len[10])
			{
				MultiByteToWideChar(65001, 0, row[10], len[10]+1, pc, TOL_MAXSTR);
				pJob->m_strPrintFmt=pc;
			}

			if(len[11])
			{
				MultiByteToWideChar(65001, 0, row[11], len[11]+1, pc, TOL_MAXSTR);
				pJob->m_strColor=pc;
			}

			if(len[12])
				pJob->m_uPlastNum=atoi(row[12]);

			str.Format(IDS_TAB_JOB, j+1);
			m_tabJobs.InsertItem(j, str);
			uPrevJID=uJID;
		}

		if(pValues && len[13] && len[14])
		{
			CJobValuePtr p=new CJobValue(atoi(row[13]), atoi(row[14]));
			pValues->Add(p);
		}
	}
}

void CTaskDlg::OnBnClickedBtnHistory()
{
	Reset();
	m_tabJobs.DeleteAllItems();

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	int i=0;
	CString str;

	GetJobsHistory();

	i=(int)m_Jobs.GetCount();
	if(i)
		LoadJob(0);
	else
	{
		i=1;
		m_Jobs.SetSize(1);
		str.Format(IDS_TAB_JOB, 1);
		m_tabJobs.InsertItem(0, str);
	}

	str.LoadString(IDS_TAB_ADD);
	m_tabJobs.InsertItem(i, str);

	m_btnDelJob.EnableWindow(i>1);

	UpdateData(FALSE);
}
