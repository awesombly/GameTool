#pragma once
#include "Component.h"
#include "Renderer.h"

class CPlaneShadow : public Component
{
protected:
	Renderer* m_pTargetRender;
	D3DXPLANE m_Plane = D3DXPLANE(0.0f, 1.0f, 0.0f, -0.1f);

	ID3D11PixelShader*	m_pPShader = nullptr;	// «»ºø Ω¶¿Ã¥ı
public:
	D3DXMATRIX CreateMatrixShadow(D3DXVECTOR4* pPlane, D3DXVECTOR4* pLight);

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	CPlaneShadow(Renderer* pRenderer);
	virtual ~CPlaneShadow() = default;
};

