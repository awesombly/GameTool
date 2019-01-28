#include "ColliderAABB.h"
#include "ObjectManager.h"
#include "ColliderOBB.h"



ColliderAABB::ColliderAABB(const float& radius, const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
	: m_minPos(minPos), m_maxPos(maxPos)
{
	ObjectManager::Get().PushCollider(this);
	Init();
	m_radius = radius;
}

ColliderAABB::ColliderAABB(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
	: m_minPos(minPos), m_maxPos(maxPos)
{
	ObjectManager::Get().PushCollider(this);
	Init();
	auto length = (m_maxPos - m_minPos) * 0.5f;
	m_radius = std::max<float>(std::max<float>(length.x, length.y), length.z) * 1.5f;
}


bool ColliderAABB::Init() noexcept
{
	m_myName = L"CAABB";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::AABB;
	return true;
}

Component* ColliderAABB::clone() noexcept
{
	auto pCollider = new ColliderAABB(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}

bool ColliderAABB::CollisionCheck(Collider* pCollider) noexcept
{
	if (!SphereToSphere(this, pCollider))
		return false;

	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		if (AABBToAABB(this, (ColliderAABB*)pCollider))
			return true;
	}	break;
	case ECollider::OBB:
	{
		if (AABBToOBB(this, (ColliderOBB*)pCollider))
			return true;
	}	break;
	case ECollider::Sphere:
	{
		if (SphereToAABB(pCollider, this))
			return true;
	}	break;
	}

	return false;
}


void ColliderAABB::SetMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos) noexcept
{
	m_minPos = minPos;
	m_maxPos = maxPos;
}

D3DXVECTOR3 ColliderAABB::GetMin() noexcept
{
	return Product(m_minPos, m_pParent->GetWorldScale());
}

D3DXVECTOR3 ColliderAABB::GetMax() noexcept
{
	return Product(m_maxPos, m_pParent->GetWorldScale());
}

D3DXVECTOR3 ColliderAABB::GetLength() noexcept
{
	return Product((m_maxPos - m_minPos), m_pParent->GetWorldScale());
}