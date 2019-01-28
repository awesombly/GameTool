#include "stdafx.h"
#include "EffectTool.h"
#include "afxdialogex.h"
#include "Dialog_CTransformer.h"
#include "Dialog_Inspector.h"

Dialog_CTransformer* Dialog_CTransformer::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_CTransformer, CFormView)

Dialog_CTransformer::Dialog_CTransformer()
	: CFormView(IDD_DIALOG_CTransformer)
{
	Instance = this;
}


Dialog_CTransformer::~Dialog_CTransformer()
{}


void Dialog_CTransformer::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_T_PosX, m_PosX);
	DDX_Control(pDX, IDC_T_PosY, m_PosY);
	DDX_Control(pDX, IDC_T_PosZ, m_PosZ);
	DDX_Control(pDX, IDC_T_RotX, m_RotX);
	DDX_Control(pDX, IDC_T_RotY, m_RotY);
	DDX_Control(pDX, IDC_T_RotZ, m_RotZ);
	DDX_Control(pDX, IDC_T_ScaleX, m_ScaleX);
	DDX_Control(pDX, IDC_T_ScaleY, m_ScaleY);
	DDX_Control(pDX, IDC_T_ScaleZ, m_ScaleZ);
	DDX_Control(pDX, IDC_isEnable, m_isEnable);
}

Dialog_CTransformer* Dialog_CTransformer::CreateOne(CWnd* pParent)
{
	Dialog_CTransformer* pForm = new Dialog_CTransformer();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_CTransformer, CFormView)

	ON_BN_CLICKED(IDC_isEnable, &Dialog_CTransformer::OnBnClickedisenable)
	ON_BN_CLICKED(ID_T_Apply, &Dialog_CTransformer::OnBnClickedTApply)
	ON_BN_CLICKED(ID_T_Reset, &Dialog_CTransformer::OnBnClickedTReset)
END_MESSAGE_MAP()


void Dialog_CTransformer::OnBnClickedisenable()
{
	if (Dialog_Inspector::Instance->SelectComponent != nullptr)
	{
		m_pTrans->isEnable(m_isEnable.GetCheck());
	}
}



// =====================================================================

// 다이얼로그 갱신
void Dialog_CTransformer::UpdateEdit(const Component* pComp) noexcept
{
	if (pComp == nullptr) return;
	m_pTrans = (CTransformer*)pComp;
	
	D3DXVECTOR3 vector3 = m_pTrans->GetPosition();
	m_PosX.SetWindowTextW(std::to_wstring(vector3.x).c_str());
	m_PosY.SetWindowTextW(std::to_wstring(vector3.y).c_str());
	m_PosZ.SetWindowTextW(std::to_wstring(vector3.z).c_str());

	auto rotate = m_pTrans->GetRotation();
	m_RotX.SetWindowTextW(std::to_wstring(rotate.x).c_str());
	m_RotY.SetWindowTextW(std::to_wstring(rotate.y).c_str());
	m_RotZ.SetWindowTextW(std::to_wstring(rotate.z).c_str());

	vector3 = m_pTrans->GetScale();
	m_ScaleX.SetWindowTextW(std::to_wstring(vector3.x).c_str());
	m_ScaleY.SetWindowTextW(std::to_wstring(vector3.y).c_str());
	m_ScaleZ.SetWindowTextW(std::to_wstring(vector3.z).c_str());
}


void Dialog_CTransformer::OnBnClickedTApply()
{
	m_pTrans->isEnable(m_isEnable.GetCheck());

	try
	{
		D3DXVECTOR3 vec3;
		m_PosX.GetWindowTextW(m_tempString);
		vec3.x = ::stof(m_tempString.GetBuffer());
		m_PosY.GetWindowTextW(m_tempString);
		vec3.y = ::stof(m_tempString.GetBuffer());
		m_PosZ.GetWindowTextW(m_tempString);
		vec3.z = ::stof(m_tempString.GetBuffer());
		m_pTrans->SetPosition(vec3);

		D3DXQUATERNION quat;
		m_RotX.GetWindowTextW(m_tempString);
		quat.x = ::stof(m_tempString.GetBuffer());
		m_RotY.GetWindowTextW(m_tempString);
		quat.y = ::stof(m_tempString.GetBuffer());
		m_RotZ.GetWindowTextW(m_tempString);
		quat.z = ::stof(m_tempString.GetBuffer());
		quat.w = 1.0f;
		m_pTrans->SetRotation(quat);

		m_ScaleX.GetWindowTextW(m_tempString);
		vec3.x = ::stof(m_tempString.GetBuffer());
		m_ScaleY.GetWindowTextW(m_tempString);
		vec3.y = ::stof(m_tempString.GetBuffer());
		m_ScaleZ.GetWindowTextW(m_tempString);
		vec3.z = ::stof(m_tempString.GetBuffer());
		m_pTrans->SetScale(vec3);
	}
	catch (...)
	{
		MessageBox(L"잘못 입력된 값이 있습니다.", L"삐빅-");
	}
}


void Dialog_CTransformer::OnBnClickedTReset()
{
	if (Dialog_Inspector::Instance->SelectComponent != nullptr)
	{
		UpdateEdit(Dialog_Inspector::Instance->SelectComponent);
	}
}
