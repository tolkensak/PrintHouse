
#pragma once

// CTaskJob class

class CTaskJob : public SmartObject
{
public:
	CTaskJob();
	CTaskJob(CTaskJob& job);
	CTaskJob(UINT uTID, UINT uJID, LPARAM lParam);

	UINT m_uTID;
	UINT m_uJID;
	LPARAM m_lParam;
};

typedef SmartPointer<CTaskJob> CTaskJobPtr;
typedef	CArray <CTaskJobPtr, CTaskJobPtr&> CTaskJobArray;
