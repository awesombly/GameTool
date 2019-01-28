#pragma once
#include "Renderer.h"


// 한 면만을 가진 평면을 나타내는 클래스
class RPlane : public Renderer
{
protected:
	virtual HRESULT Create() noexcept override;
public:
	//bool CollisionCheck(const POINT& point) const noexcept;
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	RPlane() = default;
public:
	RPlane(const wstring_view& myName, const wstring_view& srcName = L"",
			const string_view& vertexShaderName = "VS_Final", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual ~RPlane() = default;
};