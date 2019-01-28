#include "stdafx.h"
#include "EffectTool.h"
#include "afxdialogex.h"
#include "Dialog_Null.h"

Dialog_Null* Dialog_Null::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_Null, CFormView)

Dialog_Null::Dialog_Null()
	: CFormView(IDD_DIALOG_Null)
{
	Instance = this;
}


Dialog_Null::~Dialog_Null()
{}


void Dialog_Null::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

Dialog_Null* Dialog_Null::CreateOne(CWnd* pParent)
{
	Dialog_Null* pForm = new Dialog_Null();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_Null, CFormView)

END_MESSAGE_MAP()