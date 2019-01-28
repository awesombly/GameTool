#include "RasterizerState.h"
#include "DxManager.h"




HRESULT RasterizerState::SetRasterizerState(const D3D11_CULL_MODE& cullMode, const D3D11_FILL_MODE& fillMode, const D3D11_PRIMITIVE_TOPOLOGY& primitive) noexcept
{
	m_RasterizerDesc = { 0, };
	m_RasterizerDesc.DepthClipEnable = true;
	m_RasterizerDesc.CullMode = m_CullMode = cullMode;
	m_RasterizerDesc.FillMode = m_FillMode = fillMode;
	m_PrimitiveType = primitive;
	DxManager::GetDevice()->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);
	return S_OK;
}

HRESULT RasterizerState::SetRasterizerState(const D3D11_RASTERIZER_DESC& RSDesc) noexcept
{
	m_RasterizerDesc = RSDesc;
	DxManager::GetDevice()->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);
	return S_OK;
}

HRESULT RasterizerState::SetState(ID3D11DeviceContext* pDContext) noexcept
{
	//if (FAILED(SetRasterizerState(m_CullMode, m_FillMode)))
	//		return E_FAIL;
	pDContext->IASetPrimitiveTopology(m_PrimitiveType);
	pDContext->RSSetState(m_pRasterizerState);
	return S_OK;
}

void RasterizerState::SetPrimitiveType(const D3D11_PRIMITIVE_TOPOLOGY& type) noexcept
{
	m_PrimitiveType = type;
}