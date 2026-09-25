// ToolBarX.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "ToolBarX.h"


// CToolBarX

IMPLEMENT_DYNAMIC(CToolBarX, CToolBar)

CToolBarX::CToolBarX() : m_dx(0)
{
}

CToolBarX::~CToolBarX()
{
}


BEGIN_MESSAGE_MAP(CToolBarX, CToolBar)
END_MESSAGE_MAP()

CSize CToolBarX::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize sz=CToolBar::CalcFixedLayout(bStretch, bHorz);
	sz.cx+=m_dx;
	return sz;
}

CSize CToolBarX::CalcDynamicLayout(int nLength, DWORD nMode)
{
	CSize sz=CToolBar::CalcDynamicLayout(nLength, nMode);
	sz.cx+=m_dx;
	return sz;
}
