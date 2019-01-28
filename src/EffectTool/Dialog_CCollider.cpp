#include "stdafx.h"
#include "EffectTool.h"
#include "afxdialogex.h"
#include "Dialog_CCollider.h"
#include "Dialog_Inspector.h"

Dialog_CCollider* Dialog_CCollider::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_CCollider, CFormView)

Dialog_CCollider::Dialog_CCollider()
	: CFormView(IDD_DIALOG_Collider)
{
	Instance = this;
}


Dialog_CCollider::~Dialog_CCollider()
{}


void Dialog_CCollider::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_Radius, m_Radius);
	DDX_Control(pDX, IDC_C_MinX, m_MinX);
	DDX_Control(pDX, IDC_C_MinY, m_MinY);
	DDX_Control(pDX, IDC_C_MinZ, m_MinZ);
	DDX_Control(pDX, IDC_C_MaxX, m_MaxX);
	DDX_Control(pDX, IDC_C_MaxY, m_MaxY);
	DDX_Control(pDX, IDC_C_MaxZ, m_MaxZ);
	DDX_Control(pDX, IDC_C_DirectionX, m_DirectionX);
	DDX_Control(pDX, IDC_C_DirectionY, m_DirectionY);
	DDX_Control(pDX, IDC_C_DirectionZ, m_DirectionZ);
	DDX_Control(pDX, IDC_C_Damping, m_Damping);
	DDX_Control(pDX, IDC_C_Repulsion, m_Repulsion);
	DDX_Control(pDX, IDC_C_GravityScale, m_GravityScale);
	DDX_Control(pDX, IDC_P_isEnable, m_isEnable);
	DDX_Control(pDX, IDC_P_isEnable2, m_usePhysics);
}

Dialog_CCollider* Dialog_CCollider::CreateOne(CWnd* pParent)
{
	Dialog_CCollider* pForm = new Dialog_CCollider();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_CCollider, CFormView)

	ON_BN_CLICKED(ID_C_Apply, &Dialog_CCollider::OnBnClickedCApply)
	ON_BN_CLICKED(ID_C_Reset, &Dialog_CCollider::OnBnClickedCReset)
END_MESSAGE_MAP()

// ===================================================================

// 다이얼로그 갱신
void Dialog_CCollider::UpdateEdit(const Component* pComp) noexcept
{
	if (pComp == nullptr) return;
	m_pCollider = (Collider*)pComp;
	if (m_pCollider->m_eCollider == ECollider::Sphere)
	{
		m_MinX.EnableWindow(0);
		m_MinY.EnableWindow(0);
		m_MinZ.EnableWindow(0);
		m_MaxX.EnableWindow(0);
		m_MaxY.EnableWindow(0);
		m_MaxZ.EnableWindow(0);
	}
	else
	{
		m_MinX.EnableWindow(1);
		m_MinY.EnableWindow(1);
		m_MinZ.EnableWindow(1);
		m_MaxX.EnableWindow(1);
		m_MaxY.EnableWindow(1);
		m_MaxZ.EnableWindow(1);
	}
	m_isEnable.SetCheck(m_pCollider->isEnable());
	m_usePhysics.SetCheck(m_pCollider->usePhysics());

	m_MinX.SetWindowTextW(std::to_wstring(-1.0f).c_str());
	m_MinY.SetWindowTextW(std::to_wstring(-1.0f).c_str());
	m_MinZ.SetWindowTextW(std::to_wstring(-1.0f).c_str());
	m_MaxX.SetWindowTextW(std::to_wstring(1.0f).c_str());
	m_MaxY.SetWindowTextW(std::to_wstring(1.0f).c_str());
	m_MaxZ.SetWindowTextW(std::to_wstring(1.0f).c_str());

	D3DXVECTOR3 direction = m_pCollider->m_pPhysics->m_direction;
	m_DirectionX.SetWindowTextW(std::to_wstring(direction.x).c_str());
	m_DirectionY.SetWindowTextW(std::to_wstring(direction.y).c_str());
	m_DirectionZ.SetWindowTextW(std::to_wstring(direction.z).c_str());

	float tempFloat = m_pCollider->GetRadius();
	m_Radius.SetWindowTextW(std::to_wstring(tempFloat).c_str());

	tempFloat = m_pCollider->m_pPhysics->m_damping;
	m_Damping.SetWindowTextW(std::to_wstring(tempFloat).c_str());

	tempFloat = m_pCollider->m_pPhysics->m_repulsion;
	m_Repulsion.SetWindowTextW(std::to_wstring(tempFloat).c_str());

	tempFloat = m_pCollider->m_pPhysics->m_GravityScale;
	m_GravityScale.SetWindowTextW(std::to_wstring(tempFloat).c_str());
}

void Dialog_CCollider::OnBnClickedCApply()
{
	m_pCollider->isEnable(m_isEnable.GetCheck());
	m_pCollider->usePhysics(m_usePhysics.GetCheck());

	try
	{
		m_Radius.GetWindowTextW(m_tempString);
		m_pCollider->SetRadius(std::stof(m_tempString.GetBuffer()));

		D3DXVECTOR3 dir;
		m_DirectionX.GetWindowTextW(m_tempString);
		dir.x = std::stof(m_tempString.GetBuffer());
		m_DirectionY.GetWindowTextW(m_tempString);
		dir.y = std::stof(m_tempString.GetBuffer());
		m_DirectionZ.GetWindowTextW(m_tempString);
		dir.z = std::stof(m_tempString.GetBuffer());
		m_pCollider->SetDirectionForce(dir);

		m_Damping.GetWindowTextW(m_tempString);
		m_pCollider->m_pPhysics->m_damping = std::stof(m_tempString.GetBuffer());

		m_Repulsion.GetWindowTextW(m_tempString);
		m_pCollider->m_pPhysics->m_repulsion = std::stof(m_tempString.GetBuffer());

		m_GravityScale.GetWindowTextW(m_tempString);
		m_pCollider->m_pPhysics->m_GravityScale = std::stof(m_tempString.GetBuffer());

		if (m_pCollider->m_eCollider == ECollider::AABB)
		{
			D3DXVECTOR3 minPos, maxPos;
			m_MinX.GetWindowTextW(m_tempString);
			minPos.x = std::stof(m_tempString.GetBuffer());
			m_MinY.GetWindowTextW(m_tempString);
			minPos.y = std::stof(m_tempString.GetBuffer());
			m_MinZ.GetWindowTextW(m_tempString);
			minPos.z = std::stof(m_tempString.GetBuffer());

			m_MaxX.GetWindowTextW(m_tempString);
			maxPos.x = std::stof(m_tempString.GetBuffer());
			m_MaxY.GetWindowTextW(m_tempString);
			maxPos.y = std::stof(m_tempString.GetBuffer());
			m_MaxZ.GetWindowTextW(m_tempString);
			maxPos.z = std::stof(m_tempString.GetBuffer());

			((ColliderAABB*)m_pCollider)->SetMinMax(minPos, maxPos);
		}
		else if (m_pCollider->m_eCollider == ECollider::OBB)
		{
			D3DXVECTOR3 minPos, maxPos;
			m_MinX.GetWindowTextW(m_tempString);
			minPos.x = std::stof(m_tempString.GetBuffer());
			m_MinY.GetWindowTextW(m_tempString);
			minPos.y = std::stof(m_tempString.GetBuffer());
			m_MinZ.GetWindowTextW(m_tempString);
			minPos.z = std::stof(m_tempString.GetBuffer());

			m_MaxX.GetWindowTextW(m_tempString);
			maxPos.x = std::stof(m_tempString.GetBuffer());
			m_MaxY.GetWindowTextW(m_tempString);
			maxPos.y = std::stof(m_tempString.GetBuffer());
			m_MaxZ.GetWindowTextW(m_tempString);
			maxPos.z = std::stof(m_tempString.GetBuffer());

			((ColliderOBB*)m_pCollider)->SetMinMax(minPos, maxPos);
		}
	}
	catch (...)
	{
		MessageBox(L"잘못 입력된 값이 있습니다.", L"삐빅-");
	}
}


void Dialog_CCollider::OnBnClickedCReset()
{
	if (Dialog_Inspector::Instance->SelectComponent != nullptr)
	{
		UpdateEdit(Dialog_Inspector::Instance->SelectComponent);
	}
}
