#pragma once
#include "RSphere.h"

// 전체 배경용 스카이구체
class SkySphere : public RSphere
{
protected:
	void UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept override;
public:
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	SkySphere() = default;
public:
	SkySphere(const UINT& slices, const wstring_view& myName, const wstring_view& srcName = L"",
			const string_view& vertexShaderName = "VS_Sky", const string_view& pixelShaderName = "PS_Sky") noexcept;
	virtual ~SkySphere() = default;
};