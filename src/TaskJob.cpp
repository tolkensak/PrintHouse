
#include "stdafx.h"
#include "App.h"
#include "TaskJob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTaskJob::CTaskJob()
	: m_uTID(0)
	, m_uJID(0)
	, m_lParam(0)
{
}

CTaskJob::CTaskJob(CTaskJob& job)
{
	m_uTID=job.m_uTID;
	m_uJID=job.m_uJID;
	m_lParam=job.m_lParam;
}

CTaskJob::CTaskJob(UINT uTID, UINT uJID, LPARAM lParam)
{
	m_uTID=uTID;
	m_uJID=uJID;
	m_lParam=lParam;
}
