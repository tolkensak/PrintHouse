// HistoryView.h : interface of the CHistoryView class
//

#pragma once

#include "WorkView.h"
#include "TermDlg.h"

// CHistoryView

class CHistoryView : public CWorkView
{
public:
	CHistoryView();
	virtual ~CHistoryView();

	virtual void Reset();
	virtual BOOL ContextMenu(HMENU hMenu, UINT* puFlags, int iItem);
	afx_msg void OnViewTerm();

protected:
	UINT m_uLen;
	CTermDlg m_dlgTerm;
	CHAR m_pcQuery[MAX_QRY_BUFF_LEN];

	void FormatQuery();
	virtual void FillData();
	virtual BOOL DeleteJob(MySQLConn* pConn, UINT uTID, UINT uJID);
	DECLARE_MESSAGE_MAP()
};
