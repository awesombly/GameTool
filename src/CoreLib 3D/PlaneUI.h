#pragma once
#include "RPlane.h"


// UICamera로 직교 투영됨
class PlaneUI : public RPlane
{
protected:
	//virtual HRESULT Create() noexcept override;
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	PlaneUI() = default;
public:
	PlaneUI(const wstring_view& myName, const wstring_view& srcName = L"",
		 const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PS_Basic") noexcept;
	virtual ~PlaneUI() = default;
};