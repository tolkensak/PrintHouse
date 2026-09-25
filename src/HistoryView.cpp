// HistoryView.cpp : implementation of the CHistoryView class
//

#include "stdafx.h"
#include "App.h"
#include "HistoryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHistoryView

CHistoryView::CHistoryView()
	: m_uLen(0)
{
	m_uID=ID_VIEW_HISTORY;
}

CHistoryView::~CHistoryView()
{
}

BEGIN_MESSAGE_MAP(CHistoryView, CWorkView)
	ON_COMMAND(ID_VIEW_TERM, OnViewTerm)
END_MESSAGE_MAP()

void CHistoryView::Reset()
{
	FormatQuery();
	CWorkView::Reset();
}

BOOL CHistoryView::DeleteJob(MySQLConn* pConn, UINT uTID, UINT uJID)
{
	BOOL bSuccess;
	CHAR pcQuery[MAX_QRY_BUFF_LEN];

	sprintf(pcQuery, "DELETE FROM `proch` WHERE tid=%d AND jid=%d", uTID, uJID);
	bSuccess=pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0;

	if(!bSuccess)
		return FALSE;

	sprintf(pcQuery, "DELETE FROM `jobh` WHERE tid=%d AND jid=%d", uTID, uJID);
	bSuccess=pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery))==0;

	if(!bSuccess)
		return FALSE;

	sprintf(pcQuery, "DELETE FROM `taskh` WHERE tid=%d", uTID);
	return pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery));
}

void CHistoryView::FillData()
{
	if(!m_uLen)
		return;

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

	if(pConn->RealQuery(m_pcQuery, m_uLen))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	int i, j;
	PUlong len;
	LVITEM lvi;
	MYSQL_ROW row;
	COleDateTime date;
	TCHAR pc[TOL_MAXSTR];
	int n=pRes->NumFields();
	CTaskJobPtr p;

	lvi.pszText=pc;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	for(i=0; row=pRes->FetchRow(); i++)
	{
		len=pRes->FetchLengths();

		lvi.mask=LVIF_STATE|LVIF_PARAM;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.lParam=atoi(row[0]);
		lvi.state=INDEXTOSTATEIMAGEMASK(5);
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

void CHistoryView::OnViewTerm()
{
	if(m_dlgTerm.DoModal()==IDOK)
		Reset();
}

void CHistoryView::FormatQuery()
{
	CString str;
	CString strWhere;
	CString strOrder;
	CStringArray arr;

	if(!m_dlgTerm.m_strCodeC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.code %s \'%s\' "), m_dlgTerm.m_strCodeC, m_dlgTerm.m_strCodeE);
	}

	if(!m_dlgTerm.m_strJobCF.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.jid %s %d "), m_dlgTerm.m_strJobCF, m_dlgTerm.m_uJobEF);
	}

	if(!m_dlgTerm.m_strJobCT.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.jid %s %d "), m_dlgTerm.m_strJobCT, m_dlgTerm.m_uJobET);
	}

	if(!m_dlgTerm.m_strReferDateCF.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.refer_date %s \'%s\' "), m_dlgTerm.m_strReferDateCF, m_dlgTerm.m_dtReferDateDF.Format(_T(APP_SQL_DATE_FORMAT)));
	}

	if(!m_dlgTerm.m_strReferDateCT.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.refer_date %s \'%s\' "), m_dlgTerm.m_strReferDateCT, m_dlgTerm.m_dtReferDateDT.Format(_T(APP_SQL_DATE_FORMAT)));
	}

	if(!m_dlgTerm.m_strCompanyC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.company %s \'%s\' "), m_dlgTerm.m_strCompanyC, m_dlgTerm.m_strCompanyE);
	}

	if(!m_dlgTerm.m_strTaskC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.name %s \'%s\' "), m_dlgTerm.m_strTaskC, m_dlgTerm.m_strTaskE);
	}

	if(!m_dlgTerm.m_strEditionCF.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.edition %s %u "), m_dlgTerm.m_strEditionCF, m_dlgTerm.m_uEditionEF);
	}

	if(!m_dlgTerm.m_strEditionCT.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.edition %s %u "), m_dlgTerm.m_strEditionCT, m_dlgTerm.m_uEditionET);
	}

	if(!m_dlgTerm.m_strSizeC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.size %s \'%s\' "), m_dlgTerm.m_strSizeC, m_dlgTerm.m_strSizeE);
	}

	if(!m_dlgTerm.m_strPaperFmtC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.paper_fmt %s \'%s\' "), m_dlgTerm.m_strPaperFmtC, m_dlgTerm.m_strPaperFmtE);
	}

	if(!m_dlgTerm.m_strPaperNumCF.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.paper_num %s %u "), m_dlgTerm.m_strPaperNumCF, m_dlgTerm.m_uPaperNumEF);
	}

	if(!m_dlgTerm.m_strPaperNumCT.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.paper_num %s %u "), m_dlgTerm.m_strPaperNumCT, m_dlgTerm.m_uPaperNumET);
	}

	if(!m_dlgTerm.m_strPrintFmtC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.print_fmt %s \'%s\' "), m_dlgTerm.m_strPrintFmtC, m_dlgTerm.m_strPrintFmtE);
	}

	if(!m_dlgTerm.m_strColorC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.color %s \'%s\' "), m_dlgTerm.m_strColorC, m_dlgTerm.m_strColorE);
	}

	if(!m_dlgTerm.m_strPlastNumCF.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.plast_num %s %u "), m_dlgTerm.m_strPlastNumCF, m_dlgTerm.m_uPlastNumEF);
	}

	if(!m_dlgTerm.m_strPlastNumCT.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" j.plast_num %s %u "), m_dlgTerm.m_strPlastNumCT, m_dlgTerm.m_uPlastNumET);
	}

	if(!m_dlgTerm.m_strNoteC.IsEmpty())
	{
		if(!strWhere.IsEmpty())
			strWhere+=_T(" AND ");

		strWhere.AppendFormat(_T(" t.note %s \'%s\' "), m_dlgTerm.m_strNoteC, m_dlgTerm.m_strNoteE);
	}


	if(m_dlgTerm.m_iCode>0)
	{
		str=_T(" t.code ");
		if(m_dlgTerm.m_bCode)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iCode, str);
	}

	if(m_dlgTerm.m_iJob>0)
	{
		str=_T(" j.jid ");
		if(m_dlgTerm.m_bJob)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iJob, str);
	}

	if(m_dlgTerm.m_iReferDate>0)
	{
		str=_T(" t.refer_date ");
		if(m_dlgTerm.m_bReferDate)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iReferDate, str);
	}

	if(m_dlgTerm.m_iCompany>0)
	{
		str=_T(" t.company ");
		if(m_dlgTerm.m_bCompany)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iCompany, str);
	}

	if(m_dlgTerm.m_iTask>0)
	{
		str=_T(" t.name ");
		if(m_dlgTerm.m_bTask)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iTask, str);
	}

	if(m_dlgTerm.m_iEdition>0)
	{
		str=_T(" t.edition ");
		if(m_dlgTerm.m_bEdition)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iEdition, str);
	}

	if(m_dlgTerm.m_iSize>0)
	{
		str=_T(" j.size ");
		if(m_dlgTerm.m_bSize)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iSize, str);
	}

	if(m_dlgTerm.m_iPaperFmt>0)
	{
		str=_T(" j.paper_fmt ");
		if(m_dlgTerm.m_bPaperFmt)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iPaperFmt, str);
	}

	if(m_dlgTerm.m_iPaperNum>0)
	{
		str=_T(" j.paper_num ");
		if(m_dlgTerm.m_bPaperNum)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iPaperNum, str);
	}

	if(m_dlgTerm.m_iPrintFmt>0)
	{
		str=_T(" j.print_fmt ");
		if(m_dlgTerm.m_bPrintFmt)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iPrintFmt, str);
	}

	if(m_dlgTerm.m_iColor>0)
	{
		str=_T(" j.color ");
		if(m_dlgTerm.m_bColor)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iColor, str);
	}

	if(m_dlgTerm.m_iPlastNum>0)
	{
		str=_T(" j.plast_num");
		if(m_dlgTerm.m_bPlastNum)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iPlastNum, str);
	}

	if(m_dlgTerm.m_iNote>0)
	{
		str=_T(" t.note ");
		if(m_dlgTerm.m_bNote)
			str+=_T(" DESC ");

		arr.SetAtGrow(m_dlgTerm.m_iNote, str);
	}

	strOrder.Empty();
	int n=(int)arr.GetCount();
	for(int i=0; i<n; i++)
	{
		if(arr[i].IsEmpty())
			continue;

		if(!strOrder.IsEmpty())
			strOrder+=_T(", ");

		strOrder+=arr[i];
	}

	CHAR pc[TOL_MAXSTR];
	CHAR pcWhere[TOL_MAXSTR];
	CHAR pcOrder[TOL_MAXSTR];

	*pcWhere='\0';
	*pcOrder='\0';

	if(!strWhere.IsEmpty())
	{
		WideCharToMultiByte(65001, 0, strWhere, strWhere.GetLength()+1, pc, TOL_MAXSTR, NULL, NULL);
		sprintf(pcWhere, " WHERE %s", pc);
	}

	if(!strOrder.IsEmpty())
	{
		WideCharToMultiByte(65001, 0, strOrder, strOrder.GetLength()+1, pc, TOL_MAXSTR, NULL, NULL);
		sprintf(pcOrder, " ORDER BY %s", pc);
	}

	m_uLen=sprintf(m_pcQuery, "SELECT t.tid, t.code, j.jid, DATE_FORMAT(t.refer_date, \'%s\'), t.company, t.name, t.edition, j.size, j.paper_fmt, j.paper_num, j.print_fmt, j.color, j.plast_num, t.note FROM taskh AS t INNER JOIN jobh AS j ON j.tid=t.tid %s %s", APP_SQL_DATE_FORMAT, pcWhere, pcOrder);
}

BOOL CHistoryView::ContextMenu(HMENU hMenu, UINT* puFlags, int iItem)
{
	HMENU hmMain=AfxGetMainWnd()->GetMenu()->m_hMenu;

	if(iItem!=-1)
	{
		Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_INFO, TRUE);

		if(theApp.m_sess.m_uRID==1 && iItem!=-1)
			Menu_CopyItem(hMenu, -1, hmMain, ID_FILE_DELETE, TRUE);

		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	}

	Menu_CopyItem(hMenu, -1, hmMain, ID_VIEW_TERM, TRUE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
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
