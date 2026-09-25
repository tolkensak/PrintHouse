
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


#define WM_ADD_JOB WM_USER+1
#define WM_SEL_CHNAGE_WORK_LIST WM_USER+2


#define IDS_MBT_INFO  801
#define IDS_MBT_WARN  802
#define IDS_MBT_CONF  803


class CSess
{
public:
	CSess();
	void Reset();
	CSess& operator=(CSess& sess);
	CSess& operator()(UINT uUID, UINT uRID, UINT uWID);

	UINT m_uUID;
	UINT m_uRID;
	UINT m_uWID;
};


class CApp : public TWinApp
{
public:
	CApp();
	virtual ~CApp();

// Overrides
public:
	UINT m_uWID;
	CSess m_sess;
	BOOL m_bAutoReload;
	UINT m_uReloadSpace;
	CHAR m_pcServer[TOL_MAXSTR];

	MySQLConn* GetConn();

	void SaveQueue();
	void LoadQueue();

	int MessageBox(UINT uID, UINT uType, CWnd* pWndParent);
	int MessageBox(LPCTSTR pc, UINT uType, CWnd* pWndParent);
	void FieldInfo(UINT uID, CWnd* pWndParent);
	void ConnectionError(LPCSTR pcQuery=NULL);

protected:
	MySQLConn m_conn;

	afx_msg void OnAppAbout();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CApp theApp;
