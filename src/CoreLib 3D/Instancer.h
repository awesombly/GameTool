#pragma once
#include "Component.h"
//#include "InstanceRenderer.h"

class InstanceRenderer;

class Instancer : public Component
{
public:
	InstanceRenderer*	m_pInstance = nullptr;	// 연결된 인스턴스 렌더러
	float m_refTexture = 0.1f;
public:
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	Instancer();
	Instancer(InstanceRenderer* pInstance);
	virtual ~Instancer() = default;
};

