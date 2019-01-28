#include "stdafx.h"
#include "EffectTool.h"
#include "Dialog_CAnimation.h"
#include "afxdialogex.h"

Dialog_CAnimation* Dialog_CAnimation::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_CAnimation, CFormView)

Dialog_CAnimation::Dialog_CAnimation()
	: CFormView(IDD_DIALOG_CAnimation)
{
	Instance = this;
}


Dialog_CAnimation::~Dialog_CAnimation()
{}


void Dialog_CAnimation::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

Dialog_CAnimation* Dialog_CAnimation::CreateOne(CWnd* pParent)
{
	Dialog_CAnimation* pForm = new Dialog_CAnimation();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_CAnimation, CFormView)
END_MESSAGE_MAP()