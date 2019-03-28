#include "SkySphere.h"
#include "ObjectManager.h"


SkySphere::SkySphere(const UINT& slices, const wstring_view& myName, const wstring_view& srcName,
					 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_radius = 1.0f;
	m_slices = slices;
	SetInfo(myName, EComponent::Etc, srcName, vertexShaderName, pixelShaderName);
	Create();
	//m_pGShader = DxManager::GetInstance().m_GShaderList["GS_SO"];
}

bool SkySphere::Init() noexcept
{
	RSphere::Init();
	return true;
}

bool SkySphere::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	//RSphere::Frame(spf, accTime);
	return true;
	spf; accTime;
}


bool SkySphere::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	DxManager::GetInstance().SetDepthStencilState(EDepthS::D_Off_NoWrite);
	DxManager::GetInstance().SetSamplerState(0, ESamTextureS::Mirror);
	RSphere::Render(pDContext);
	DxManager::GetInstance().SetSamplerState(0, ESamTextureS::Current);
	DxManager::GetInstance().SetDepthStencilState(EDepthS::Current);
	return true;
}

bool SkySphere::Release()	noexcept
{
	RSphere::Release();
	return true;
}


void SkySphere::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	// 상수버퍼를 갱신
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	// 접근하고 수정하고 접근해제?
	pDContext->Map(m_pMatrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CB_VSMatrix* pConstData = (CB_VSMatrix*)MappedResource.pData;		// 바꿀 객체

	// 뷰 행렬 좌표 0으로
	static D3DXMATRIX matView;
	matView = (*m_ppCamera)->m_matView;
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	matView = matView * (*m_ppCamera)->m_matProj;
	D3DXMatrixTranspose(&matView, &matView);

	pConstData->m_matWorld = m_pParent->GetWorldMatrix();
	pConstData->m_matView = matView;
	pConstData->m_matProj = Matrix::Identity;

	pDContext->Unmap(m_pMatrixCBuffer, 0);
}


Component* SkySphere::clone() noexcept
{
	//return (Component*)(new SkySphere(*this));
	return (new SkySphere(*this))->cloneAddition();
}