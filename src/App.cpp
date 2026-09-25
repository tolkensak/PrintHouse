
#include "stdafx.h"
#include "App.h"
#include "MainFrm.h"
#include "LoginDlg.h"
#include "OptionsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CApp theApp;


BEGIN_MESSAGE_MAP(CApp, TWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


CApp::CApp()
	: m_bAutoReload(TRUE)
	, m_uReloadSpace(5)
{
	*m_pcServer='\0';
}

CApp::~CApp()
{
	SaveQueue();
}

BOOL CApp::InitInstance()
{
	InitCommonControls();

	if(!TWinApp::InitInstance())
		return FALSE;

	if(MySQL::LibraryInit())
		return FALSE;

	LinkCtrl_RegisterClass(m_hInstance);

#ifdef _MYSQLAPP
	strcpy(m_pcUser, "dba");
	strcpy(m_pcPass, "tol");
	strcpy(m_pcDB, "printhouse1");
#endif

	m_bAutoReload=GetProfileInt(_T(""), _T("AutoReload"), m_bAutoReload);
	m_uReloadSpace=GetProfileInt(_T(""), _T("ReloadSpace"), m_uReloadSpace);
	CString str=GetProfileString(_T(""), _T("Server"), _T(""));

//	if(str.IsEmpty())
//	{
//		COptionsDlg dlg;
//		dlg.DoModal();
//	}
//#ifdef _MYSQLAPP
//	else
//		WideCharToMultiByte(65001, 0, str, str.GetLength()+1, m_pcServer, TOL_MAXSTR, NULL, NULL);
//#endif

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;

	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW, NULL, NULL);
	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	pFrame->SendMessage(WM_COMMAND, MAKEWPARAM(ID_TOOLS_LOGIN, 0));
	return TRUE;
}

int CApp::ExitInstance()
{
	m_conn.Close();
	MySQL::LibraryEnd();
	return TWinApp::ExitInstance();
}

MySQLConn* CApp::GetConn()
{
	//int i=m_conn.Ping();

	//_ASSERTE(i==0);

	//if(i!=0)
	//	throw ArgException();

	//return m_conn;

	if(m_conn && m_conn.Ping()==0)
		return &m_conn;

	//m_conn.Close();

	if(!m_conn.Init())
	{
		AfxMessageBox(_T("Can not initialize MySQL server."));
		return NULL;
	}

	my_bool b=1;
	m_conn.Options(MYSQL_OPT_RECONNECT, &b);

	if(!m_conn.RealConnect("localhost", "dba", "tol", "printhouse1", 0, 0, CLIENT_MULTI_STATEMENTS|CLIENT_MULTI_RESULTS))
	{
		AfxMessageBox(_T("Can not connect to MySQL server."));
		return NULL;
	}

	m_conn.SetCharacterSet("utf8");

	return &m_conn;
}

void CApp::FieldInfo(UINT uID, CWnd* pWndParent)
{
	MessageBox(uID, MB_ICONINFORMATION, pWndParent);
	(pWndParent->GetDlgItem(uID))->SetFocus();
}

int CApp::MessageBox(UINT uID, UINT uType, CWnd* pWndParent)
{
	CString str;
	str.LoadString(uID);
	return MessageBox(str, uType, pWndParent);
}

int CApp::MessageBox(LPCTSTR pc, UINT uType, CWnd* pWndParent)
{
	CString str;

	if(uType&MB_ICONINFORMATION)
		str.LoadString(IDS_MBT_INFO);
	else if(uType&MB_ICONQUESTION)
		str.LoadString(IDS_MBT_CONF);
	else if(uType&MB_ICONWARNING)
		str.LoadString(IDS_MBT_WARN);

	return ::MessageBox(pWndParent->m_hWnd, pc, str, uType);
}

void CApp::ConnectionError(LPCSTR pcQuery)
{
	CString str;

#if defined(UNICODE) || defined(_UNICODE)
	str.Format(_T("ERRNO: %d\nERROR: %S\nSQLSTATE: %S"), m_conn.Errno(), m_conn.Error(), m_conn.SQLState());

	if(pcQuery)
		str.AppendFormat(_T("\n\nQUERY:     \n%S"), pcQuery);
#else
	str.Format(_T("ERRNO: %d\nERROR: %s\nSQLSTATE: %s"), m_conn.Errno(), m_conn.Error(), m_conn.SQLState());

	if(pcQuery)
		str.AppendFormat(_T("\n\nQUERY:     \n%s"), pcQuery);
#endif

	AfxMessageBox(str, MB_OK|MB_ICONERROR);
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUT };
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implementation
protected:
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
};

#define ICON_CX 64
#define ICON_CY 64

CAboutDlg::CAboutDlg()
	: CDialog(CAboutDlg::IDD)
	, m_hIcon(NULL)
{
	m_hIcon=(HICON)LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ICON_CX, ICON_CY, 0);
}

CAboutDlg::~CAboutDlg()
{
	if(m_hIcon)
		DestroyIcon(m_hIcon);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void CAboutDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis)
{
	if(m_hIcon && lpdis->CtlType==ODT_STATIC)
		DrawIconEx(lpdis->hDC, 0, 0, m_hIcon, ICON_CX, ICON_CY, 0, NULL, DI_NORMAL);

	CDialog::OnDrawItem(nIDCtl, lpdis);
}

// App command to run the dialog
void CApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CSess::CSess()
{
	Reset();
}

void CSess::Reset()
{
	m_uUID=0;
	m_uRID=0;
	m_uWID=0;
}

CSess& CSess::operator=(CSess& sess)
{
	m_uUID=sess.m_uUID;
	m_uRID=sess.m_uRID;
	m_uWID=sess.m_uWID;
	return *this;
}

CSess& CSess::operator()(UINT uUID, UINT uRID, UINT uWID)
{
	m_uUID=uUID;
	m_uRID=uRID;
	m_uWID=uWID;
	return *this;
}

void CApp::SaveQueue()
{
	if(m_sess.m_uRID!=1)
		return;

	CString str;
	str.Format(_T("user\\%d"), m_sess.m_uUID);
	WriteProfileInt(str, _T("shop"), m_uWID);
}

void CApp::LoadQueue()
{
	if(m_sess.m_uRID!=1)
	{
		m_uWID=m_sess.m_uWID;
		return;
	}

	CString str;
	str.Format(_T("user\\%d"), m_sess.m_uUID);
	m_uWID=GetProfileInt(str, _T("shop"), m_sess.m_uWID);
}
