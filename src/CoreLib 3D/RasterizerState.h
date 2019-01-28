#pragma once
#include "header.h"

class RasterizerState
{
protected:
	// 상태 변수들
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveType;
	D3D11_RASTERIZER_DESC	 m_RasterizerDesc;
	D3D11_CULL_MODE			 m_CullMode;
	D3D11_FILL_MODE			 m_FillMode;

	// 버텍스, 픽셀 쉐이더
	ID3D11RasterizerState*	 m_pRasterizerState;
public:

	// 레스터라이저 상태값 지정
	HRESULT SetRasterizerState(const D3D11_CULL_MODE& cullMode, const D3D11_FILL_MODE& fillMode, const D3D11_PRIMITIVE_TOPOLOGY& = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) noexcept;
	HRESULT SetRasterizerState(const D3D11_RASTERIZER_DESC& RSDesc) noexcept;

	// 프리미티브 유형 지정 
	void SetPrimitiveType(const D3D11_PRIMITIVE_TOPOLOGY& type) noexcept;

	// 레스터라이저 상태값 적용 
	HRESULT SetState(ID3D11DeviceContext* pDContext) noexcept;
public:
	RasterizerState() = default;
	~RasterizerState() = default;
};

