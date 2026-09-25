
#pragma once

// CToolBarX

class CToolBarX : public CToolBar
{
	DECLARE_DYNAMIC(CToolBarX)

public:
	CToolBarX();
	virtual ~CToolBarX();

	int m_dx;

	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);

protected:
	DECLARE_MESSAGE_MAP()
};


