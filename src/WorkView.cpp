// WorkView.cpp : implementation of the CWorkView class
//

#include "stdafx.h"
#include "App.h"
#include "WorkView.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWorkView

CWorkView::CWorkView()
{
	SetEmptyStr(IDS_EMPTY_TEXT_WORK);
	m_Jobs.SetSize(0);
}

CWorkView::~CWorkView()
{
}


BEGIN_MESSAGE_MAP(CWorkView, CDummyView)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_COMMAND(ID_FILE_INFO, OnFileInfo)
	ON_COMMAND(ID_VIEW_RELOAD, OnViewReload)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_INFO, OnUpdateFileInfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RELOAD, OnUpdateViewReload)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_COMMAND_RANGE(1, 99, OnToggleCol)
END_MESSAGE_MAP()

// CWorkView message handlers

void CWorkView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelChanged();
	*pResult = 0;
}

void CWorkView::Reset()
{
	Reload();
}

void CWorkView::Erase()
{
	DeleteAllItems();
	m_Jobs.RemoveAll();
	SelChanged();
}

void CWorkView::Reload(BOOL bKeepState)
{
	if(theApp.m_sess.m_uUID==0)
	{
		Erase();
		return;
	}

	CTaskJobArray stas;
	if(bKeepState)
		SaveState(stas);

	Erase();
	FillData();

	if(bKeepState)
		LoadState(stas);
	SelChanged();
}

void CWorkView::SelChanged()
{
	if(GetSelectedCount()==1)
	{
		int j=GetNextItem(-1, LVNI_SELECTED);
		int n=(int)m_Jobs.GetCount();
		for(int i=0; i<n; i++)
			if(m_Jobs[i]->m_lParam==j)
			{
				CTaskJobPtr p=new CTaskJob(*m_Jobs[i]);
				p->m_lParam=GetItemState(i, LVIS_STATEIMAGEMASK);
				AfxGetMainWnd()->SendMessage(WM_SEL_CHNAGE_WORK_LIST, m_uID, (LPARAM)(LPVOID)p);
				return;
			}
	}

	AfxGetMainWnd()->SendMessage(WM_SEL_CHNAGE_WORK_LIST, m_uID);
}

void CWorkView::SaveState(CTaskJobArray& jobs)
{
	if(!GetSelectedCount())
		return;

	CTaskJobPtr p;

	TCHAR pc[TOL_MAXSTR];
	LVITEM lvi;
	lvi.pszText=pc;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.stateMask=LVIS_SELECTED;
	lvi.iItem=-1;

	while((lvi.iItem=GetNextItem(lvi.iItem, LVNI_SELECTED))!=-1)
	{
		lvi.iSubItem=0;
		lvi.mask=LVIF_STATE|LVIF_PARAM;
		GetItem(&lvi);

		lvi.iSubItem=2;
		lvi.mask=LVIF_TEXT;
		GetItem(&lvi);

		p=new CTaskJob((UINT)lvi.lParam, _tstoi(pc), lvi.state);
		jobs.Add(p);
	}
}

void CWorkView::LoadState(CTaskJobArray& jobs)
{
	TCHAR pc[TOL_MAXSTR];
	CTaskJobPtr pJob;
	LVITEM lvi;
	lvi.iSubItem=0;
	lvi.mask=LVIF_PARAM;
	lvi.stateMask=LVIS_SELECTED;
	lvi.state=0;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.pszText=pc;

	int i;
	int n=GetItemCount();
	int m=(int)jobs.GetCount();

	for(lvi.iItem=0; lvi.iItem<n; lvi.iItem++)
	{
		GetItem(&lvi);
		for(i=0; i<m; i++)
		{
			pJob=jobs[i];
			if(lvi.lParam==pJob->m_uTID)
			{
				lvi.mask=LVIF_TEXT;
				lvi.iSubItem=2;
				GetItem(&lvi);

				lvi.iSubItem=0;

				if(pJob->m_uJID==_tstoi(pc))
				{
					lvi.mask=LVIF_STATE;
					lvi.state=(UINT)pJob->m_lParam;
					SetItem(&lvi);
				}

				lvi.mask=LVIF_PARAM;
				break;
			}
		}
	}

	//if(n && !lvi.state)
	//{
	//	lvi.mask=LVIF_STATE;
	//	lvi.iItem=0;
	//	lvi.iSubItem=0;
	//	lvi.state=LVIS_SELECTED;
	//	SetItem(&lvi);
	//}
}

void CWorkView::OnFileDelete()
{
	int n=GetSelectedCount();
	if(n==0)
		return;

	if(theApp.MessageBox(IDS_QST_DELETE, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	pConn->AutoCommit(FALSE);

	LVITEM lvi;
	TCHAR pc[TOL_MAXSTR];

	lvi.iItem=-1;
	lvi.pszText=pc;
	lvi.cchTextMax=TOL_MAXSTR;

	while((lvi.iItem=GetNextItem(lvi.iItem, LVNI_SELECTED))!=-1)
	{
		lvi.iSubItem=0;
		lvi.mask=LVIF_PARAM;
		GetItem(&lvi);

		lvi.iSubItem=2;
		lvi.mask=LVIF_TEXT;
		GetItem(&lvi);

		if(DeleteJob(pConn, (UINT)lvi.lParam, _tstoi(pc)))
			pConn->Commit();
		else
		{
			theApp.ConnectionError();
			pConn->Rollback();
		}
	}

	pConn->AutoCommit(TRUE);
	Reload();
}

void CWorkView::OnFileInfo()
{
	if(GetSelectedCount()!=1)
		return;

	LVITEM lvi;
	lvi.mask=LVIF_PARAM;
	lvi.iItem=GetNextItem(-1, LVNI_SELECTED);
	lvi.iSubItem=0;
	GetItem(&lvi);

	CInfoDlg dlg((UINT)lvi.lParam, m_uID);
	dlg.DoModal();
}

void CWorkView::OnViewReload()
{
	Reload();
}

void CWorkView::OnUpdateFileDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CWorkView::OnUpdateFileInfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uWID==1);
}

void CWorkView::OnUpdateViewReload(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uUID);
}
