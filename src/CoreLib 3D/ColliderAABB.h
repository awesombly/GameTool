#pragma once
#include "Collider.h"


class ColliderAABB : public Collider
{
private:
	D3DXVECTOR3 m_minPos;
	D3DXVECTOR3 m_maxPos;
public:
	D3DXVECTOR3 GetLength()					 noexcept;
	void SetMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos) noexcept;
	D3DXVECTOR3 GetMin()					 noexcept;
	D3DXVECTOR3 GetMax()					 noexcept;

	bool CollisionCheck(Collider* pCollider) noexcept override;
	bool Init()								 noexcept override;
	Component* clone()						 noexcept override;
public:
	//friend class ObjectManager;
	ColliderAABB(const float& radius, const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos);
	ColliderAABB(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos);
	virtual ~ColliderAABB() = default;
};