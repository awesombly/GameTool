#pragma once
#include "Component.h"

class RPlane;
class HeightMap;

class CPhysics : public Component
{
protected:
	HeightMap* m_pMap;
	RPlane* m_pFollower;		// 디버그용 바닥
	
	float m_weight = 1.6f;		// 물체 무게
	float m_curGravity = 0.0f;	// 현재 중력
	
	bool useGravity = true;
public:
	void Update() noexcept override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept;
public:
	CPhysics();
	virtual ~CPhysics() = default;
};

