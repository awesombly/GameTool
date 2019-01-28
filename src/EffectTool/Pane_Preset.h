#pragma once
#include "Dialog_Preset.h"
#include "Dialog_CParticle.h"


class Pane_Preset : public CDockablePane
{
public:
	static Pane_Preset* Instance;
	DECLARE_DYNAMIC(Pane_Preset)
	Dialog_Preset*		m_wndPreset;
	Dialog_CParticle*	m_wndCParticle;
public:
	void SetCompDialog(const Component* pComp) noexcept;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	Pane_Preset();
	virtual ~Pane_Preset();
};