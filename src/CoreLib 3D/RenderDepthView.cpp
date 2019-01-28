#include "RenderDepthView.h"
#include "ObjectManager.h"
#include <wrl.h>						// ComPtr



HRESULT RenderDepthView::Create(ID3D11DeviceContext* pDContext, IDXGISwapChain* pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc, const bool& isShadowMap) noexcept
{
	if (m_pScreen != nullptr)
	{
		ObjectManager::Get().RemoveObject(m_pScreen->m_pParent);
		m_pScreen = nullptr;
	}
	if (isShadowMap)
	{
		m_RTViewCount = 1;
		m_pScreen = new PlaneUI(L"ScreenView", L"None.png", "VS_Basic", "PS_MRT_None");
	}
	else
	{
		m_RTViewCount = 2;
		m_pScreen = new PlaneUI(L"ScreenView", L"None.png", "VS_MRT", "PS_MRT_None");
	}
	m_pScreen->SetCamera(&ObjectManager::Cameras[ECamera::Screen]);

	GameObject* pObject = new GameObject(L"ScreenView", m_pScreen);
	pObject->isStatic(true);
	// 깊이뷰, 텍스쳐:리소스:랜더 타겟 생성
	if(FAILED(CreateResourceRTView(pDContext, pSwapChain, swapDesc)) ||
	   FAILED(CreateDSView(swapDesc)))
		return E_FAIL;
	// 리소스 장착
	vector<Sprite>* sprites = new vector<Sprite>();
	sprites->emplace_back(Sprite(new Texture(m_pTexSRViews[0]), 0.0f));
	if (m_RTViewCount == 2)
	{
		sprites->emplace_back(Sprite(new Texture(m_pTexSRViews[1]), 0.0f));
		m_pScreen->isMultiTexture(true);
	}
	m_pScreen->SetSpriteList(sprites);
	return S_OK;
}

void RenderDepthView::DrawView(ID3D11DeviceContext* pDContext) noexcept
{
	DxManager::GetInstance().SetRasterizerState(ERasterS::CullBack);
	pDContext->OMSetRenderTargets(1, &m_pRTView, nullptr);
	// 원본 랜더 타겟에 텍스쳐 랜더 타겟을 드로우
	m_pScreen->PrevRender(pDContext);
	m_pScreen->PostRender(pDContext);
	DxManager::GetInstance().SetRasterizerState(ERasterS::Current);
}

void RenderDepthView::ClearView(ID3D11DeviceContext* pDContext) noexcept
{
	// 버퍼 클리어
	static float BackColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	static float BackDepthColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pDContext->ClearRenderTargetView(m_pTexRTViews[0], BackColor);
	pDContext->ClearRenderTargetView(m_pTexRTViews[1], BackDepthColor);
	// 깊이-스텐실 버퍼를 초기화, 초기화 대상 플래그, 깊이값, 스텐실값
	pDContext->ClearDepthStencilView(m_pDepthSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// 랜더 타겟 뷰 설정(텍스쳐)
	pDContext->OMSetRenderTargets(m_RTViewCount, m_pTexRTViews, m_pDepthSView);
}

void RenderDepthView::Setting(ID3D11DeviceContext* pDContext) noexcept
{
	// 랜더 타겟 뷰 설정(텍스쳐)
	pDContext->OMSetRenderTargets(m_RTViewCount, m_pTexRTViews, m_pDepthSView);
}

HRESULT RenderDepthView::CreateResourceRTView(ID3D11DeviceContext* pDContext, IDXGISwapChain*& pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	// 랜더 타겟 생성
	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)) ||
		FAILED(DxManager::GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTView)))
		return E_FAIL;
	// 텍스쳐 랜더 타겟 생성
	m_TexDesc = { 0, };
	m_TexDesc.Width  = swapDesc.BufferDesc.Width;
	m_TexDesc.Height = swapDesc.BufferDesc.Height;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.SampleDesc.Count   = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TexDesc.Usage  = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.ArraySize = 1;
	// 쉐이더 리소스 뷰 데스크
	D3D11_SHADER_RESOURCE_VIEW_DESC SRViewDesc = { 0, };
	SRViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRViewDesc.Texture2D.MostDetailedMip = 0;
	SRViewDesc.Texture2D.MipLevels = m_TexDesc.MipLevels;
	// 랜더 타켓 데스크
	D3D11_RENDER_TARGET_VIEW_DESC RTViewDesc = { 0, };
	RTViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTViewDesc.Texture2D.MipSlice = 0;

	if (m_RTViewCount == 2)
	{
		if (m_pTexTextures[0] != nullptr)
		{
			m_pTexTextures[0]->Release();
			m_pTexSRViews[0]->Release();
			m_pTexRTViews[0]->Release();
		}
		if (m_pTexTextures[1] != nullptr)
		{
			m_pTexTextures[1]->Release();
			m_pTexSRViews[1]->Release();
			m_pTexRTViews[1]->Release();
		}
		if (FAILED(DxManager::GetDevice()->CreateTexture2D(&m_TexDesc, nullptr, &m_pTexTextures[0])) ||
			FAILED(DxManager::GetDevice()->CreateTexture2D(&m_TexDesc, nullptr, &m_pTexTextures[1])) ||
			FAILED(DxManager::GetDevice()->CreateShaderResourceView(m_pTexTextures[0], &SRViewDesc, &m_pTexSRViews[0])) ||
			FAILED(DxManager::GetDevice()->CreateShaderResourceView(m_pTexTextures[1], &SRViewDesc, &m_pTexSRViews[1])) ||
			FAILED(DxManager::GetDevice()->CreateRenderTargetView(m_pTexTextures[0], &RTViewDesc, &m_pTexRTViews[0])) ||
			FAILED(DxManager::GetDevice()->CreateRenderTargetView(m_pTexTextures[1], &RTViewDesc, &m_pTexRTViews[1])))
		{
			return E_FAIL;
	   }
	}
	else
	{
		if (m_pTexTextures[0] != nullptr)
		{
			m_pTexTextures[0]->Release();
			m_pTexSRViews[0]->Release();
			m_pTexRTViews[0]->Release();
		}
		if (FAILED(DxManager::GetDevice()->CreateTexture2D(&m_TexDesc, nullptr, &m_pTexTextures[0])) ||
			FAILED(DxManager::GetDevice()->CreateShaderResourceView(m_pTexTextures[0], &SRViewDesc, &m_pTexSRViews[0])) ||
			FAILED(DxManager::GetDevice()->CreateRenderTargetView(m_pTexTextures[0], &RTViewDesc, &m_pTexRTViews[0])))
		{
			return E_FAIL;
		}
	}
	return S_OK;
	pDContext;
}

HRESULT RenderDepthView::CreateDSView(const DXGI_SWAP_CHAIN_DESC& swapDesc) noexcept
{
	// 텍스쳐 생성
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexDSView = nullptr;
	D3D11_TEXTURE2D_DESC depthTexDesc = { 0, };
	depthTexDesc.Width = swapDesc.BufferDesc.Width;
	depthTexDesc.Height = swapDesc.BufferDesc.Height;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;

	// 깊이 스텐실 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC DSViewDesc = { 0, };
	DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSViewDesc.Texture2D.MipSlice = 0;

	if (m_RTViewCount == 2)
	{
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// 생성 타입
		
		DSViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (FAILED(DxManager::GetDevice()->CreateTexture2D(&depthTexDesc, nullptr, &pTexDSView)) ||
			FAILED(DxManager::GetDevice()->CreateDepthStencilView(pTexDSView.Get(), &DSViewDesc, &m_pDepthSView)))
		{
			return E_FAIL;
		}
	}
	else
	{
		depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		DSViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		// 깊이맵일시만
		D3D11_SHADER_RESOURCE_VIEW_DESC SRViewDesc = { 0, };
		SRViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		SRViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRViewDesc.Texture2D.MostDetailedMip = 0;
		SRViewDesc.Texture2D.MipLevels = 1;

		if (FAILED(DxManager::GetDevice()->CreateTexture2D(&depthTexDesc, nullptr, &pTexDSView)) ||
			FAILED(DxManager::GetDevice()->CreateDepthStencilView(pTexDSView.Get(), &DSViewDesc, &m_pDepthSView)) ||
			FAILED(DxManager::GetDevice()->CreateShaderResourceView(pTexDSView.Get(), &SRViewDesc, &m_pTexSRViews[0])))
			//FAILED(DxManager::GetDevice()->CreateShaderResourceView(m_pTexDSView, &SRViewDesc, &m_pTexSRViews[1])))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT RenderDepthView::CreateKernel() noexcept
{
	//// 버퍼 생성
	//D3D11_BUFFER_DESC vbDesc =
	//{
	//	MaxMaskCount * MaxMaskCount * sizeof(float),
	//	D3D11_USAGE_DEFAULT,
	//	D3D11_BIND_SHADER_RESOURCE,
	//	0
	//};
	//DxManager::GetDevice()->CreateBuffer(&vbDesc, NULL, &m_pMaskBuffer);
	//
	//D3D11_SHADER_RESOURCE_VIEW_DESC SRViewDesc = { 0, };
	//SRViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//SRViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//SRViewDesc.Buffer.ElementOffset = 0;
	//SRViewDesc.Buffer.ElementWidth = MaxMaskCount * MaxMaskCount;
	//DxManager::GetDevice()->CreateShaderResourceView(m_pMaskBuffer, &SRViewDesc, &m_pMaskArray);
	

	//// 커널 연산
	//float sigma = 1.0f;
	//m_iMask = iRadius * 2 + 1;
	//m_pfKernelMask = new float[m_iMask*m_iMask];
	//float mean = m_iMask / 2.0f;
	//float sum = 0.0f;
	//
	//for (int x = 0; x < m_iMask; ++x)
	//{
	//	for (int y = 0; y < m_iMask; ++y)
	//	{
	//		m_pfKernelMask[x*m_iMask + y] = expf(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))
	//			/ (2 * 3.141592f * sigma * sigma);
	//
	//		sum += m_pfKernelMask[x*m_iMask + y];
	//	}
	//}
	//for (int x = 0; x < m_iMask; ++x)
	//{
	//	for (int y = 0; y < m_iMask; ++y)
	//	{
	//		m_pfKernelMask[x*m_iMask + y] /= sum;
	//	}
	//}
	return S_OK;
}




vector<Sprite>::iterator& RenderDepthView::GetScreenSprite() const noexcept
{
	return m_pScreen->GetCurSprite();
}
