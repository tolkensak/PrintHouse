// DummyView.h : interface of the CDummyView class
//

#pragma once

// CDummyView

class CDummyView : public TGridCtrl
{
public:
	CDummyView();
	virtual ~CDummyView();

	void LoadCols();
	virtual void Reset();
	virtual BOOL ContextMenu(HMENU hMenu, UINT* puFlags, int iItem);

public:
	UINT m_uID;

protected:
	DECLARE_MESSAGE_MAP()
};

