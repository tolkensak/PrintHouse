// QueueView.h : interface of the CQueueView class
//

#pragma once

#include "WorkView.h"

// CQueueView

class CQueueView : public CWorkView
{
public:
	CQueueView();
	virtual ~CQueueView();

	afx_msg void OnFileAdd();
	afx_msg void OnFileEdit();
	afx_msg void OnFileStart();
	afx_msg void OnFileStop();
	afx_msg void OnFileHistory();
	afx_msg void OnFileSubmit();
	afx_msg void OnPrior(UINT uCmdID);
	afx_msg void OnUpdateFileAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileEdit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileHistory(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSubmit(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePrior(CCmdUI *pCmdUI);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	virtual void Reset();
	virtual BOOL ContextMenu(HMENU hMenu, PUINT puFlags, int iItem);

protected:
	UINT m_uTimer;

	void StartTimer();
	void EditTask(UINT uTID);
	virtual void FillData();
	virtual BOOL DeleteJob(MySQLConn* pConn, UINT uTID, UINT uJID);
	DECLARE_MESSAGE_MAP()
};

