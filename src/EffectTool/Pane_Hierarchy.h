#pragma once
#include "Dialog_Hierarchy.h"


class Pane_Hierarchy : public CDockablePane
{
public:
	DECLARE_DYNAMIC(Pane_Hierarchy)
	Dialog_Hierarchy* m_wndForm;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	Pane_Hierarchy();
	virtual ~Pane_Hierarchy();
};