#pragma once
#include "header.h"

// 불러온 텍스쳐 저장용
class Texture
{
private:
	ID3D11ShaderResourceView* m_pSRView;
	D3D11_TEXTURE2D_DESC	  m_texDesc;
public:
	Texture*	CreateShaderResourceView(const wstring_view& srcUrl) noexcept;
	HRESULT		SetShaderResource(ID3D11DeviceContext* pDContext, const UINT& setSlot = 0) noexcept;
	ID3D11ShaderResourceView*& GetSRView() noexcept;
	UINT GetTexWidth()  const noexcept;
	UINT GetTexHeight() const noexcept;

	Texture(ID3D11ShaderResourceView* pSRView) noexcept;
	Texture() = default;
	~Texture() = default;
};