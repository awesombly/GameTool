#pragma once
#include "Dialog_Inspector.h"


class Pane_Inspector : public CDockablePane
{
public:
	DECLARE_DYNAMIC(Pane_Inspector)
	Dialog_Inspector* m_wndForm;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	Pane_Inspector();
	virtual ~Pane_Inspector();
};

