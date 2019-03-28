#pragma once
#include "Light.h"
#include "Component.h"
#include "Sprite.h"

struct Vertex_PC
{
	D3DXVECTOR3 pos;		// 위치 좌표
	D3DXVECTOR4 col;		// 컬러값
};

struct Vertex_PNCT
{
	D3DXVECTOR3 pos;		// 위치 좌표
	D3DXVECTOR3 nor;		// 노말 벡터
	D3DXVECTOR4 col;		// 컬러값
	D3DXVECTOR2 tex;		// 텍스쳐 좌표
	D3DXVECTOR3 tan;		// 접선벡터
	bool operator == (const Vertex_PNCT& vertex)
	{
		return pos == vertex.pos && nor == vertex.nor && col == vertex.col && tex == vertex.tex ? true : false;
	}
};

//struct CB_Material
//{
//	float useLight;// : packoffset(c0);
//	float a;  float s; float d;
//	//bool useNormalMap;
//	//bool useEnviMap;
//	//bool useShadow;
//};

enum class EEnviType : char {
	Basic = 0, Fresnel, Refraction,
};

enum class ERenderType : char {
	Basic = 0, Particle, HeightMap, Instance, //, Line, Cube,
};


//class Camera;

// 랜더링 기능이 있는 컴포넌트
class Renderer : public Component
{
protected:
	Camera** m_ppCamera;							// 적용중인 카메라, 세팅시 해당 카메라 적용

	UINT m_indexCount = 0;							// 인덱스 갯수
	vector<WORD>		m_indexList;				// 인덱스 배열
	// 버퍼들
	ID3D11Buffer*		m_pVertexBuffer = nullptr;	// 정점 버퍼
	ID3D11Buffer*		m_pIndexBuffer  = nullptr;	// 인덱스 버퍼
	
	// 레이아웃, 쉐이더
	ID3D11InputLayout*	m_pInputLayout  = nullptr;	// 정점 레이아웃
	ID3D11VertexShader*	m_pVShader	    = nullptr;	// 정점 쉐이더
	ID3D11PixelShader*	m_pPShader	    = nullptr;	// 픽셀 쉐이더
	ID3D11ShaderResourceView* m_pEnviMap = nullptr;	  // 환경 맵
	ID3D11ShaderResourceView* m_pNormalMap = nullptr; // 노말 맵

	ID3D11Buffer*		m_pMatrixCBuffer = nullptr;		// 상수 버퍼 - 행렬
	ID3D11Buffer*		m_pMaterialCBuffer = nullptr;	// 쉐도우, 메테리얼 상수
	
	vector<Sprite>*		m_pSpriteList = nullptr;	// 스프라이트(좌표, 프레임) 리스트
	vector<Sprite>::iterator m_curSprite;			// 현재 스프라이트 이터레이터
	float				m_frameCount = 0.0f;

	bool m_isMultiTexture = false;	// 멀티 텍스쳐링 유무
	// 정점 버퍼 세팅용
	static UINT Stride;				// 정점 크기
	static UINT Offset;				// 시작 오프셋
public:
	CB_Material			m_cbMaterial;					// 쉐도우, 메테리얼 데이터

	vector<Vertex_PNCT>	m_vertexList;				// 정점 배열
	wstring				m_srcName;					// 텍스처 이름 (default=객체이름)
	ERenderType			m_eRenderType = ERenderType::Basic;
protected:
	// 생성자->초기값 세팅
	void SetInfo(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName = L"",
			const string_view& vertexShaderName = "VS_Final", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual HRESULT Create() noexcept { return S_OK; };
	// 버퍼 생성
	HRESULT CreateConstBuffer(const void* data, const UINT& dataSize, ID3D11Buffer** pConstBuffer);
	virtual HRESULT CreateVertexBuffer();
	virtual HRESULT CreateIndexBuffer(const UINT& indexCount, const bool& needInit = true);
	virtual void	UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept;

	virtual void UpdateTextureUV();
	virtual void EndSpriteEvent();
	// 접선 벡터 구하기
	void CreateTangentSpaceVectors(D3DXVECTOR3 *v0, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2,
								   const D3DXVECTOR2& uv0, const D3DXVECTOR2& uv1, const D3DXVECTOR2& uv2,
								   D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal) noexcept;
	void CreateTangentSpaceVectors(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3,
								   const float& _u1, const float& _v1, const float& _u2, const float& _v2, const float& _u3, const float& _v3,
								   D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal) noexcept;
	//DWORD EncodeVectorAsDWORDColor(D3DXVECTOR3* vVector) noexcept;
public:
	// 노말, 환경 등 설정
	void SetNormalMap(const wstring_view& normalTexName) noexcept;
	void SetEnviromentMap(const wstring_view& enviTexName, const EEnviType& eEnviType = EEnviType::Basic) noexcept;
	void SetLightRate(const float& value)	noexcept;
	void SetShadowRate(const float& value)	noexcept;
	float GetLightRate()			  const noexcept;
	float GetShadowRate()			  const noexcept;

	// 정점, 픽셀 쉐이더 및 레이아웃 지정
	void SetShaderLayout(const string_view& vertexName, const string_view& pixelName);
	void SetPixelShader(const string_view& pixelName);
	// 잡잡함수
	void SetSpriteList(const wstring_view& srcName = L"") noexcept;
	void SetSpriteList(vector<Sprite>* pSpriteList) noexcept;
	vector<Sprite>::iterator& GetCurSprite() noexcept;
	void SetCamera(Camera** pCamera);
	void SetColor(ID3D11DeviceContext* pDContext, const float& red, const float& green, const float& blue);
	void SetColor(ID3D11DeviceContext* pDContext, const D3DXVECTOR4& color);
	void SetAlpha(ID3D11DeviceContext* pDContext, const float& alpha);

	bool isMultiTexture() noexcept;
	void isMultiTexture(const bool& isMulti) noexcept;

	virtual bool PrevRender(ID3D11DeviceContext* pDContext)		noexcept;
	virtual bool PostRender(ID3D11DeviceContext* pDContext)		noexcept;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	virtual Component* cloneAddition() noexcept;		// clone 복사시 pointer 등 추가 처리
protected:
	friend class CoreMFC;
	friend class Raycast;
	Renderer() = default;
	Renderer(Renderer&) = default;
	Renderer(Renderer&&) = default;
	Renderer& operator = (const Renderer&) = default;
	Renderer& operator = (Renderer&&) = default;
public:
	Renderer(const wstring_view& myName, const string_view& vertexShaderName = "VS_Final", const string_view& pixelShaderName = "PS_Final") noexcept;
	~Renderer() noexcept = default;
};