#pragma once
#include "Renderer.h"


// n * m 개의 맵을 만듬, 높이맵 설정시 해당 텍스쳐 픽셀값으로 맞춤
class HeightMap : public Renderer
{
protected:
	D3DXVECTOR3 m_light = { -100, 100, 0 };		// 일단 조명
	// 전체 정점수, 페이스수, 셀 간격
	UINT m_vertexCount;
	UINT m_faceCount;
	float m_cellSize;
	// 전체 정점 행, 열 갯수
	UINT m_vertexCols;			
	UINT m_vertexRows;
	// 맵 전체 사이즈
	float m_mapHalfWidth;
	float m_mapHalfHeight;
	vector<UINT> m_vertexHeight;
	// 페이스들의 노말 벡터
	vector<D3DXVECTOR3>	  m_faceNormal;			
	// 각 정점을 공유하는 페이스 리스트
	vector<vector<UINT> > m_NormalLookupTable;	
protected:
	void GenFaceNormal()			 noexcept;	// 1. 모든 페이스에 대한 노말 생성
	void GenNormalLookupTable()		 noexcept;	// 2. 각 정점을 공유하는 모든 페이스 리스트 구축
	void GenVertexNormalFastLookup() noexcept;	// 3. 정점 계산
	void InitVertexNormalVector()	 noexcept;	// 각 정점의 노말 벡터 계산 (1+2+3)
	D3DXVECTOR3 GetNormalVector(const UINT& vectorIndex) noexcept;	// 해당 인덱스의 노말 벡터 반환
	virtual HRESULT CreateIndexBuffer(const WORD& cols, const WORD& rows, const UINT& indexCount) noexcept;
public:
	// 맵 생성
	virtual HRESULT Create(const WORD& cols, const WORD& rows, const float& cellSize = 1, const float& offsetX = 1.0f, const float& offsetY = 1.0f) noexcept;
	// 높이맵 불러오기
	virtual void SetHeightMap(ID3D11DeviceContext* pContext, const wstring_view& fileName, const wstring_view& directory) noexcept;
	// 높이맵 불러오기 + 생성
	virtual void CreateHeightMap(ID3D11DeviceContext* pContext, const wstring_view& fileName, const float& cellSize = 1, const float& offsetX = 1.0f, const float& offsetY = 1.0f, const wstring_view& directory = L"../../data/texture/") noexcept;
	// 해당 좌표 맵 정점 높이 반환
	virtual float GetMapHeight(D3DXVECTOR3 position, Renderer* pPlane = nullptr);
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	Component* cloneAddition() noexcept override;
protected:
	HeightMap() = default;
public:
	HeightMap(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName = L"",
			  const string_view& vertexShaderName = "VS_Final", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual ~HeightMap() = default;
};