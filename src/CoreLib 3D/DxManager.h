#pragma once
#include "SamplerState.h"
#include "RasterizerState.h"
#include "RenderDepthView.h"
#include "GameObject.h"
#include "Camera.h"
#include <filesystem>

// 상태 객체 열거형
enum class ERasterS : char {
	Current = 0,			// 적용중인 상태
	Basic,					// 기본값
	Wireframe,
	CullBack,
	CullFront,
	Line,
	DepthBias,
	size					// 잉여
};
enum class EBlendS : char {
	Current = 0,
	Basic,
	NoAlpha,
	//Red,
	//Green,
	//Blue,
	size
};
enum class EDepthS : char {
	Current = 0,
	Basic,
	D_Always_S_Off,
	D_Less_S_Always,
	D_Off_S_Less,
	D_Off_S_Off,
	D_Off_NoWrite,
	D_Less_NoWrite,
	size
};
enum class ESamTextureS : char {
	Current = 0,
	Basic,
	Wrap,
	Mirror,
	Clamp,
	Border,
};
enum class ESamFilterS : char {
	Current = 0,
	Basic,
	Linear,
	Anisotropic,
	Point,
	CompLinearPoint,
};
enum class EViewPort : char {
	Main = 0,
	MiniMap,
	SubView1,
	SubView2,
	SubView3,
	SubView4,
};


class Texture;

class DxManager : public IBasis, public ISingleton<DxManager>
{
private:
	ID3D11Device*				m_pD3dDevice;		// 그래픽카드 제어권(리소스 생성)
	ID3D11DeviceContext*		m_pD3dContext;		// 랜더링 처리 및 제어
	IDXGIFactory*				m_pGIFactory;		// Graphic Infrastructure 객체
	//D3D11_VIEWPORT				m_ViewPort;			// 뷰포트
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;	// SwapChain상태
	// ========================================================================
	map<EViewPort, D3D11_VIEWPORT>		   m_ViewPortList;	// 뷰포트 맵
	map<wstring, Texture*>				   m_TextureList;	// 텍스쳐 맵

	SamplerState						   m_SamplerState;	// 샘플러 상태 객체
	map<ERasterS, RasterizerState*>		   m_RasterList;	// 레스터라이저 상태 맵
	map<EBlendS, ID3D11BlendState*>		   m_BlenderList;	// 블랜더 상태 맵
	map<EDepthS, ID3D11DepthStencilState*> m_DepthList;		// 깊이	상태 맵
	HRESULT hr;			// 에러 체킹용
public:
	IDXGISwapChain*	m_pSwapChain;		// 전면 및 후면 버퍼 생성 및 제어
	RenderDepthView	m_RTDSView;			// 렌타뷰, 깊이-스텐실 뷰
	RenderDepthView m_RTDSViewShadow;	// 쉐도우 맵 생성용

	map<string, ID3D11VertexShader*>	   m_VShaderList;	// 정점 쉐이더 맵
	map<string, ID3D11PixelShader*>		   m_PShaderList;	// 픽셀 쉐이더 맵
	map<string, ID3D11GeometryShader*>	   m_GShaderList;	// 기하 쉐이더 맵
	map<string, ID3D11InputLayout*>		   m_LayoutList;	// 인풋 레이아웃 맵
private:
	// DX 초기화
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain();
	//HRESULT SetViewPort();
	// 상태들 생성
	HRESULT CreateRasterizerState();
	HRESULT CreateBlendState();
	HRESULT CreateDepthStencilState();
	// 쉐이더 로딩
	void	InitLoadShader() noexcept;
	HRESULT LoadVertexShader(const wstring_view& loadUrl, const string_view& funcName);
	HRESULT LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName);
	HRESULT LoadGeometryShader(const wstring_view& loadUrl, const string_view& funcName);
	// 텍스쳐, 정점 레이아웃 로딩
	Texture* LoadShaderResourceView(const filesystem::path& textureName, const bool& isFullPath);
	HRESULT  LoadInputLayout(ID3DBlob* pVertexBuf, const string_view& funcName);
public:
	static ID3D11Device*& GetDevice();
	static ID3D11DeviceContext*& GetDContext();
	// 뷰포트 생성, 적용
	void CreateViewPort(const EViewPort& viewEnum, const float& x, const float& y, const float& width, const float& height);
	void SetViewPort(const EViewPort& viewEnum);
	void ClearDepthStencilView();
	// 상태 설정
	void SetRasterizerState(const ERasterS& rasterEnum);
	void SetBlendState(const EBlendS& blendEnum);
	void SetDepthStencilState(const EDepthS& depthEnum, const UINT& stencilRef = 1);
	void SetSamplerState(const UINT& slotIndex, const ESamTextureS& textureEnum, const ESamFilterS& filterEnum = ESamFilterS::Current, const UINT& maxAnisotropy = 16, const D3D11_COMPARISON_FUNC& compFunc = (D3D11_COMPARISON_FUNC)0);

	Texture* GetTexture(const wstring_view& textureName, const bool& isFullPath = false);
	HRESULT	ResizeDevice(const int& width, const int& height);
		
	bool InitRTView() noexcept;
	virtual bool PrevRender() noexcept;
	virtual bool PostRender() noexcept;
	virtual bool Init() noexcept override;
	virtual bool Frame() noexcept override;
	virtual bool Render() noexcept override;
	virtual bool Release() noexcept override;
private:
	friend class ISingleton<DxManager>;
	friend class Dialog_Preset;
	DxManager() = default;
public:
	virtual ~DxManager() = default;
};