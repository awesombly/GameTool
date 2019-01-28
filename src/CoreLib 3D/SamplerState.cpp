#include "SamplerState.h"
#include "DxManager.h"

SamplerState::SamplerState()
{
	//m_curFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//m_curTextureMode = D3D11_TEXTURE_ADDRESS_MIRROR;
	m_pSamplerState = nullptr;
}

void SamplerState::Frame(ID3D11DeviceContext* pDContext) noexcept
{
	if (Input::Get().GetKeyState(VK_F9) == EKeyState::DOWN)
	{
		switch (m_curTextureMode)
		{
		case D3D11_TEXTURE_ADDRESS_MIRROR:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_CLAMP;
		}break;
		case D3D11_TEXTURE_ADDRESS_CLAMP:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_BORDER;
		}break;
		case D3D11_TEXTURE_ADDRESS_BORDER:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_WRAP;
		}break;
		default:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_MIRROR;
		}break;
		}
		Update(pDContext);
	}
	if (Input::Get().GetKeyState(VK_F8) == EKeyState::DOWN)
	{
		switch (m_curFilter)
		{
		case D3D11_FILTER_MIN_MAG_MIP_POINT:
		{
			m_curFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}break;
		case D3D11_FILTER_MIN_MAG_MIP_LINEAR:
		{
			m_curFilter = D3D11_FILTER_ANISOTROPIC;
		}break;
		default:
			m_curFilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		}
		Update(pDContext);
	}
}

void SamplerState::Update(ID3D11DeviceContext* pDContext) noexcept
{
	if (m_pSamplerState != nullptr)
		m_pSamplerState->Release();

	D3D11_SAMPLER_DESC samplerDesc{ 0, };

	samplerDesc.AddressU = m_curTextureMode;
	samplerDesc.AddressV = m_curTextureMode;
	samplerDesc.AddressW = m_curTextureMode;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 1;
	samplerDesc.MipLODBias = 0;
	samplerDesc.Filter = m_curFilter;
	DxManager::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	pDContext->PSSetSamplers(0, 1, &m_pSamplerState);
}

void SamplerState::SetSate(ID3D11DeviceContext* pDContext, const UINT& slotIndex, const ESamTextureS& textureEnum, const ESamFilterS& filterEnum, const UINT& maxAnisotropy, const D3D11_COMPARISON_FUNC& compFunc) noexcept
{
	//if (textureEnum == ESamTextureS::Current && filterEnum == ESamFilterS::Current)
	//{
//		pDContext->PSSetSamplers(0, 1, &m_pSamplerState);
	//	return;
	//}

	D3D11_TEXTURE_ADDRESS_MODE texture;
	D3D11_FILTER filter;
	
	switch (filterEnum)
	{
	case ESamFilterS::Basic:
	case ESamFilterS::Linear:
	{
		filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}break;
	case ESamFilterS::Point:
	{
		filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	}break;
	case ESamFilterS::Anisotropic:
	{
		filter = D3D11_FILTER_ANISOTROPIC;
	}break;
	case ESamFilterS::CompLinearPoint:
	{
		filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	}break;
	default:
		filter = m_curFilter;
		break;
	}

	switch (textureEnum)
	{
	case ESamTextureS::Basic:
	case ESamTextureS::Mirror:
	{
		texture = D3D11_TEXTURE_ADDRESS_MIRROR;
	}break;
	case ESamTextureS::Clamp:
	{
		texture = D3D11_TEXTURE_ADDRESS_CLAMP;
	}break;
	case ESamTextureS::Wrap:
	{
		texture = D3D11_TEXTURE_ADDRESS_WRAP;
	}break;
	case ESamTextureS::Border:
	{
		texture = D3D11_TEXTURE_ADDRESS_BORDER;
	}break;
	default:
	{
		texture = m_curTextureMode;
	}break;
	}

	if (m_pSamplerState != nullptr)
		m_pSamplerState->Release();

	D3D11_SAMPLER_DESC samplerDesc{ 0, };

	samplerDesc.AddressU = texture;
	samplerDesc.AddressV = texture;
	samplerDesc.AddressW = texture;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MaxAnisotropy = maxAnisotropy;
	samplerDesc.ComparisonFunc = compFunc;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 1;
	samplerDesc.MipLODBias = 0;
	samplerDesc.Filter = filter;
	DxManager::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	pDContext->PSSetSamplers(slotIndex, 1, &m_pSamplerState);
}