
#pragma once

#include "TaskJob.h"

class CProcessView : public TGridCtrlFit
{
public:
	CProcessView();
	virtual ~CProcessView();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToggleCol(UINT uCmdID){};

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void ContextMenuHeader(CPoint pt){};

protected:
	DECLARE_MESSAGE_MAP()
};
