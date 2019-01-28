#include "ColliderOBB.h"
#include "ObjectManager.h"
#include "ColliderAABB.h"



ColliderOBB::ColliderOBB(const float& radius, const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
{
	ObjectManager::Get().PushCollider(this);
	Init();
	m_radius = radius;
	SetMinMax(minPos, maxPos);
}


ColliderOBB::ColliderOBB(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
{
	ObjectManager::Get().PushCollider(this);
	Init();
	auto length = (maxPos - minPos) * 0.5f;
	m_radius = std::max<float>(std::max<float>(length.x, length.y), length.z) * 1.5f;
	SetMinMax(minPos, maxPos);
}

bool ColliderOBB::Init() noexcept
{
	m_myName = L"COBB";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::OBB;
	return true;
}

bool ColliderOBB::Frame(const float& spf, const float& accTime) noexcept
{
	Collider::Frame(spf, accTime);

	m_rotate[0] = Divide(m_pParent->GetRight(),		m_pParent->GetWorldScale());
	m_rotate[1] = Divide(m_pParent->GetUp(),		m_pParent->GetWorldScale());
	m_rotate[2] = Divide(m_pParent->GetForward(),	m_pParent->GetWorldScale());
	return true;
}

Component* ColliderOBB::clone() noexcept
{
	auto pCollider = new ColliderOBB(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}

bool ColliderOBB::CollisionCheck(Collider* pCollider) noexcept
{
	if (!SphereToSphere(this, pCollider))
		return false;

	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		if (AABBToOBB((ColliderAABB*)pCollider, this))
			return true;
	}	break;
	case ECollider::OBB:
	{
		if (OBBToOBB(this, (ColliderOBB*)pCollider))
			return true;
	}	break;
	case ECollider::Sphere:
	{
		if (SphereToOBB(pCollider, this))
			return true;
	}	break;
	}

	return false;
}

D3DXVECTOR3 ColliderOBB::GetExtents() noexcept
{
	return Product(m_extents, m_pParent->GetWorldScale());
}

void ColliderOBB::SetMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos) noexcept
{
	m_extents = (maxPos - minPos) * 0.5f;
	m_pivot = minPos + m_extents;
	m_rotate[0] = { 1, 0, 0 };
	m_rotate[1] = { 0, 1, 0 };
	m_rotate[2] = { 0, 0, 1 };
}