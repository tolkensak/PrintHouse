// User.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "UserDlg.h"


CUser::CUser()
	: m_uUID(0)
	, m_uRID(0)
	, m_uWID(0)
	, m_strLogin(_T(""))
	, m_strPass(_T(""))
	, m_strFName(_T(""))
	, m_strMName(_T(""))
	, m_strLName(_T(""))
	, m_strRole(_T(""))
	, m_strWork(_T(""))
{
}

CUser::CUser(CUser& user)
{
	operator=(user);
}

CUser& CUser::operator=(CUser& user)
{
	m_uUID=user.m_uUID;
	m_uRID=user.m_uRID;
	m_uWID=user.m_uWID;
	m_strLogin=user.m_strLogin;
	m_strPass=user.m_strPass;
	m_strFName=user.m_strFName;
	m_strMName=user.m_strMName;
	m_strLName=user.m_strLName;
	m_strRole=user.m_strRole;
	m_strWork=user.m_strWork;

	return *this;
}
