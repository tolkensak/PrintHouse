
#include "stdafx.h"
#include "App.h"
#include "ProcessView.h"

/////////////////////////////////////////////////////////////////////////////
// CProcessView

BEGIN_MESSAGE_MAP(CProcessView, TGridCtrlFit)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(1, 99, OnToggleCol)
END_MESSAGE_MAP()

CProcessView::CProcessView()
{
	SetEmptyStr(IDS_EMPTY_TEXT_PROCESS);
}

CProcessView::~CProcessView()
{
}

int CProcessView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(TGridCtrlFit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString str;

	str.LoadString(IDS_COL_STATE);
	InsertColumn(0, str, LVCFMT_LEFT, 24, 0);

	str.LoadString(IDS_COL_SHOP);
	InsertColumn(1, str, LVCFMT_LEFT, 56, 0);

	str.LoadString(IDS_COL_VALUE);
	InsertColumn(2, str, LVCFMT_RIGHT, -1, 1);

	LoadState();
	return 0;
}

#define STATETOINDEX(s) ((((s)&LVIS_STATEIMAGEMASK)>>12)-1)

COLORREF _crState[]=
{
	0xccffff, // sari
	0xccffcc, // jasil
	0xccccff, // hizil
	0xffcc88, // kok
	0xcccccc  // sur
};

int _nStateNum=sizeof(_crState)/sizeof(COLORREF);

void CProcessView::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	ASSERT(lpdis->CtlType==ODT_LISTVIEW);
	if(lpdis->itemID<0)
		return;

	LVITEM lvi;
	lvi.iItem=lpdis->itemID;

	lvi.mask=LVIF_STATE;
	lvi.stateMask=LVIS_STATEIMAGEMASK;
	lvi.iSubItem=0;
	GetItem(&lvi);

	CDC dc;
	dc.Attach(lpdis->hDC);

	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	COLORREF crBkColor = crOldBkColor;

	int i=STATETOINDEX(lvi.state);
	if(i>=0 && i<_nStateNum)
		crBkColor=_crState[i];


	dc.SetBkColor(crBkColor);
	dc.FillSolidRect(&lpdis->rcItem, crBkColor);

	if(lpdis->itemID>0)
	{
		CPen pen(PS_SOLID, 1, 0xd8e9ec);
		CPen* oldPen=dc.SelectObject(&pen);
		dc.MoveTo(lpdis->rcItem.left, lpdis->rcItem.top);
		dc.LineTo(lpdis->rcItem.right, lpdis->rcItem.top);
		dc.SelectObject(oldPen);
	}

	CRect rc(lpdis->rcItem);
	GetImageList(LVSIL_STATE)->Draw(&dc, i, rc.TopLeft(), ILD_NORMAL);
	rc.left+=GetColumnWidth(0);

	TCHAR pc[TOL_MAXSTR];
	lvi.mask=LVIF_TEXT;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.pszText=pc;

	lvi.iSubItem=1;
	GetItem(&lvi);
	rc.right=rc.left+GetColumnWidth(1)-2;
	rc.left+=5;
	dc.DrawText(pc, -1, &rc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	lvi.iSubItem=2;
	GetItem(&lvi);
	rc.left=rc.right+4;
	rc.right=lpdis->rcItem.right-5;
	dc.DrawText(pc, -1, &rc, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}
