#pragma once
#include "Collider.h"


class ColliderOBB : public Collider
{
private:
	D3DXVECTOR3		m_extents;
public:
	D3DXVECTOR3		m_rotate[3];
public:
	D3DXVECTOR3 GetExtents()							noexcept;
	void SetMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos) noexcept;

	bool CollisionCheck(Collider* pCollider)			noexcept override;
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	Component* clone()									noexcept override;
public:
	//friend class ObjectManager;
	ColliderOBB(const float& radius, const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos);
	ColliderOBB(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos);
	virtual ~ColliderOBB() = default;
};