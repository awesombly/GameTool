#pragma once
#include "Dialog_Null.h"
#include "Dialog_CRenderer.h"
#include "Dialog_CAnimation.h"
#include "Dialog_CCollider.h"
#include "Dialog_CTransformer.h"


class Pane_Component : public CDockablePane
{
public:
	static Pane_Component* Instance;
	DECLARE_DYNAMIC(Pane_Component)
	Dialog_Null*		 m_wndNull;
	Dialog_CRenderer*	 m_wndCRenderer;
	Dialog_CAnimation*	 m_wndCAnimation;
	Dialog_CTransformer* m_wndCTransformer;
	Dialog_CCollider*	 m_wndCCollder;
public:
	void SetCompDialog(const Component* pComp) noexcept;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	Pane_Component();
	virtual ~Pane_Component();
};

