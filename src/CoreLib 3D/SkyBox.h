#pragma once
#include "RCube.h"

// 전체 배경용 스카이박스
class SkyBox : public RCube
{
protected:
	ID3D11ShaderResourceView* m_pSRViews[6];
protected:
	void UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept override;
public:
	bool PostRender(ID3D11DeviceContext* pDContext)		noexcept override;
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	SkyBox() = default;
public:
	SkyBox(const wstring_view& myName, const wstring_view& srcName = L"",
		   const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual ~SkyBox() = default;
};