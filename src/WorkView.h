// WorkView.h : interface of the CWorkView class
//

#pragma once

#include "DummyView.h"
#include "TaskJob.h"

// CWorkView

class CWorkView : public CDummyView
{
public:
	CWorkView();
	virtual ~CWorkView();

	afx_msg void OnFileDelete();
	afx_msg void OnFileInfo();
	afx_msg void OnViewReload();
	afx_msg void OnUpdateFileDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewReload(CCmdUI *pCmdUI);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnToggleCol(UINT uCmdID){};

	virtual void Reset();

public:
	CTaskJobArray m_Jobs;

protected:
	void Erase();
	void SelChanged();
	virtual void FillData(){};
	void Reload(BOOL bKeepState=FALSE);
	virtual BOOL DeleteJob(MySQLConn* pConn, UINT uTID, UINT uJID){return FALSE;};

	void SaveState(CTaskJobArray& jobs);
	void LoadState(CTaskJobArray& jobs);
	DECLARE_MESSAGE_MAP()
};

