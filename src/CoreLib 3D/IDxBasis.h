#pragma once

struct ID3D11DeviceContext;

class IDxBasis
{
public:
	IDxBasis() = default;
	virtual ~IDxBasis() = default;
public:
	virtual bool Init()								noexcept = 0;
	virtual bool Frame(const float&, const float&)	noexcept = 0;
	virtual bool Render(ID3D11DeviceContext*)		noexcept = 0;
	virtual bool Release()							noexcept = 0;
};