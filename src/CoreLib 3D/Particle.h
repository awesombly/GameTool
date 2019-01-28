#pragma once
#include "Transform.h"

// + 행렬 계산, 파티클 매니저?, 인스턴싱
class Particle : public Transform
{
private:
	float m_frameCount;			// 프레임 계산

	float m_lifeCycle;
	
	// m_Look == scalePerLife
	// m_Side == InitScale

	D3DXVECTOR3 m_direction;
	float m_maxMoveSpeed;
	float m_accMoveSpeed;
	float m_curMoveSpeed;

	D3DXQUATERNION m_dirAngle;
	float m_rotateSpeed;
	
	D3DXVECTOR4 m_color;
	
	float m_gravityPower;
	float m_curGravity;

	bool m_isEnable;
public:
	void SetLifeCycle(const float& minValue, const float& maxValue)					 noexcept;

	void SetInitPosition(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue)	 noexcept;
	void SetInitRotation(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue)	 noexcept;
																					 
	void SetInitScale(const float& minValue, const float& maxValue)					 noexcept;
	void SetInitScale(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue)		 noexcept;
	void SetScalePerLife(const float& minValue, const float& maxValue)				 noexcept;
	void SetScalePerLife(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue)	 noexcept;
																					 
	void SetDirection(const D3DXVECTOR3& direction)		noexcept;					 
	void SetDirection(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue)		 noexcept;
	void SetMaxMoveSpeed(const float& minValue, const float& maxValue) noexcept;	 
	void SetAccMoveSpeed(const float& minValue, const float& maxValue) noexcept;	 
	void SetCurMoveSpeed(const float& minValue, const float& maxValue) noexcept;	 
																					 
	void SetDirAngle(const D3DXQUATERNION& dirAngle)								 noexcept;
	void SetDirAngle(const D3DXQUATERNION& minValue, const D3DXQUATERNION& maxValue) noexcept;
	void SetRotateSpeed(const float& minValue, const float& maxValue)				 noexcept;

	void SetColor(const D3DXVECTOR4& color) noexcept;
	void SetColor(const D3DXVECTOR4& minValue, const D3DXVECTOR4& maxValue)			 noexcept;
																					 
	void SetGravityPower(const float& minValue, const float& maxValue)				 noexcept;

	bool IsEnable();
	void IsEnable(const bool& isEnable);

	bool Init()											noexcept ;
	bool Frame(const float& spf, const float& accTime)	noexcept ;
	//bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//bool Release()										noexcept override;
	Particle* clone() noexcept;
public:
	friend class ParticleSystem;
	Particle() = default;
	virtual ~Particle() = default;
};