#pragma once
#include "Dialog_CAnimation.h"


class Pane_CAnimation : public CDockablePane
{
public:
	DECLARE_DYNAMIC(Pane_CAnimation)
	Dialog_CAnimation* m_wndForm;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	Pane_CAnimation();
	virtual ~Pane_CAnimation();
};