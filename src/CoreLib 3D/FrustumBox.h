#pragma once
#include "Renderer.h"


// 카메라 프러스텀
class FrustumBox : public Renderer
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
	FrustumBox() = default;
public:
	FrustumBox(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName = L"",
			   const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PS_Frustum") noexcept;
	virtual ~FrustumBox() = default;
};