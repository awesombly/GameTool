#pragma once
#include "Renderer.h"
#include "Instancer.h"

#pragma pack(push, 1)
struct InstanceData 
{
	D3DXMATRIX matWorld;
	float numTexture;
	D3DXVECTOR4 color;
	//bool isEnable;
};
#pragma pack(pop)


// 같은 종류의 렌더링 객체들을 인스턴싱
class InstanceRenderer : public Renderer
{
private:
	forward_list<Instancer*>			m_objectList; // 등록된 객체들
	forward_list<Instancer*>::iterator  m_curObject;

	ID3D11Buffer*		 m_pBuffers[2];				  // 정점 + 인스턴스
	ID3D11Buffer*		 m_pInstanceBuffer = nullptr; // 데이터 등록할 버퍼
	vector<InstanceData> m_dataList;				  // 버퍼 등록 데이터

	// + 스프라이트는 각자 돌아야댐 
	//	-> 스프라이트(이펙트) 형식 사용시 현재 스프라이트 카운터 받도록

	// 정점 버퍼 세팅용
	UINT Strides[2];				// 정점 크기
	UINT Offsets[2];				// 시작 오프셋
public:
	//
private:
	HRESULT Create() noexcept override;
	// 버퍼 생성
	HRESULT CreateInstanceBuffer() noexcept;
	void	UpdateInstanceBuffer(ID3D11DeviceContext* pDContext) noexcept;
	void	UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept override;

	Component* cloneAddition() noexcept override;
public:
	void AddInstance(Instancer* pObject)				noexcept;
	void RemoveInstance(Instancer* pObject)			noexcept;

	bool PostRender(ID3D11DeviceContext* pDContext)		noexcept override;
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept override;
protected:
	friend class AseParser;
	friend class MaxImporter;
	//InstanceRenderer() = default;
public:
	InstanceRenderer(const wstring_view& myName, const string_view& vertexShaderName = "VS_Instance", const string_view& pixelShaderName = "PS_Instance") noexcept;
	virtual ~InstanceRenderer() = default;
};