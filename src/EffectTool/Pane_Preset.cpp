#include "stdafx.h"
#include "EffectTool.h"
#include "Pane_Preset.h"



IMPLEMENT_DYNAMIC(Pane_Preset, CDockablePane)
Pane_Preset* Pane_Preset::Instance = nullptr;

Pane_Preset::Pane_Preset()
{	
	Instance = this;
}


Pane_Preset::~Pane_Preset()
{}

BEGIN_MESSAGE_MAP(Pane_Preset, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// MapPane 메시지 처리기

int Pane_Preset::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->m_wndPreset = Dialog_Preset::CreateOne(this);
	this->m_wndCParticle = Dialog_CParticle::CreateOne(this);

	return 0;
}

void Pane_Preset::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndPreset)
	{
		// 너비, 높이 재설정, z값 무시
		m_wndPreset->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	if (m_wndCParticle)
	{
		// 너비, 높이 재설정, z값 무시
		m_wndCParticle->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		m_wndCParticle->ShowWindow(SW_HIDE);
	}
}

int Pane_Preset::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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

// 컴포넌트 타입별 설정
void Pane_Preset::SetCompDialog(const Component* pComp) noexcept
{
	try {
		m_wndCParticle->ShowWindow(SW_HIDE);
		m_wndPreset->ShowWindow(SW_HIDE);
		if (pComp == nullptr)
		{
			m_wndPreset->ShowWindow(SW_SHOW);
			return;
		}

		bool isNullComp = true;
		switch (pComp->m_comptType)
		{
		case EComponent::Renderer:
		{
			switch (((Renderer*)pComp)->m_eRenderType)
			{
			case ERenderType::Particle:
			{
				m_wndCParticle->ShowWindow(SW_SHOW);
				m_wndCParticle->UpdateEdit(pComp);
				isNullComp = false;
			}	break;
			}
		}	break;
		}

		if (isNullComp)
		{
			m_wndPreset->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		MessageBox(L"처리중 예외가 발생 하였습니다.", L"삐빅-");
	}
}