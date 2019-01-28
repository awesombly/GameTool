#pragma once
#include "header.h"
#include "PlaneUI.h"

class RenderDepthView
{
private:
	enum { MaxMaskCount = 25, };
protected:

	//ID3D11Buffer* m_pMaskBuffer;
	//ID3D11ShaderResourceView* m_pMaskArray;
	//int m_iMask = 3;

	int m_RTViewCount;

	D3D11_TEXTURE2D_DESC		m_TexDesc;
	ID3D11Texture2D*			m_pTexTextures[2];	// 중간 랜더용 텍스처
	ID3D11RenderTargetView*		m_pTexRTViews[2];	// 텍스처 랜더, 노말+깊이
public:
	PlaneUI*					m_pScreen;

	ID3D11ShaderResourceView*	m_pTexSRViews[2];	// 중간 출력용 리소스
	ID3D11RenderTargetView*		m_pRTView;			// 생성된 후면 버퍼 중 랜더링 될 버퍼
	ID3D11DepthStencilView*		m_pDepthSView;		// 깊이 스텐실 뷰
protected:
	virtual HRESULT CreateResourceRTView(ID3D11DeviceContext* pDContext, IDXGISwapChain*& pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc) noexcept;
	virtual HRESULT CreateDSView(const DXGI_SWAP_CHAIN_DESC& swapDesc) noexcept;
	// 가우시안 블러에 쓰는듯
	virtual HRESULT CreateKernel() noexcept;
public:
	HRESULT Create(ID3D11DeviceContext* pDContext, IDXGISwapChain* pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc, const bool& isShadowMap = false) noexcept;
	void DrawView(ID3D11DeviceContext* pDContext)  noexcept;
	void ClearView(ID3D11DeviceContext* pDContext) noexcept;
	void Setting(ID3D11DeviceContext* pDContext) noexcept;
	vector<Sprite>::iterator& GetScreenSprite() const noexcept;

	RenderDepthView() = default;
	virtual ~RenderDepthView() = default;
};