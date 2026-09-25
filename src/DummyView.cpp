// DummyView.cpp : implementation of the CDummyView class
//

#include "stdafx.h"
#include "App.h"
#include "DummyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDummyView

CDummyView::CDummyView()
	: m_uID(0)
{
}

CDummyView::~CDummyView()
{
}


BEGIN_MESSAGE_MAP(CDummyView, TGridCtrl)
END_MESSAGE_MAP()

void CDummyView::Reset()
{
	DeleteAllItems();
	AfxGetMainWnd()->SendMessage(WM_SEL_CHNAGE_WORK_LIST, m_uID);
}

void CDummyView::LoadCols()
{
	int i=0;
	CString str;
	LVCOLUMN lvc;

	lvc.mask=LVCF_TEXT|LVCF_WIDTH;
	str.LoadString(IDS_COL_STATE);
	lvc.pszText=str.GetBuffer();
	lvc.cx=24;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_TEXT|LVCF_WIDTH;
	str.LoadString(IDS_COL_CODE);
	lvc.pszText=str.GetBuffer();
	lvc.cx=80;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	str.LoadString(IDS_COL_JOB);
	lvc.pszText=str.GetBuffer();
	lvc.cx=40;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	str.LoadString(IDS_COL_REFER_DATE);
	lvc.pszText=str.GetBuffer();
	lvc.cx=100;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_TEXT|LVCF_WIDTH;
	str.LoadString(IDS_COL_COMPANY);
	lvc.pszText=str.GetBuffer();
	lvc.cx=100;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_TEXT|LVCF_WIDTH;
	str.LoadString(IDS_COL_TASK);
	lvc.pszText=str.GetBuffer();
	lvc.cx=100;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_RIGHT;
	str.LoadString(IDS_COL_EDITION);
	lvc.pszText=str.GetBuffer();
	lvc.cx=50;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	str.LoadString(IDS_COL_SIZE);
	lvc.pszText=str.GetBuffer();
	lvc.cx=100;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	str.LoadString(IDS_COL_PAPER_FMT);
	lvc.pszText=str.GetBuffer();
	lvc.cx=100;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_RIGHT;
	str.LoadString(IDS_COL_PAPER_NUM);
	lvc.pszText=str.GetBuffer();
	lvc.cx=50;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	str.LoadString(IDS_COL_PRINT_FMT);
	lvc.pszText=str.GetBuffer();
	lvc.cx=100;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	str.LoadString(IDS_COL_COLOR);
	lvc.pszText=str.GetBuffer();
	lvc.cx=70;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_RIGHT;
	str.LoadString(IDS_COL_PLAST_NUM);
	lvc.pszText=str.GetBuffer();
	lvc.cx=85;
	InsertColumn(i++, &lvc);

	lvc.mask=LVCF_TEXT|LVCF_WIDTH;
	str.LoadString(IDS_COL_NOTE);
	lvc.pszText=str.GetBuffer();
	lvc.cx=130;
	InsertColumn(i++, &lvc);

	LoadState();
}

BOOL CDummyView::ContextMenu(HMENU hMenu, UINT* puFlags, int iItem)
{
	HMENU hmMain=AfxGetMainWnd()->GetMenu()->m_hMenu;
	Menu_CopyItem(hMenu, -1, hmMain, ID_TOOLS_LOGIN, TRUE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	Menu_CopyItem(hMenu, -1, hmMain, ID_TOOLS_OPTIONS, TRUE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	Menu_CopyItem(hMenu, -1, hmMain, ID_APP_ABOUT, TRUE);
	return TRUE;
}
