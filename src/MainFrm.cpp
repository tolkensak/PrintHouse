// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "App.h"

#include "MainFrm.h"
#include "UsersDlg.h"
#include "PassDlg.h"
#include "OptionsDlg.h"
#include "LoginDlg.h"
#include "ProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, TMainWnd)

BEGIN_MESSAGE_MAP(CMainFrame, TMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_PROCESS_BAR, OnViewProcessBar)
	ON_COMMAND(ID_TOOLS_LOGIN, OnToolsLogin)
	ON_COMMAND(ID_TOOLS_USERS, OnToolsUsers)
	ON_COMMAND(ID_TOOLS_PASS, OnToolsPass)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND_RANGE(ID_VIEW_QUEUE, ID_VIEW_HISTORY, OnView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROCESS_BAR, OnUpdateViewProcessBar)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_USERS, OnUpdateToolsUsers)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PASS, OnUpdateToolsPass)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_QUEUE, ID_VIEW_HISTORY, OnUpdateView)
	ON_MESSAGE(WM_SEL_CHNAGE_WORK_LIST, OnSelChangeWorkList)
	ON_CBN_SELCHANGE(IDC_CB_QUEUE, OnSelChangeCbQueue)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
	: m_pvwActive(NULL)
	, m_pvwHistory(NULL)
	, m_himlToolbar(NULL)
	, m_himlState(NULL)
{
}

CMainFrame::~CMainFrame()
{
	if(m_pvwHistory)
		delete m_pvwHistory;

	if(m_himlToolbar)
		ImageList_Destroy(m_himlToolbar);

	if(m_himlState)
		ImageList_Destroy(m_himlState);
}

BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = (int)pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT)(UINT_PTR)pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}

#define ID_STC_QUEUE 998
#define ID_CB_QUEUE 999
#define SPACE       5

void CMainFrame::LoadStandardTBButtons()
{
	CToolBarCtrl& tb=m_tbStandard.GetToolBarCtrl();

	int i=tb.GetButtonCount();
	for(i--; i>=0; i--)
		tb.DeleteButton(i);

	i=0;
	TBBUTTON tbb[30];

	if(theApp.m_sess.m_uRID==1)
	{
		tbb[i].iBitmap=0;
		tbb[i].idCommand=ID_VIEW_QUEUE;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=1;
		tbb[i].idCommand=ID_VIEW_HISTORY;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		if(m_pvwActive->m_uID==ID_VIEW_QUEUE)
		{
			tbb[i].iBitmap=-1;
			tbb[i].idCommand=ID_STC_QUEUE;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=-1;
			tbb[i].idCommand=ID_CB_QUEUE;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;
			tbb[i].iBitmap=6;
			tbb[i].idCommand=ID_FILE_START;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=7;
			tbb[i].idCommand=ID_FILE_STOP;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=8;
			tbb[i].idCommand=ID_FILE_HISTORY;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=-1;
			tbb[i].idCommand=0;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_SEP;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=2;
			tbb[i].idCommand=ID_FILE_ADD;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=3;
			tbb[i].idCommand=ID_FILE_EDIT;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;
		}
		else
		{
			tbb[i].iBitmap=15;
			tbb[i].idCommand=ID_VIEW_TERM;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=-1;
			tbb[i].idCommand=0;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_SEP;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;
		}

		tbb[i].iBitmap=4;
		tbb[i].idCommand=ID_FILE_DELETE;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=5;
		tbb[i].idCommand=ID_FILE_INFO;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=13;
		tbb[i].idCommand=ID_VIEW_RELOAD;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=19;
		tbb[i].idCommand=ID_TOOLS_USERS;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;
	}
	else if(theApp.m_sess.m_uRID==2)
	{
		tbb[i].iBitmap=0;
		tbb[i].idCommand=ID_VIEW_QUEUE;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=1;
		tbb[i].idCommand=ID_VIEW_HISTORY;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		if(m_pvwActive->m_uID==ID_VIEW_HISTORY)
		{
			tbb[i].iBitmap=15;
			tbb[i].idCommand=ID_VIEW_TERM;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_BUTTON;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;

			tbb[i].iBitmap=-1;
			tbb[i].idCommand=0;
			tbb[i].fsState=TBSTATE_ENABLED;
			tbb[i].fsStyle=TBSTYLE_SEP;
			tbb[i].dwData=0;
			tbb[i].iString=-1;
			i++;
		}

		tbb[i].iBitmap=5;
		tbb[i].idCommand=ID_FILE_INFO;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=13;
		tbb[i].idCommand=ID_VIEW_RELOAD;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;
	}
	else if(theApp.m_sess.m_uRID==3)
	{
		tbb[i].iBitmap=16;
		tbb[i].idCommand=ID_FILE_SUBMIT;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=13;
		tbb[i].idCommand=ID_VIEW_RELOAD;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_BUTTON;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;

		tbb[i].iBitmap=-1;
		tbb[i].idCommand=0;
		tbb[i].fsState=TBSTATE_ENABLED;
		tbb[i].fsStyle=TBSTYLE_SEP;
		tbb[i].dwData=0;
		tbb[i].iString=-1;
		i++;
	}

	tbb[i].iBitmap=18;
	tbb[i].idCommand=ID_TOOLS_OPTIONS;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=TBSTYLE_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;
	i++;

	tbb[i].iBitmap=17;
	tbb[i].idCommand=ID_APP_ABOUT;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=TBSTYLE_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;
	i++;

	tbb[i].iBitmap=-1;
	tbb[i].idCommand=0;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=TBSTYLE_SEP;
	tbb[i].dwData=0;
	tbb[i].iString=-1;
	i++;

	tbb[i].iBitmap=14;
	tbb[i].idCommand=ID_TOOLS_LOGIN;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=TBSTYLE_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;
	i++;

	tb.AddButtons(i, tbb);

//	CSize sz=m_tbStandard.CalcFixedLayout(FALSE, TRUE);
//
//#ifdef _USE_REBAR
//	if(m_ReBar)
//	{
//		REBARBANDINFO rbBand;
//		rbBand.cbSize = sizeof(REBARBANDINFO);  // Required
//		rbBand.fMask  = RBBIM_SIZE;
//		rbBand.cx     = sz.cx+HIWORD(m_tbStandard.SendMessage(TB_GETBUTTONSIZE));
//		m_ReBar.GetReBarCtrl().SetBandInfo(0, &rbBand);
//	}
//#else
//	m_tbStandard.SetWindowPos(0, 0, 0, sz.cx, sz.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOCOPYBITS);
//#endif

	m_tbStandard.m_dx=0;

	if(theApp.m_sess.m_uRID==1 && m_pvwActive->m_uID==ID_VIEW_QUEUE)
	{
		CRect ri, rc;
		TBBUTTONINFO tbi;

		tbi.cbSize=sizeof(TBBUTTONINFO);
		tbi.dwMask=TBIF_SIZE|TBIF_STATE|TBIF_STYLE;
		tbi.fsState=0;
		tbi.fsStyle=0x40;//BTNS_SHOWTEXT;

		if(!m_stcQueue.GetSafeHwnd())
		{
			m_stcQueue.Create(_T("Ha"), WS_CHILD, CRect(0, 0, 20, 15), &m_tbStandard);
			m_stcQueue.SetFont(m_tbStandard.GetFont());
			m_tbStandard.ModifyStyle(0, WS_CLIPCHILDREN);
		}

		if(!m_cbQueue.GetSafeHwnd())
		{
			m_cbQueue.Create(WS_CHILD|WS_TABSTOP|CBS_DROPDOWNLIST, CRect(0, 0, 130, 300), &m_tbStandard, IDC_CB_QUEUE);
			m_cbQueue.SetFont(m_tbStandard.GetFont());
			m_tbStandard.ModifyStyle(0, WS_CLIPCHILDREN);
			FillQueue();
		}

		i=m_tbStandard.CommandToIndex(ID_STC_QUEUE);
		if(i>=0)
		{
			m_tbStandard.GetItemRect(i, &ri);
			m_tbStandard.m_dx-=ri.Width();

			m_stcQueue.GetWindowRect(&rc);
			tbi.cx=static_cast<WORD>(rc.Width()+SPACE);
			tb.SetButtonInfo(ID_STC_QUEUE, &tbi);
			m_tbStandard.m_dx+=tbi.cx;

			ri.top+=max((ri.Height()-rc.Height())/2, 0);	 //move to middle
			m_stcQueue.SetWindowPos(0, ri.left+SPACE, ri.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS);
		}

		i=m_tbStandard.CommandToIndex(ID_CB_QUEUE);
		if(i>=0)
		{
			m_tbStandard.GetItemRect(i, &ri);
			m_tbStandard.m_dx-=ri.Width();

			m_cbQueue.GetWindowRect(&rc);
			tbi.cx=static_cast<WORD>(rc.Width()+SPACE);
			tb.SetButtonInfo(ID_CB_QUEUE, &tbi);
			m_tbStandard.m_dx+=tbi.cx;

			ri.top+=max((ri.Height()-rc.Height())/2,0);	 //move to middle
			m_cbQueue.SetWindowPos(0, ri.left, ri.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS);
		}

		m_stcQueue.ShowWindow(SW_SHOW);
		m_cbQueue.ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_stcQueue.GetSafeHwnd())
			m_stcQueue.ShowWindow(SW_HIDE);

		if(m_cbQueue.GetSafeHwnd())
			m_cbQueue.ShowWindow(SW_HIDE);
	}

#ifndef _USE_REBAR
	RecalcLayout();
#endif
}

//void CMainFrame::LoadQueueTBButtons()
//{
//#ifdef _USE_REBAR
//	CReBarCtrl& rb=m_ReBar.GetReBarCtrl();
//#endif
//
//	if(theApp.m_sess.m_uRID!=1 || m_pvwActive->m_uID!=ID_VIEW_QUEUE)
//	{
//		if(m_tbQueue)
//#ifdef _USE_REBAR
//			rb.ShowBand(1, 0);
//#else
//		m_tbQueue.ShowWindow(SW_HIDE);
//#endif
//
//		return;
//	}
//
//	if(m_tbQueue)
//	{
//#ifdef _USE_REBAR
//		rb.ShowBand(1, 1);
//#else
//		m_tbQueue.ShowWindow(SW_SHOW);
//#endif
//		return;
//	}
//
//#ifdef _USE_REBAR
//	if(!m_tbQueue.CreateEx(this))
//#else
//	if(!m_tbQueue.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
//#endif
//		return;
//
//#ifndef _USE_REBAR
//	m_tbQueue.SetWindowText(_T("Queue"));
//#endif
//	m_tbQueue.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)m_himlToolbar);
//	m_tbQueue.SetSizes(CSize(31, 31), CSize(24, 24));
//
//	int i=0;
//	TBBUTTON tbb[30];
//	CToolBarCtrl& tb=m_tbQueue.GetToolBarCtrl();
//
//	tbb[i].iBitmap=-1;
//	tbb[i].idCommand=ID_STC_QUEUE;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_BUTTON;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tbb[i].iBitmap=-1;
//	tbb[i].idCommand=ID_CB_QUEUE;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_BUTTON;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tbb[i].iBitmap=-1;
//	tbb[i].idCommand=0;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_SEP;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tbb[i].iBitmap=9;
//	tbb[i].idCommand=ID_PRIOR_TOP;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_BUTTON;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tbb[i].iBitmap=10;
//	tbb[i].idCommand=ID_PRIOR_UP;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_BUTTON;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tbb[i].iBitmap=11;
//	tbb[i].idCommand=ID_PRIOR_DOWN;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_BUTTON;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tbb[i].iBitmap=12;
//	tbb[i].idCommand=ID_PRIOR_BOTTOM;
//	tbb[i].fsState=TBSTATE_ENABLED;
//	tbb[i].fsStyle=TBSTYLE_BUTTON;
//	tbb[i].dwData=0;
//	tbb[i].iString=-1;
//	i++;
//
//	tb.AddButtons(i, tbb);
//
//	m_tbQueue.m_dx=0;
//
//	CRect ri, rc;
//	TBBUTTONINFO tbi;
//
//	tbi.cbSize=sizeof(TBBUTTONINFO);
//	tbi.dwMask=TBIF_SIZE|TBIF_STATE|TBIF_STYLE;
//	tbi.fsState=0;
//	tbi.fsStyle=0x40;//BTNS_SHOWTEXT;
//
//	i=m_tbQueue.CommandToIndex(ID_STC_QUEUE);
//	if(i>=0 && m_stcQueue.Create(_T("HA"), WS_CHILD|WS_VISIBLE, CRect(0, 0, 50, 15), &m_tbQueue))
//	{
//		m_stcQueue.SetFont(m_tbQueue.GetFont());
//		m_tbQueue.ModifyStyle(0, WS_CLIPCHILDREN);
//
//		m_tbQueue.GetItemRect(i, &ri);
//		m_tbQueue.m_dx-=ri.Width();
//
//		m_stcQueue.GetWindowRect(&rc);
//		tbi.cx=static_cast<WORD>(rc.Width()+SPACE);
//		tb.SetButtonInfo(ID_STC_QUEUE, &tbi);
//		m_tbQueue.m_dx+=tbi.cx;
//
//		ri.top+=max((ri.Height()-rc.Height())/2, 0);	 //move to middle
//		m_stcQueue.SetWindowPos(0, ri.left+SPACE, ri.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS);
//	}
//
//	i=m_tbQueue.CommandToIndex(ID_CB_QUEUE);
//	if(i>=0 && m_cbQueue.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|CBS_DROPDOWNLIST, CRect(0, 0, 130, 300), &m_tbQueue, IDC_CB_QUEUE))
//	{
//		m_cbQueue.SetFont(m_tbQueue.GetFont());
//		m_tbQueue.ModifyStyle(0, WS_CLIPCHILDREN);
//		FillQueue();
//
//		m_tbQueue.GetItemRect(i, &ri);
//		m_tbQueue.m_dx-=ri.Width();
//
//		m_cbQueue.GetWindowRect(&rc);
//		tbi.cx=static_cast<WORD>(rc.Width()+SPACE);
//		tb.SetButtonInfo(ID_CB_QUEUE, &tbi);
//		m_tbQueue.m_dx+=tbi.cx;
//
//		ri.top+=max((ri.Height()-rc.Height())/2,0);	 //move to middle
//		m_cbQueue.SetWindowPos(0, ri.left, ri.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS);
//	}
//
//#ifdef _USE_REBAR
//	m_ReBar.AddBar(&m_tbQueue);
//	m_tbQueue.SetBarStyle(m_tbQueue.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
//#else
//	m_tbQueue.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_tbQueue);
//#endif
//}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (TMainWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_himlToolbar=ImageList_LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDB_TOOLBAR), 24, 0, 0xFF00FF, IMAGE_BITMAP, LR_CREATEDIBSECTION);
	m_himlState=ImageList_LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDB_STATE), 16, 0, 0xFF00FF, IMAGE_BITMAP, LR_CREATEDIBSECTION);

#ifdef _USE_REBAR
	if (!m_tbStandard.CreateEx(this))
#else
	if (!m_tbStandard.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
#endif
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_tbStandard.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)m_himlToolbar);
	m_tbStandard.SetSizes(CSize(31, 31), CSize(24, 24));
	LoadStandardTBButtons();

#ifdef _USE_REBAR
	if(!m_ReBar.Create(this) || !m_ReBar.AddBar(&m_tbStandard))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_tbStandard.SetBarStyle(m_tbStandard.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
#else
	m_tbStandard.SetWindowText(_T("Standard"));
#endif

	CString str;
	str.LoadString(IDS_PROCESS_BAR_TITLE);
	if(!m_cbProcess.Create(str, this, CSize(200, 100), TRUE, IDC_DB_PROCESS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(!m_vwProcess.CreateEx(WS_EX_CLIENTEDGE, WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS
		|LVS_OWNERDRAWFIXED
		, CRect(0, 0, 0, 0), &m_cbProcess, IDC_LC_PROCESS))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	ListView_SetImageList(m_vwProcess.m_hWnd, m_himlState, LVSIL_STATE);
	m_vwProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP);
	m_cbProcess.SetCtrl(&m_vwProcess);

	HWND hWnd=CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, m_hWnd, (HMENU)AFX_IDW_PANE_FIRST, theApp.m_hInstance, NULL);
	if (!hWnd)
	//CListCtrl lc;
	//if (!lc.CreateEx(WS_EX_CLIENTEDGE, WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if(!m_vwDummy.SubclassWindow(hWnd))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	m_pvwActive=&m_vwDummy;
	ListView_SetImageList(m_pvwActive->m_hWnd, m_himlState, LVSIL_STATE);
	m_pvwActive->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
	m_pvwActive->LoadCols();

	if(!m_StatusBar.Create(this) || !m_StatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_cbProcess.SetBarStyle(m_cbProcess.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_cbProcess.EnableDocking(CBRS_ALIGN_ANY);
#ifndef _USE_REBAR
	m_tbStandard.EnableDocking(CBRS_ALIGN_ANY);
#endif
	EnableDocking(CBRS_ALIGN_ANY);
#ifndef _USE_REBAR
	DockControlBar(&m_tbStandard);
#endif
	DockControlBar(&m_cbProcess, AFX_IDW_DOCKBAR_RIGHT);

	str=_T("ctrl\\");
	if(VerifyBarState(str))
	{
		m_cbProcess.LoadState(str);
		LoadBarState(str);
	}

	SetView(1);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!TMainWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0, 0, 0, theApp.LoadIcon(IDR_MAINFRAME));
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	TMainWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	TMainWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	if(m_pvwActive)
		m_pvwActive->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(m_pvwActive && m_pvwActive->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return TMainWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewProcessBar()
{
	ShowControlBar(&m_cbProcess, !m_cbProcess.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateViewProcessBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_cbProcess.IsVisible());
}

BOOL CMainFrame::SetView(UINT uView)
{
	if(uView==m_pvwActive->m_uID)
		return FALSE;

	switch(uView)
	{
	case ID_VIEW_QUEUE:
		m_vwQueue.SubclassWindow(m_pvwActive->UnsubclassWindow());
		m_pvwActive=&m_vwQueue;
		break;
	case ID_VIEW_HISTORY:
		if(!m_pvwHistory)
			m_pvwHistory=new CHistoryView;

		m_pvwHistory->SubclassWindow(m_pvwActive->UnsubclassWindow());
		m_pvwActive=m_pvwHistory;
		break;
	default:
		m_vwDummy.SubclassWindow(m_pvwActive->UnsubclassWindow());
		m_pvwActive=&m_vwDummy;
		break;
	}

	LoadStandardTBButtons();
	//LoadQueueTBButtons();
	m_pvwActive->Reset();
	return TRUE;
}

void CMainFrame::OnToolsLogin()
{
	theApp.SaveQueue();
	theApp.m_uWID=0;
	theApp.m_sess.Reset();
	SetView(0);

	CLoginDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	theApp.m_sess=dlg.m_sess;
	theApp.LoadQueue();
	SetView(ID_VIEW_QUEUE);
	SelectQueue();
}

void CMainFrame::OnView(UINT uCmdID)
{
	SetView(uCmdID);
}

void CMainFrame::OnUpdateView(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uWID==1);

	if(m_pvwActive)
		pCmdUI->SetCheck(pCmdUI->m_nID==m_pvwActive->m_uID);
}

void CMainFrame::OnToolsUsers()
{
	CUsersDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateToolsUsers(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uRID==1);
}

void CMainFrame::OnToolsPass()
{
	CPassDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnSelChangeCbQueue()
{
	int i=m_cbQueue.GetCurSel();
	if(i==CB_ERR)
		return;

	theApp.m_uWID=(UINT)m_cbQueue.GetItemData(i);

	if(m_pvwActive)
		m_pvwActive->Reset();

	SetFocus();
}

void CMainFrame::OnUpdateToolsPass(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.m_sess.m_uUID);
}

void CMainFrame::OnToolsOptions()
{
	COptionsDlg dlg;
	if(dlg.DoModal()==IDOK)
		m_pvwActive->Reset();
}

void CMainFrame::OnDestroy()
{
	CString str(_T("ctrl\\"));
	m_cbProcess.SaveState(str);
	SaveBarState(str);

	if(m_pvwActive)
	{
		m_pvwActive->SaveState();
		m_pvwActive->UnsubclassWindow();
	}

	TMainWnd::OnDestroy();
}

LRESULT CMainFrame::OnSelChangeWorkList(WPARAM wParam, LPARAM lParam)
{
	if(!m_vwProcess)
		return 0;

	m_vwProcess.DeleteAllItems();

	if(!lParam)
		return 0;

	CHAR pcQuery[MAX_QRY_BUFF_LEN];
	CTaskJob* pJob=(CTaskJob*)lParam;

	switch(wParam)
	{
	case ID_VIEW_QUEUE:
		sprintf(pcQuery, "CALL sp_proc(%d, %d, %d, %d)", theApp.m_sess.m_uRID, theApp.m_sess.m_uWID, pJob->m_uTID, pJob->m_uJID);
		break;
	case ID_VIEW_HISTORY:
		sprintf(pcQuery, "CALL sp_proch(%d, %d)", pJob->m_uTID, pJob->m_uJID);
		break;
	default:
		return 0;
	}

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return 0;

	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return 0;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return 0;

	LVITEM lvi;
	PUlong len;
	MYSQL_ROW row;
	TCHAR pc[TOL_MAXSTR];

	lvi.pszText=pc;
	lvi.cchTextMax=TOL_MAXSTR;
	lvi.stateMask=LVIS_STATEIMAGEMASK;

	for(lvi.iItem=0; row=pRes->FetchRow(); lvi.iItem++)
	{
		len=pRes->FetchLengths();

		lvi.mask=LVIF_STATE;
		lvi.iSubItem=0;

		if(wParam==ID_VIEW_HISTORY)
			lvi.state=INDEXTOSTATEIMAGEMASK(5);
		else if(atoi(row[3]))
			lvi.state=INDEXTOSTATEIMAGEMASK(4);
		else
			lvi.state=(UINT)pJob->m_lParam;

		m_vwProcess.InsertItem(&lvi);

		lvi.mask=LVIF_TEXT;

		lvi.iSubItem=1;
		MultiByteToWideChar(65001, 0, row[0], len[0]+1, pc, TOL_MAXSTR);
		m_vwProcess.SetItem(&lvi);

		lvi.iSubItem=2;
		MultiByteToWideChar(65001, 0, row[1], len[1]+1, pc, TOL_MAXSTR);
		m_vwProcess.SetItem(&lvi);
	}

	return 0;
}

void CMainFrame::FillQueue()
{
	m_cbQueue.ResetContent();

	MySQLConn* pConn=theApp.GetConn();
	if(!pConn)
		return;

 	LPSTR pcQuery="SELECT w.name, w.wid FROM `work` AS w ORDER BY 1";
	if(pConn->RealQuery(pcQuery, (ULONG)strlen(pcQuery)))
		return;

	MySQLResPtr pRes=pConn->StoreResult();
	if(!pRes)
		return;

	PULONG len;
	MYSQL_ROW row;
	TCHAR pc[TOL_MAXSTR];

	while(row=pRes->FetchRow())
	{
		len=pRes->FetchLengths();
		MultiByteToWideChar(65001, 0, row[0], len[0]+1, pc, TOL_MAXSTR);
		m_cbQueue.SetItemData(m_cbQueue.AddString(pc), atoi(row[1]));
	}
}

void CMainFrame::SelectQueue()
{
	if(theApp.m_sess.m_uRID!=1)
		return;

	int i, n=m_cbQueue.GetCount();
	for(i=0; i<n; i++)
	{
		if(theApp.m_uWID==(UINT)m_cbQueue.GetItemData(i))
		{
			m_cbQueue.SetCurSel(i);
			return;
		}
	}
}
