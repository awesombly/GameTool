#pragma once
#include "Renderer.h"


// 구체를 표현한 랜더러 클래스
class RSphere : public Renderer
{
protected:
	float m_radius;
	UINT  m_slices;

	ID3D11GeometryShader* m_pGShader;
protected:
	virtual HRESULT Create() noexcept override;
	void CreateSphere(const UINT& slices, const float& radius);
	void MakeSpherePoint(vector<D3DXVECTOR3>* pSpherePoints, const UINT& slices);
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	RSphere() = default;
public:
	RSphere(const UINT& slices, const wstring_view& myName, const wstring_view& srcName = L"",
		  const string_view& vertexShaderName = "VS_Final", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual ~RSphere() = default;
};