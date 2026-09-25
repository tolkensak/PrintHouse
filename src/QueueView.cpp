
#include "stdafx.h"
#include "App.h"
#include "QueueView.h"
#include "TaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CQueueView

CQueueView::CQueueView()
	: m_uTimer(0)
{
	m_uID=ID_VIEW_QUEUE;
}

CQueueView::~CQueueView()
{
}


BEGIN_MESSAGE_MAP(CQueueView, CWorkView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_ADD, OnFileAdd)
	ON_COMMAND(ID_FILE_EDIT, OnFileEdit)
	ON_COMMAND(ID_FILE_START, OnFileStart)
	ON_COMMAND(ID_FILE_STOP, OnFileStop)
	ON_COMMAND(ID_FILE_HISTORY, OnFileHistory)
	ON_COMMAND(ID_FILE_SUBMIT, OnFileSubmit)
	ON_COMMAND_RANGE(ID_PRIOR_TOP, ID_PRIOR_BOTTOM, OnPrior)
	ON_UPDATE_COMMAND_UI(ID_FILE_ADD, OnUpdateFileAdd)
	ON_UPDATE_COMMAND_UI(ID_FILE_EDIT, OnUpdateFileEdit)
	ON_UPDATE_COMMAND_UI(ID_FILE_START, OnUpdateFileStart)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP, OnUpdateFileStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_HISTORY, OnUpdateFileHistory)
	ON_UPDATE_COMMAND_UI(ID_FILE_SUBMIT, OnUpdateFileSubmit)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PRIOR_TOP, ID_PRIOR_BOTTOM, OnUpdatePrior)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


int CQueueView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWorkView::OnCreate(lpCreateStruct) == -1)
		return -1;

	StartTimer();
	return 0;
}

void CQueueView::Reset()
{
	CWorkView::Reset();
	StartTimer();
}

void CQueueView::StartTimer()
{
	if(m_uTimer)
	{
		KillTimer(m_uTimer);
		m_uTimer=0;
	}

	if(theApp.m_bAutoReload && theApp.m_sess.m_uUID)
		m_uTimer=(UINT)SetTimer(1, theApp.m_uReloadSpace*60000, 0);
}

void CQueueView::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==m_uTimer)
		Reload(TRUE);

	CListCtrl::OnTimer(nIDEvent);
}

void CQueueView::OnFileAdd()
{
	CTaskDlg dlg;
	if(dlg.DoModal()==IDOK)
		Reload(TRUE);
}

void CQueueView::EditTask(UINT uTID)
{
	if(!uTID)
		return;

	CTaskDlg dlg(uTID);
	if(dlg.DoModal()==IDOK)
		Reload(TRUE);
}

void CQueueView::OnFileEdit()
{
	if(GetSelectedCount()!=1)
		return;

	LVITEM lvi;
	lvi.mask=LVIF_PARAM;
	lvi.iItem=GetNextItem(-1, LVNI_SELECTED);
	lvi.iSubItem=0;
	GetItem(&lvi);
	EditTask((UINT)lvi.lParam);
}

void CQueueView::OnFileStart()
{
	int n=GetSelectedCount();
	if(n==0)
		return;

	if(theApp.MessageBox(IDS_QST_START, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LVITEM lvi;
	TCHAR pc[TOL_MAXSTR];
	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	CHAR *pcW, pcWhr[TOL_MAXSTR];

	lvi.iItem=-1;
	lvi.pszText=pc;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	pcW=pcWhr;
	*pcW='\0';

	while((lvi.iItem=GetNextItem(lvi.iItem, LVNI_SELECTED))!=-1)
	{
		lvi.iSubItem=0;
		lvi.mask=LVIF_STATE|LVIF_PARAM;
		GetItem(&lvi);

		if(lvi.state==INDEXTOSTATEIMAGEMASK(1)
			|| lvi.state==INDEXTOSTATEIMAGEMASK(3))
		{
			lvi.iSubItem=2;
			lvi.mask=LVIF_TEXT;
			GetItem(&lvi);

			if(pcW!=pcWhr)
				pcW+=sprintf(pcW, " OR ");

			pcW+=sprintf(pcW, "(tid=%d AND jid=%S)", (UINT)lvi.lParam, pc);
		}
	}

	sprintf(pcQuery, "UPDATE `job` SET sid=2 WHERE %s", pcWhr);
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0)
		Reload(TRUE);
}

void CQueueView::OnFileStop()
{
	int n=GetSelectedCount();
	if(n==0)
		return;

	if(theApp.MessageBox(IDS_QST_STOP, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LVITEM lvi;
	TCHAR pc[TOL_MAXSTR];
	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	CHAR *pcW, pcWhr[TOL_MAXSTR];

	lvi.iItem=-1;
	lvi.pszText=pc;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	pcW=pcWhr;
	*pcW='\0';

	while((lvi.iItem=GetNextItem(lvi.iItem, LVNI_SELECTED))!=-1)
	{
		lvi.iSubItem=0;
		lvi.mask=LVIF_STATE|LVIF_PARAM;
		GetItem(&lvi);

		if(lvi.state==INDEXTOSTATEIMAGEMASK(2))
		{
			lvi.iSubItem=2;
			lvi.mask=LVIF_TEXT;
			GetItem(&lvi);

			if(pcW!=pcWhr)
				pcW+=sprintf(pcW, " OR ");

			pcW+=sprintf(pcW, "(tid=%d AND jid=%S)", (UINT)lvi.lParam, pc);
		}
	}

	sprintf(pcQuery, "UPDATE `job` SET sid=3 WHERE %s", pcWhr);
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0)
		Reload(TRUE);
}

void CQueueView::OnFileHistory()
{
	int n=GetSelectedCount();
	if(n==0)
		return;

	if(theApp.MessageBox(IDS_QST_HISTORY, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LVITEM lvi;
	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	lvi.iItem=-1;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	while((lvi.iItem=GetNextItem(lvi.iItem, LVNI_SELECTED))!=-1)
	{
		lvi.iSubItem=0;
		lvi.mask=LVIF_STATE|LVIF_PARAM;
		GetItem(&lvi);

		if(lvi.state==INDEXTOSTATEIMAGEMASK(4))
		{
			//sprintf_s(pcQuery, sizeof(pcQuery), "CALL sp_hist(%d)", (UINT)lvi.lParam);
			sprintf(pcQuery, "CALL sp_hist(%d)", (UINT)lvi.lParam);
			pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery));
		}
	}

	Reload();
}

void CQueueView::OnFileSubmit()
{
	int n=GetSelectedCount();
	if(n==0)
		return;

	if(theApp.MessageBox(IDS_QST_SUBMIT, MB_YESNO|MB_ICONQUESTION, this)!=IDYES)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	LVITEM lvi;
	TCHAR pc[TOL_MAXSTR];
	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	lvi.iItem=-1;
	lvi.pszText=pc;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	while((lvi.iItem=GetNextItem(lvi.iItem, LVNI_SELECTED))!=-1)
	{
		lvi.iSubItem=0;
		lvi.mask=LVIF_STATE|LVIF_PARAM;
		GetItem(&lvi);

		lvi.iSubItem=2;
		lvi.mask=LVIF_TEXT;
		GetItem(&lvi);

		sprintf(pcQuery, "UPDATE `proc` AS p SET p.uid=%d WHERE p.tid=%d AND p.jid=%S AND p.wid=%d", theApp.m_sess.m_uUID, (UINT)lvi.lParam, pc, theApp.m_sess.m_uWID);
		if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0)
		{
			sprintf(pcQuery, "CALL sp_state(%d, %S)", (UINT)lvi.lParam, pc);
			pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery));
		}
	}

	Reload(TRUE);
}

void CQueueView::OnPrior(UINT uCmdID)
{
	if(GetSelectedCount()!=1)
		return;

	int nAhead;
	LVITEM lvi;
	int n=GetItemCount()-1;
	lvi.iItem=GetNextItem(-1, LVNI_SELECTED);

	switch(uCmdID)
	{
	case ID_PRIOR_TOP:
	case ID_PRIOR_UP:
		if(lvi.iItem==0)
			return;

		nAhead=1;
		break;

	case ID_PRIOR_DOWN:
	case ID_PRIOR_BOTTOM:
		if(lvi.iItem==n)
			return;

		nAhead=0;
		break;

	default:
		return;
	}

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	TCHAR pc1[TOL_MAXSTR];
	lvi.cchTextMax=TOL_MAXSTR;

	lvi.mask=LVIF_PARAM;
	lvi.iSubItem=0;
	GetItem(&lvi);
	UINT u1=(UINT)lvi.lParam;

	lvi.mask=LVIF_TEXT;
	lvi.iSubItem=2;
	lvi.pszText=pc1;
	GetItem(&lvi);


	switch(uCmdID)
	{
	case ID_PRIOR_TOP: lvi.iItem=0; break;
	case ID_PRIOR_UP: lvi.iItem--; break;
	case ID_PRIOR_DOWN: lvi.iItem++; break;
	case ID_PRIOR_BOTTOM: lvi.iItem=n; break;
	}

	TCHAR pc2[TOL_MAXSTR];
	lvi.cchTextMax=TOL_MAXSTR;

	lvi.mask=LVIF_PARAM;
	lvi.iSubItem=0;
	GetItem(&lvi);
	UINT u2=(UINT)lvi.lParam;

	lvi.mask=LVIF_TEXT;
	lvi.iSubItem=2;
	lvi.pszText=pc2;
	GetItem(&lvi);

	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	sprintf(pcQuery, "CALL sp_prior(%d, %d, %S, %d, %S)", nAhead, u1, pc1, u2, pc2);

	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		theApp.ConnectionError(pcQuery);
	else
		Reload(TRUE);
}

void CQueueView::OnUpdateFileAdd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CQueueView::OnUpdateFileEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CQueueView::OnUpdateFileStart(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CQueueView::OnUpdateFileStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CQueueView::OnUpdateFileHistory(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CQueueView::OnUpdateFileSubmit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==3);
}

void CQueueView::OnUpdatePrior(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CQueueView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(theApp.m_sess.m_uRID==1)
	{
		LVHITTESTINFO hti;
		hti.flags=LVHT_ONITEM;
		GetCursorPos(&hti.pt);
		ScreenToClient(&hti.pt);

		HitTest(&hti);
		if(hti.iItem!=-1)
		{
			LVITEM lvi;
			lvi.mask=LVIF_PARAM;
			lvi.iItem=hti.iItem;
			lvi.iSubItem=0;
			GetItem(&lvi);
			EditTask((UINT)lvi.lParam);
		}
	}

	*pResult=0;
}

BOOL CQueueView::DeleteJob(MySQLConn* pConn, UINT uTID, UINT uJID)
{
	BOOL bSuccess;
	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	sprintf(pcQuery, "DELETE FROM `proc` WHERE tid=%d AND jid=%d", uTID, uJID);
	bSuccess=pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0;

	if(!bSuccess)
		return FALSE;

	sprintf(pcQuery, "DELETE FROM `job` WHERE tid=%d AND jid=%d", uTID, uJID);
	bSuccess=pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0;

	if(!bSuccess)
		return FALSE;

	sprintf(pcQuery, "DELETE FROM `task` WHERE tid=%d", uTID);
	return pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery));
}

void CQueueView::FillData()
{
	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];
 	sprintf(pcQuery, "CALL sp_job(%d, %d, \'%s\')", theApp.m_sess.m_uRID, theApp.m_uWID, APP_SQL_DATE_FORMAT);

	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i, j;
	PULONG len;
	LVITEM lvi;
	MYSQL_ROW row;
	CTaskJobPtr p;
	COleDateTime date;
	TCHAR pc[TOL_MAXSTR];
	int n=pRes->NumFields()-1;

	lvi.pszText=pc;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	for(i=0; row=pRes->FetchRow(); i++)
	{
		len=pRes->FetchLengths();

		lvi.mask=LVIF_STATE|LVIF_PARAM;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.lParam=atoi(row[0]);
		lvi.state=INDEXTOSTATEIMAGEMASK(atoi(row[n]));
		lvi.iItem=InsertItem(&lvi);

		if(lvi.iItem>=0)
		{
			p=new CTaskJob((UINT)lvi.lParam, atoi(row[2]), lvi.iItem);
			m_Jobs.Add(p);

			lvi.mask=LVIF_TEXT;

			for(j=1; j<n; j++)
			{
				if(row[j])
				{
					lvi.iSubItem=j;
					MultiByteToWideChar(65001, 0, row[j], len[j]+1, pc, TOL_MAXSTR);

					if(j==3)
					{
						date.ParseDateTime(pc, VAR_DATEVALUEONLY);
						lstrcpy(pc, date.Format(VAR_DATEVALUEONLY));
					}

					SetItem(&lvi);
				}
			}
		}
	}
}

BOOL CQueueView::ContextMenu(HMENU hMenu, UINT* puFlags, int iItem)
{
	HMENU hmMain=AfxGetMainWnd()->GetMenu()->m_hMenu;

	if(theApp.m_sess.m_uRID==1)
	{
		if(iItem!=-1)
		{
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_START, TRUE);
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_STOP, TRUE);
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_HISTORY, TRUE);
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_INFO, TRUE);
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);

			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_ADD, TRUE);
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_EDIT, TRUE);
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_DELETE, TRUE);
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		}
		else
		{
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_ADD, TRUE);
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		}
	}
	else if(theApp.m_sess.m_uRID==2)
	{
		if(iItem!=-1)
		{
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_INFO, TRUE);
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		}
	}
	else
	{
		if(iItem!=-1)
		{
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_SUBMIT, TRUE);
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		}
	}

	Menu_CopyItem(hMenu, -1, hmMain, ID_TOOLS_LOGIN, TRUE);

	if(theApp.m_sess.m_uRID==1)
		Menu_CopyItem(hMenu, -1, hmMain, ID_TOOLS_USERS, TRUE);

	Menu_CopyItem(hMenu, -1, hmMain, ID_TOOLS_OPTIONS, TRUE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	Menu_CopyItem(hMenu, -1, hmMain, ID_VIEW_RELOAD, TRUE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	Menu_CopyItem(hMenu, -1, hmMain, ID_APP_ABOUT, TRUE);

	return TRUE;
}
