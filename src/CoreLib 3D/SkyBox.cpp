#include "SkyBox.h"
#include "ObjectManager.h"

SkyBox::SkyBox(const wstring_view& myName, const wstring_view& srcName,
			   const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, EComponent::Etc, srcName, vertexShaderName, pixelShaderName);
	Create();
	m_pSRViews[0] = ObjectManager::GetInstance().GetSpriteList(m_srcName + L"front.jpg")->front().m_pTexture->GetSRView();//m_curSprite->m_pTexture->GetSRView();
	m_pSRViews[1] = ObjectManager::GetInstance().GetSpriteList(m_srcName + L"right.jpg")->front().m_pTexture->GetSRView();
	m_pSRViews[2] = ObjectManager::GetInstance().GetSpriteList(m_srcName + L"back.jpg")->front().m_pTexture->GetSRView();
	m_pSRViews[3] = ObjectManager::GetInstance().GetSpriteList(m_srcName + L"left.jpg")->front().m_pTexture->GetSRView();
	m_pSRViews[4] = ObjectManager::GetInstance().GetSpriteList(m_srcName + L"up.jpg")->front().m_pTexture->GetSRView();
	m_pSRViews[5] = ObjectManager::GetInstance().GetSpriteList(m_srcName + L"down.jpg")->front().m_pTexture->GetSRView();
}

bool SkyBox::Init() noexcept
{
	RCube::Init();
	return true;
}

bool SkyBox::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;
	//RCube::Frame(spf, accTime);
	return true;
	spf; accTime;
}


bool SkyBox::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	//pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	for (int i = 0; i < 6; ++i)
	{
		pDContext->PSSetShaderResources(0, 1, &m_pSRViews[i]);
		pDContext->DrawIndexed(6, i * 6, 0);
	}

	return true;
}

bool SkyBox::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	DxManager::GetInstance().SetDepthStencilState(EDepthS::D_Off_NoWrite);
	DxManager::GetInstance().SetSamplerState(0, ESamTextureS::Mirror);
	RCube::Render(pDContext);
	DxManager::GetInstance().SetSamplerState(0, ESamTextureS::Current);
	DxManager::GetInstance().SetDepthStencilState(EDepthS::Current);
	return true;
}

bool SkyBox::Release()	noexcept
{
	RCube::Release();
	return true;
}


void SkyBox::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
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

Component* SkyBox::clone() noexcept
{
	//return (Component*)(new SkyBox(*this));
	return (new SkyBox(*this))->cloneAddition();
}