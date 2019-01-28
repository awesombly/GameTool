#include "Instancer.h"
#include "InstanceRenderer.h"


Instancer::Instancer()
{
	m_myName = L"Instancer";
	m_comptType = EComponent::Instancer;
	Init();
}

Instancer::Instancer(InstanceRenderer* pInstance) : Instancer()
{
	m_pInstance = pInstance;
}

bool Instancer::Init() noexcept
{
	return true;
}

bool Instancer::Frame(const float& spf, const float& accTime)	noexcept
{
	return true;
	spf; accTime;
}

bool Instancer::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool Instancer::Release()	noexcept
{
	if (m_pInstance != nullptr)
	{
		m_pInstance->RemoveInstance(this);
	}
	return true;
}


Component* Instancer::clone() noexcept
{
	auto pClone = new Instancer(*this);
	// 인스턴스 등록
	if (m_pInstance != nullptr)
	{
		m_pInstance->AddInstance(pClone);
	}
	return (Component*)pClone;
}