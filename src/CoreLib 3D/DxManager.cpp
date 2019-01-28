#include "DxManager.h"
#include "WriteManager.h"
#include "Window.h"
#include "Texture.h"
#include <wrl.h>						// ComPtr

bool DxManager::Init() noexcept
{
	// DX 초기화
	if (FAILED(CreateDevice()) ||
		FAILED(CreateGIFactory()) ||
		FAILED(CreateSwapChain()) )
	{
		ErrorMessage(__FUNCTION__ + " -> DX 초기화 실패!"s);
		return false;
	}
	// 쉐이더 로드
	InitLoadShader();

	// 상태들 생성
	if (FAILED(CreateRasterizerState()) ||
		FAILED(CreateBlendState()) ||
		FAILED(CreateDepthStencilState()))
	{
		ErrorMessage(__FUNCTION__ + " -> 상태 객체 생성 실패!"s);
		return false;
	}
	// 상태들 기본값 적용
	SetRasterizerState(ERasterS::Basic);
	SetBlendState(EBlendS::Basic);
	SetDepthStencilState(EDepthS::Basic);
	SetSamplerState(0, ESamTextureS::Basic, ESamFilterS::Basic);

	// Writer 초기화
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	if (!WriteManager::GetInstance().Init(pBackBuffer))
	{
		ErrorMessage(__FUNCTION__ + " -> Write Init Failed!"s);
		return false;
	}

	// 화면 변경 막기
	if (FAILED(m_pGIFactory->MakeWindowAssociation(Window::m_hWnd, 
				DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		return false;
	}
	
	return true;
}

void DxManager::InitLoadShader() noexcept
{
	LoadVertexShader(L"../../data/shader/Final.hlsl", "VS_Final");
	LoadPixelShader(L"../../data/shader/Final.hlsl", "PS_Final");
	// 깊이맵 쉐도우
	LoadVertexShader(L"../../data/shader/ShadowMap.hlsl", "VS_Shadow");
	LoadVertexShader(L"../../data/shader/ShadowMap.hlsl", "VS_DepthMap");
	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_Shadow");
	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_DepthMap");
	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_NO_CMP");

	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_PlaneShadow");
	// Multi Render Target 
	LoadVertexShader(L"../../data/shader/RenderTarget.hlsl", "VS_MRT");
	LoadPixelShader(L"../../data/shader/RenderTarget.hlsl", "PS_MRT");
	LoadPixelShader(L"../../data/shader/RenderTarget.hlsl", "PS_MRT_None");
	LoadPixelShader(L"../../data/shader/RenderTarget.hlsl", "PS_MRT_Normal");
	// 정점 쉐이더 로드
	LoadVertexShader(L"../../data/shader/Shader.hlsl", "VS_Basic");
	LoadVertexShader(L"../../data/shader/Shader.hlsl", "VS_Light");
	LoadVertexShader(L"../../data/shader/Shader.hlsl", "VS_PC");
	// 픽셀 쉐이더 로드
	LoadPixelShader(L"../../data/shader/Shader.hlsl", "PS_Basic");
	LoadPixelShader(L"../../data/shader/Shader.hlsl", "PS_PC");
	LoadPixelShader(L"../../data/shader/Shader.hlsl", "PS_Frustum");
	// 구체 큐브맵
	LoadVertexShader(L"../../data/shader/CubeMap.hlsl", "VS_Sky");
	LoadPixelShader(L"../../data/shader/CubeMap.hlsl", "PS_Sky");
	// 인스턴싱
	LoadVertexShader(L"../../data/shader/Instance.hlsl", "VS_Particle");
	LoadVertexShader(L"../../data/shader/Instance.hlsl", "VS_Instance");
	LoadPixelShader(L"../../data/shader/Instance.hlsl", "PS_Instance");
	LoadPixelShader(L"../../data/shader/Instance.hlsl", "PS_SingleInstance");
	// 노말맵
	LoadVertexShader(L"../../data/shader/NormalMap.hlsl", "VS_Normal");
	LoadPixelShader(L"../../data/shader/NormalMap.hlsl", "PS_Normal");
	// 환경매핑
	LoadVertexShader(L"../../data/shader/EnviMap.hlsl", "VS_Envi");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_Envi");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_Fresnel");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_Refraction");
	LoadVertexShader(L"../../data/shader/EnviMap.hlsl", "VS_EnviDy");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_EnviDy");
	LoadGeometryShader(L"../../data/shader/EnviMap.hlsl", "GS_EnviDy");
	// 외곽선 출력
	LoadVertexShader(L"../../data/shader/OutLine.hlsl", "VS_OutLine");
	LoadPixelShader(L"../../data/shader/OutLine.hlsl", "PS_OutLine");
	// 기하 쉐이더 로드
	LoadGeometryShader(L"../../data/shader/CubeMap.hlsl", "GS_SO");
	// 스트림 아웃용 쉐이더
	LoadVertexShader(L"../../data/shader/CubeMap.hlsl", "VS_SO");
}

bool DxManager::InitRTView() noexcept
{
	// 랜더 타겟, 뷰포트 생성
	m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	m_RTDSViewShadow.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc, true);
	//m_RTDSViewCube.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	CreateViewPort(EViewPort::Main, 0, 0, (float)m_SwapChainDesc.BufferDesc.Width, (float)m_SwapChainDesc.BufferDesc.Height);
	CreateViewPort(EViewPort::MiniMap,
				   Window::getClientRect().right * 0.03f, Window::getClientRect().bottom * 0.03f,
				   Window::getClientRect().right * 0.3f, Window::getClientRect().bottom * 0.3f);
	m_RTDSView.ClearView(m_pD3dContext);
	SetViewPort(EViewPort::Main);
	return true;
}

bool DxManager::Frame() noexcept
{
	// 화면 랜더 상태 변경
	if (Input::GetKeyState(VK_F1) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Wireframe];
	}
	else if (Input::GetKeyState(VK_F2) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		SetBlendState(EBlendS::Basic);
		m_RTDSView.m_pScreen->SetPixelShader("PS_MRT_Normal");
	}
	else if (Input::GetKeyState(VK_F3) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		SetBlendState(EBlendS::Current);
		m_RTDSView.m_pScreen->SetPixelShader("PS_MRT_None");
	}
	else if (Input::GetKeyState(VK_F4) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		SetBlendState(EBlendS::Current);
		m_RTDSView.m_pScreen->SetPixelShader("PS_MRT");
	}

#pragma region StateKeyCheck
	// 레스터라이저 상태 변경
	if (Input::GetInstance().GetKeyState(VK_F5) == EKeyState::DOWN)
	{
		auto iter = std::find_if(m_RasterList.rbegin(), m_RasterList.rend(),
						 [&](auto iter) {
					return iter.second == m_RasterList[ERasterS::Current];
		});
		// 마지막 원소라면 기본값으로, 아님 다음꺼로
		if (iter == m_RasterList.rbegin())
			m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		else
			m_RasterList[ERasterS::Current] = (--iter)->second;
		SetRasterizerState(ERasterS::Current);
	}
	// 블렌더 상태 변경
	if (Input::GetInstance().GetKeyState(VK_F6) == EKeyState::DOWN)
	{
		auto iter = std::find_if(m_BlenderList.rbegin(), m_BlenderList.rend(),
						 [&](auto iter) {
					return iter.second == m_BlenderList[EBlendS::Current];
		});
		// 마지막 원소라면 기본값으로, 아님 다음꺼로
		if (iter == m_BlenderList.rbegin())
			m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::Basic];
		else
			m_BlenderList[EBlendS::Current] = (--iter)->second;
		SetBlendState(EBlendS::Current);
	}
	// 깊이-스텐실 상태 변경
	if (Input::GetInstance().GetKeyState(VK_F7) == EKeyState::DOWN)
	{
		auto iter = std::find_if(m_DepthList.rbegin(), m_DepthList.rend(),
								 [&](auto iter) {
			return iter.second == m_DepthList[EDepthS::Current];
		});
		// 마지막 원소라면 기본값으로, 아님 다음꺼로
		if (iter == m_DepthList.rbegin())
			m_DepthList[EDepthS::Current] = m_DepthList[EDepthS::Basic];
		else
			m_DepthList[EDepthS::Current] = (--iter)->second;
		SetDepthStencilState(EDepthS::Current);
	}
	// 샘플러 상태 변경 (F8, F9)
	m_SamplerState.Frame(m_pD3dContext);
#pragma endregion
	return true;
}

bool DxManager::PrevRender() noexcept
{
	// RenderTarget 설정
	//SetViewPort();
	WriteManager::Get().Begin();
	return true;
}

bool DxManager::PostRender() noexcept
{
	m_RTDSView.DrawView(m_pD3dContext);
	WriteManager::Get().End();
	//  백버퍼 내용 클리핑
	m_pSwapChain->Present(0, 0);					
	// 렌더, 깊이 뷰 클리어
	m_RTDSView.ClearView(m_pD3dContext);
	return true;
}

bool DxManager::Render() noexcept
{
	return true;
}

bool DxManager::Release() noexcept
{
	// DX 해제
	m_pSwapChain->Release();
	m_pGIFactory->Release();
	m_pD3dContext->Release();
	m_pD3dDevice->Release();

	WriteManager::Get().Release();
	return true;
}

void DxManager::CreateViewPort(const EViewPort& viewEnum, const float& x, const float& y, const float& width, const float& height)
{
	D3D11_VIEWPORT viewPort;
	// 뷰포트 속성 설정   
	viewPort.TopLeftX = x;				// 가로 시작 지점
	viewPort.TopLeftY = y;				// 세로 시작 지점
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	m_ViewPortList[viewEnum] = std::move(viewPort);
}

void DxManager::SetViewPort(const EViewPort& viewEnum)
{
	m_pD3dContext->RSSetViewports(1, &m_ViewPortList[viewEnum]);
}

void DxManager::ClearDepthStencilView()
{
	m_pD3dContext->ClearDepthStencilView(m_RTDSView.m_pDepthSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

Texture* DxManager::GetTexture(const wstring_view& textureName, const bool& isFullPath)
{
	// 없으면 리소스 로드
	auto iter = m_TextureList.find(textureName.data());
	if (iter == m_TextureList.end())
		return LoadShaderResourceView(textureName, isFullPath);
	if (iter->second == nullptr)
	{
		ErrorMessage(__FUNCTION__ + " -> : Texture is Null"s);
		return nullptr;
	}
	return iter->second;
}

Texture* DxManager::LoadShaderResourceView(const filesystem::path& textureName, const bool& isFullPath)
{
	Texture* pTexture = nullptr;
	if (isFullPath)
	{
		pTexture = (new Texture())->CreateShaderResourceView(textureName.c_str());
	}
	else
	{
		pTexture = (new Texture())->CreateShaderResourceView(L"../../data/texture/"s + textureName.c_str());
	}
	if (pTexture == nullptr)
		return nullptr;
	else
		return m_TextureList[textureName.filename()] = pTexture;
}

HRESULT DxManager::LoadVertexShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// 정점 쉐이더 생성
	ID3D11VertexShader* pVShader = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf  = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "vs_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	if (FAILED(hr))
	{
		ErrorMessage(__FUNCTION__ + " -> : Shader Load Error : "s + funcName.data() , true);
		return E_FAIL;
	}
	m_pD3dDevice->CreateVertexShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &pVShader);
	m_VShaderList[funcName.data()] = pVShader;

	if (FAILED(LoadInputLayout(pOutputBuf, funcName)))
	{
		ErrorMessage(__FUNCTION__ + " -> 레이아웃 생성 실패 : "s + funcName.data() , true);
		return E_FAIL;
	}

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// 픽셀 쉐이더 생성
	ID3D11PixelShader* pPShader;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "ps_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	if (FAILED(hr))
	{
		ErrorMessage(__FUNCTION__ + " -> Shader Load Error!"s, true);
		return E_FAIL;
	}
	m_pD3dDevice->CreatePixelShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &pPShader);
	m_PShaderList[funcName.data()] = pPShader;

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadGeometryShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// 기하 쉐이더 생성
	ID3D11GeometryShader* pGShader = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "gs_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	if (FAILED(hr))
	{
		ErrorMessage(__FUNCTION__ + " -> Shader Load Error : "s + funcName.data(), true);
		return E_FAIL;
	}
	m_pD3dDevice->CreateGeometryShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &pGShader);
	m_GShaderList[funcName.data()] = pGShader;

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadInputLayout(ID3DBlob* pVertexBuf, const string_view& funcName)
{
	ID3D11InputLayout* pInputLayout = nullptr;
	if (funcName._Equal("VS_PC"))
	{
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			// 의미구조,     , 타입,                       시작바이트, 입력타입
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if (FAILED(m_pD3dDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
			return E_FAIL;
	}
	else if (funcName._Equal("VS_Instance") || funcName._Equal("VS_Particle"))
	{
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			// 의미구조,     , 타입,                       시작바이트, 입력타입
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 48, D3D11_INPUT_PER_VERTEX_DATA,   0 },

			{ "mTransform",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "mTransform",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "mTransform",	2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "mTransform",	3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXTURE",	0, DXGI_FORMAT_R32_FLOAT,			1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "InsCOLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 68, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};
		if (FAILED(m_pD3dDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
			return E_FAIL;
	}
	else
	{
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			// 의미구조,     , 타입,                       시작바이트, 입력타입
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if (FAILED(m_pD3dDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
			return E_FAIL;
	}
	m_LayoutList[funcName.data()] = pInputLayout;
	return S_OK;
}


HRESULT DxManager:: CreateRasterizerState()
{
	for (int i = 1; i < (int)ERasterS::size; ++i)
		m_RasterList.try_emplace((ERasterS)i, new RasterizerState());

	// 랜더링 할 프리미티브, 레스터라이저 타입 지정(점, 선, 삼각형, 뿌릴 속성)
	m_RasterList[ERasterS::Basic]->SetRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_RasterList[ERasterS::Wireframe]->SetRasterizerState(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
	m_RasterList[ERasterS::CullFront]->SetRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
	m_RasterList[ERasterS::CullBack]->SetRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	m_RasterList[ERasterS::Line]->SetRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D11_RASTERIZER_DESC RSDesc = { 0, };
	RSDesc.DepthClipEnable = true;
	RSDesc.FillMode = D3D11_FILL_SOLID;
	RSDesc.CullMode = D3D11_CULL_BACK;
	RSDesc.DepthBias = 2000;
	RSDesc.DepthBiasClamp = 0.0f;
	RSDesc.SlopeScaledDepthBias = 1.0f;
	m_RasterList[ERasterS::DepthBias]->SetRasterizerState(RSDesc);
	m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
	return S_OK;
}

HRESULT DxManager::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc = { 0, };
	// 블랜딩 여부
	blendDesc.AlphaToCoverageEnable = false;
	// 8개 배열중 현재는 0번만 사용
	blendDesc.RenderTarget[0].BlendEnable = true;
	// Src : 대상, Desc : 뿌려질 위치, Op : 연산자
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// 알파값
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// 적용할 컬러 RGB
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Basic]);
	// =============================================================================
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED;
	//m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Red]);
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_GREEN;
	//m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Green]);
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_BLUE;
	//m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Blue]);

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::NoAlpha]);
	
	m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::Basic];
	return S_OK;
}

HRESULT DxManager::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC dsDescDepth = { 0, };
	// 깊이버퍼 상태값 세팅
	dsDescDepth.DepthEnable = TRUE;
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;			// 깊이 비교 조건
	// 스텐실 상태 지정
	dsDescDepth.StencilEnable = FALSE;
	dsDescDepth.StencilReadMask = 1;// 0xff;
	dsDescDepth.StencilWriteMask = 1;// 0xff;
	// 전면-후면 페이스 속성, 페이스 노말벡터가 카메라를 볼때 전면이 됨
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// 보이는 면의 속성에 따라 적용
	dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::Basic]);
	dsDescDepth.StencilEnable = true;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Less_S_Always]);
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Less_NoWrite]);
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.StencilEnable = false;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Always_S_Off]);
	dsDescDepth.DepthEnable = false;
	dsDescDepth.StencilEnable = true;
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Off_S_Less]);
	dsDescDepth.StencilEnable = false;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Off_S_Off]);
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Off_NoWrite]);

	m_DepthList[EDepthS::Current] = m_DepthList[EDepthS::Basic];
	return S_OK;
}


void DxManager::SetRasterizerState(const ERasterS& rasterEnum)
{
	m_RasterList[rasterEnum]->SetState(m_pD3dContext);
}
void DxManager::SetBlendState(const EBlendS& blendEnum)
{
	m_pD3dContext->OMSetBlendState(m_BlenderList[blendEnum], nullptr, (UINT)-1);
}
void DxManager::SetDepthStencilState(const EDepthS& depthEnum, const UINT& stencilRef)
{
	m_pD3dContext->OMSetDepthStencilState(m_DepthList[depthEnum], stencilRef);
}
void DxManager::SetSamplerState(const UINT& slotIndex, const ESamTextureS& textureEnum, const ESamFilterS& filterEnum, const UINT& maxAnisotropy, const D3D11_COMPARISON_FUNC& compFunc)
{
	m_SamplerState.SetSate(m_pD3dContext, slotIndex, textureEnum, filterEnum, maxAnisotropy, compFunc);
}

HRESULT	DxManager::ResizeDevice(const int& width, const int& height)
{
	if (m_pD3dDevice == nullptr)	return E_FAIL;

	WriteManager::Get().ReleaseDeviceResource();
	// 랜더타겟뷰 초기화, 해제
	m_pD3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_pD3dContext->OMSetDepthStencilState(nullptr, 0);
	if (m_RTDSView.m_pRTView != nullptr)	
		m_RTDSView.m_pRTView->Release();
	if (m_RTDSView.m_pDepthSView != nullptr)
		m_RTDSView.m_pDepthSView->Release();
	///if (m_RTDSViewCube.m_pRTView != nullptr)
	///	m_RTDSViewCube.m_pRTView->Release();
	///if (m_RTDSViewCube.m_pDepthSView != nullptr)
	///	m_RTDSViewCube.m_pDepthSView->Release();
	// 백버퍼 크기 조절
	m_SwapChainDesc.BufferDesc.Width = width;
	m_SwapChainDesc.BufferDesc.Height = height;
	m_SwapChainDesc.Windowed = false;
	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height,
								m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);
	// 랜더타겟뷰 생성, 뷰포트 세팅
	m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	///m_RTDSViewCube.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	SetViewPort(EViewPort::Main);

	//GetClientRect(Window::m_hWnd, &Window::getClientRect());
	Window::ResizeWindow({0, 0, width, height});

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackBuffer);
	WriteManager::Get().CreateDeviceResource(pBackBuffer);
	pBackBuffer->Release();

	return S_OK;
}

// Dx 초기 생성
HRESULT DxManager::CreateDevice()
{
	D3D_FEATURE_LEVEL pInFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	// Alt + Enter 막으려면 첫번째 인자 채우라캄
	//	디바이스 생성(어댑터:그래픽 드라이버, 디바이스 드라이버 타입, 소프트웨어 모듈, 디바이스 생성 플래그
	return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
							 // 사용 할 드라이버 버전, 버전 갯수, 현재 SDK 버전,
							 &pInFeatureLevel, 1, D3D11_SDK_VERSION,
							 // 반환 받을 Device 인터페이스, 반환 받을 드라이버 버전
							 &m_pD3dDevice, nullptr,
							 // 반환 받을 DeviceContext 인터페이스
							 &m_pD3dContext);
}

HRESULT DxManager::CreateGIFactory()
{
	// GIFactory 생성(DXGI) : GPU 및 관련 기기들 정보를 가지고 제어하는 범용 인터페이스?
	// 해당 객체에 대한 GUID 번호를 반환해주고, 인터페이스에 해당 번호에 대한 객체를 반환해줌

	// 디바이스 인터페이스 얻고, 인터페이스에서 어텝터 얻고, 어텝터에서 GI팩토리 얻음?
	Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice;
	if (FAILED(m_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice) ))
		return E_FAIL;

	Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter;
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter)) ||
		FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_pGIFactory)) ||
		FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory)))
	{
		ErrorMessage(__FUNCTION__ + " -> InitError !"s);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT DxManager::CreateSwapChain()
{
	if (m_pGIFactory == nullptr)
		return E_FAIL;

	// 스왑 체인 생성
	m_SwapChainDesc = { 0, };
	m_SwapChainDesc.BufferCount = 1;									// 백버퍼 갯수?
	m_SwapChainDesc.OutputWindow = Window::m_hWnd;						// 보여질 윈도우 핸들
	m_SwapChainDesc.Windowed = true;									// 일반=true, 풀 스크린=false
	// 백 버퍼 세팅
	m_SwapChainDesc.BufferDesc.Width = Window::getClientRect().right;	// 해상도 너비
	m_SwapChainDesc.BufferDesc.Height = Window::getClientRect().bottom;	// 해상도 높이
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;				// 재생빈도(주사율)
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;				// 분자 / 분모
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// 백버퍼 출력픽셀 포멧(Unsigned Normalize)
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 표면 및 리소스 생성 옵션, 버퍼 사용 용도
	// 멀티 샘플링
	m_SwapChainDesc.SampleDesc.Count = 1;								// 픽셀 단위 멀티 샘플링 수(1=X),  ex)안티 앨리어싱
	//SwapDesc.SampleDesc.Quality = 0;									// 이미지 품질 수준

	//SwapDesc.BufferDesc.ScanlineOrdering;								// 주사선 렌더링 모드 형식
	//SwapDesc.BufferDesc.Scaling;										// 크기 조정 모드
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;			// 페이지 플리핑(Present)후 백버퍼 보존 여부
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// 스왑 체인의 동작 결정(전체 화면시 해상도 변경x?)

	return m_pGIFactory->CreateSwapChain(m_pD3dDevice, &m_SwapChainDesc, &m_pSwapChain);
}

//HRESULT DxManager::SetViewPort()
//{
//	// 뷰포트 속성 설정   
//	m_ViewPort.TopLeftX = 0;				// 가로 시작 지점
//	m_ViewPort.TopLeftY = 0;				// 세로 시작 지점
//	m_ViewPort.Width = (float)m_SwapChainDesc.BufferDesc.Width;
//	m_ViewPort.Height = (float)m_SwapChainDesc.BufferDesc.Height;
//	m_ViewPort.MinDepth = 0.0f;
//	m_ViewPort.MaxDepth = 1.0f;
//
//	m_pD3dContext->RSSetViewports(1, &m_ViewPort);
//	return S_OK;
//}


ID3D11Device*& DxManager::GetDevice()
{
	return GetInstance().m_pD3dDevice;
}

ID3D11DeviceContext*& DxManager::GetDContext()
{
	return GetInstance().m_pD3dContext;
}