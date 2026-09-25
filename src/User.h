
#pragma once

class CUser : public SmartObject
{
public:
	CUser();
	CUser(UINT uID, LPCTSTR pcName);
	CUser(CUser& user);
	CUser& operator=(CUser& user);

	UINT m_uUID;
	UINT m_uRID;
	UINT m_uWID;
	CString m_strLogin;
	CString m_strPass;
	CString m_strFName;
	CString m_strMName;
	CString m_strLName;
	CString m_strRole;
	CString m_strWork;
};

typedef SmartPointer<CUser> CUserPtr;
typedef	CArray <CUserPtr, CUserPtr&> CUserArray;
