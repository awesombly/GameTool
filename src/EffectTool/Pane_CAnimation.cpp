#include "stdafx.h"
#include "EffectTool.h"
#include "Pane_CAnimation.h"


IMPLEMENT_DYNAMIC(Pane_CAnimation, CDockablePane)

Pane_CAnimation::Pane_CAnimation()
{}


Pane_CAnimation::~Pane_CAnimation()
{}

BEGIN_MESSAGE_MAP(Pane_CAnimation, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// MapPane 메시지 처리기

int Pane_CAnimation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->m_wndForm = Dialog_CAnimation::CreateOne(this);

	return 0;
}

void Pane_CAnimation::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		// 너비, 높이 재설정, z값 무시
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

int Pane_CAnimation::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();

	// 부모가 desk이거나 desk 자식이 이놈일때만
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}

