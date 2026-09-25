// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "QueueView.h"
#include "HistoryView.h"
#include "ProcessView.h"
#include "ToolBarX.h"

//#define _USE_REBAR

class CMainFrame : public TMainWnd
{
protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnDestroy();
	afx_msg void OnToolsUsers();
	afx_msg void OnToolsLogin();
	afx_msg void OnToolsPass();
	afx_msg void OnToolsOptions();
	afx_msg void OnViewProcessBar();
	afx_msg void OnView(UINT uCmdID);
	afx_msg void OnUpdateToolsUsers(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsPass(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewProcessBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateView(CCmdUI *pCmdUI);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

protected:  // control bar embedded members
	CStatusBar   m_StatusBar;
	CToolBarX    m_tbStandard;
	//CToolBarX  m_tbQueue;
	TCtrlBarCtrl m_cbProcess;
	CProcessView m_vwProcess;
	CQueueView   m_vwQueue;
#ifdef _USE_REBAR
	CReBar      m_ReBar;
#endif
	CHistoryView*  m_pvwHistory;
	CDummyView*  m_pvwActive;
	CDummyView   m_vwDummy;
	HIMAGELIST  m_himlToolbar;
	HIMAGELIST  m_himlState;
	CStatic     m_stcQueue;
	CComboBox   m_cbQueue;

	void FillQueue();
	void SelectQueue();
	void LoadStandardTBButtons();
	//void LoadQueueTBButtons();
	BOOL SetView(UINT uView);
	BOOL VerifyBarState(LPCTSTR lpszProfileName);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg LRESULT OnSelChangeWorkList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChangeCbQueue();
	DECLARE_MESSAGE_MAP()
};


