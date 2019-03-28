#pragma once
#include "header.h"

//union q3FeaturePair
//{
//	struct
//	{
//		UCHAR inR;
//		UCHAR outR;
//		UCHAR inI;
//		UCHAR outI;
//	};
//	int key;
//};
//
//struct q3Contact
//{
//	D3DXVECTOR3 position;			// 접촉점 월드 좌표
//	float penetration;				// 충돌 침투 깊이
//	float normalImpulse;			// 누적 노말 충격
//	float tangentImpulse[2];		// 누적 마찰 충격
//	float bias;						// Restitution + baumgarte
//	float normalMass;				// 노말 제한 질량
//	float tangentMass[2];			// 접선 제한 질량
//	q3FeaturePair fp;				// 접점에 대한 A와 B의 기능?
//	UCHAR warmStarted;				// 디버그 랜더용?
//};

class Collider;

struct PhysicsInfo
{
	bool m_usePhysics	 = true;
	bool m_isMoving		 = false;
	bool m_isGround		 = false;

	UINT UserSocket		 = DataInfo::MaxUINT;
	float m_maxHP		 = 1.0f;
	float m_curHP		 = 1.0f;
	float m_disHP		 = 1.0f;
	float m_damage		 = 0.0f;
	float m_armor		 = 1.0f;
	void(*DeadEvent)(Collider* pCollider, const UINT& killUser) = nullptr;

	D3DXVECTOR3 m_force		= Vector3::Zero;	// 적용중인 힘
	D3DXVECTOR3 m_direction = Vector3::Zero;	// 이동 힘

	float m_GravityScale = 1.0f;				// 중력크기
	float m_mass		 = 1.0f;				// 질량
	float m_repulsion	 = 0.3f;				// 반발 계수
	float m_drag		 = 0.3f;				// 마찰력
	float m_damping		 = 0.3f;				// 항력
	//float m_angularDamping = 0.2f;			// 회전 항력
	///
	//D3DXVECTOR3 m_prePosition;				// 이전 위치
	//D3DXVECTOR3 m_normal;						// 충돌 노말
	//D3DXVECTOR3 m_tangent[2];					// 접선 벡터
	//q3Contact	m_contacts[8];					// 접점 정보?
	//int			m_contactCount;
};


class Physics
{
public:
	PhysicsInfo* m_pPhysics = nullptr;
public:
	void AddForce(const D3DXVECTOR3& vForce)				noexcept;
	void SetForce(const D3DXVECTOR3& vForce)				noexcept;
	void SetDirectionForce(const D3DXVECTOR3& vForce)		noexcept;
	void SetGravityScale(const float& gravityRate)			noexcept;

	const D3DXVECTOR3 GetForce()					  const noexcept;
	const D3DXVECTOR3 GetTotalForce()				  const noexcept;
	const float GetVelocity()						  const noexcept;
	const float GetVelocitySq()						  const noexcept;
	///
	void SetDamage(const float& value, const UCHAR& stat)	noexcept;
	void SetDamage(const float& value)						noexcept;
	//void SetArmor(const UCHAR& armorStat)					noexcept;
	void OperHP(const float& value)							noexcept;
	void HealHP(const float& value)							noexcept;
	void SetHP(const float& value)							noexcept;
	const float& GetHP()							  const noexcept;
	void SetArmor(const char& stat)							noexcept;
	const float& GetArmor()							  const noexcept;
	///
	void usePhysics(const bool& usePhysics)					noexcept;
	bool usePhysics()										noexcept;
	void isMoving(const bool& isMoving)						noexcept;
	bool isMoving()											noexcept;
	void isGround(const bool& isGround)						noexcept;
	bool isGround()											noexcept;
public:
	Physics() = default;
	virtual ~Physics() = default;
};

