#include "Texture.h"
#include "DxManager.h"
#include <wrl.h>

Texture::Texture(ID3D11ShaderResourceView* pSRView) noexcept
{
	m_pSRView = pSRView;
	//ID3D11Resource* src;
	//ID3D11Texture2D* tex2D;
	//
	//m_pSRView->GetResource(&src);
	//src->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tex2D);
	//tex2D->GetDesc(&m_texDesc);
	//
	//src->Release();
	//tex2D->Release();
}

// 리소스 파일로부터 텍스쳐를 얻어 쉐이더리소스뷰로 만듬
Texture* Texture::CreateShaderResourceView(const wstring_view& srcUrl) noexcept
{
	D3DX11CreateShaderResourceViewFromFile(DxManager::GetDevice(), srcUrl.data(), NULL, NULL, &m_pSRView, NULL);
	if (m_pSRView == nullptr)
	{
		ErrorMessage(__FUNCTIONW__ + L" -> Load Error : "s + srcUrl.data() );
		delete this;
		return nullptr;
	}
	ID3D11Resource* src;
	ID3D11Texture2D* tex2D;
	
	m_pSRView->GetResource(&src);
	src->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tex2D);
	if (tex2D == nullptr)
	{
		ErrorMessage(__FUNCTIONW__ + L" -> Query Error : "s + srcUrl.data());
		return this;
	}
	tex2D->GetDesc(&m_texDesc);

	src->Release();
	tex2D->Release();

	return this;
}

// 콘텍스트 : 텍스쳐 설정
HRESULT	Texture::SetShaderResource(ID3D11DeviceContext* pDContext, const UINT& setSlot) noexcept
{
	//if (m_pSRView == nullptr)
	//{
		//ErrorMessage(__FUNCTION__ + " -> nullptr!"s);
		//return E_FAIL;
	//}
	pDContext->PSSetShaderResources(setSlot, 1, &m_pSRView);
	return S_OK;
}

UINT Texture::GetTexWidth() const noexcept
{
	return m_texDesc.Width;
}
UINT Texture::GetTexHeight() const noexcept
{
	return m_texDesc.Height;
}

ID3D11ShaderResourceView*& Texture::GetSRView() noexcept
{
	return m_pSRView;
}