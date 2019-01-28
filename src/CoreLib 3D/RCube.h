#pragma once
#include "Renderer.h"


// 6개 면을 가진 사각 큐브
class RCube : public Renderer
{
protected:
	virtual HRESULT Create() noexcept override;
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	RCube() = default;
public:
	RCube(const wstring_view& myName, const wstring_view& srcName = L"",
			const string_view& vertexShaderName = "VS_Final", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual ~RCube() = default;
};