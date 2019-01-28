#include "Particle.h"




bool Particle::Init() noexcept
{
	m_isEnable = true;
	m_position = Vector3::Zero;
	m_scale = Vector3::One;
	m_rotation = Quaternion::Base;

	m_direction = Vector3::Zero;
	m_maxMoveSpeed = 0.0f;
	m_accMoveSpeed = 0.0f;
	m_curMoveSpeed = 0.0f;

	m_rotateSpeed	 = 0.0f;
	m_dirAngle = Quaternion::Base;

	m_color = { Color::White };

	m_curGravity = 0.0f;
	m_lifeCycle  = 0.0f;
	m_frameCount = 0.0f;
	return true;
}

bool Particle::Frame(const float& spf, const float& accTime) noexcept
{
	m_frameCount += spf;
	if (m_frameCount >= m_lifeCycle)
	{
		return m_isEnable = false;
	}
	D3DXVec3Lerp(&m_scale, &m_Side, &m_Look, m_frameCount / m_lifeCycle);
	//Scaling(m_Look * spf);
	// 이동, 회전
	m_curMoveSpeed += m_accMoveSpeed * spf;
	m_curMoveSpeed = std::clamp(m_curMoveSpeed, -m_maxMoveSpeed, m_maxMoveSpeed);

	Rotate(m_dirAngle * m_rotateSpeed * spf);
	m_curGravity += m_gravityPower * spf;
	/// 파티클 시스템에서 이동 처리
	return true;
	accTime;
}

Particle* Particle::clone() noexcept
{
	return new Particle(*this);
}

void Particle::SetInitScale(const float& minValue, const float& maxValue) noexcept
{
	float value = RandomNormal() * (maxValue - minValue) + minValue;
	SetScale(value, value, value);
	m_Side = m_scale;
}
void Particle::SetInitScale(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue) noexcept
{
	SetScale(RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z);
	m_Side = m_scale;
}

void Particle::SetScalePerLife(const float& minValue, const float& maxValue)	noexcept
{
	m_Look = m_scale * (RandomNormal() * (maxValue - minValue) + minValue);
}
void Particle::SetScalePerLife(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue) noexcept
{
	m_Look = Product(m_scale, D3DXVECTOR3(RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z));
}


void Particle::SetInitPosition(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue) noexcept
{
	SetPosition(RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z);
}

void Particle::SetInitRotation(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue) noexcept
{
	SetRotation(RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z);
}

void Particle::SetDirection(const D3DXVECTOR3& direction) noexcept
{
	m_direction = direction;
}

void Particle::SetDirection(const D3DXVECTOR3& minValue, const D3DXVECTOR3& maxValue) noexcept
{
	m_direction = { RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z };
	D3DXVec3Normalize(&m_direction, &m_direction);
}

void Particle::SetDirAngle(const D3DXQUATERNION& dirAngle) noexcept
{
	m_dirAngle = dirAngle;
}

void Particle::SetDirAngle(const D3DXQUATERNION& minValue, const D3DXQUATERNION& maxValue) noexcept
{
	m_dirAngle = { RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z, RandomNormal() * (maxValue.w - minValue.w) + minValue.w };
	D3DXQuaternionNormalize(&m_dirAngle, &m_dirAngle);
}

void Particle::SetColor(const D3DXVECTOR4& color) noexcept
{
	m_color = color;
}

void Particle::SetColor(const D3DXVECTOR4& minValue, const D3DXVECTOR4& maxValue) noexcept
{
	m_color = { RandomNormal() * (maxValue.x - minValue.x) + minValue.x, RandomNormal() * (maxValue.y - minValue.y) + minValue.y, RandomNormal() * (maxValue.z - minValue.z) + minValue.z, RandomNormal() * (maxValue.w - minValue.w) + minValue.w };
	//D3DXVec4Normalize(&m_color, &m_color);
}

void Particle::SetMaxMoveSpeed(const float& minValue, const float& maxValue) noexcept
{
	m_maxMoveSpeed = RandomNormal() * (maxValue - minValue) + minValue;
}

void Particle::SetAccMoveSpeed(const float& minValue, const float& maxValue) noexcept
{
	m_accMoveSpeed = RandomNormal() * (maxValue - minValue) + minValue;
}

void Particle::SetCurMoveSpeed(const float& minValue, const float& maxValue) noexcept
{
	m_curMoveSpeed = RandomNormal() * (maxValue - minValue) + minValue;
}

void Particle::SetRotateSpeed(const float& minValue, const float& maxValue) noexcept
{
	m_rotateSpeed = RandomNormal() * (maxValue - minValue) + minValue;
}

void Particle::SetLifeCycle(const float& minValue, const float& maxValue) noexcept
{
	m_lifeCycle = RandomNormal() * (maxValue - minValue) + minValue;
}

void Particle::SetGravityPower(const float& minValue, const float& maxValue) noexcept
{
	m_gravityPower = RandomNormal() * (maxValue - minValue) + minValue;
}

bool Particle::IsEnable()
{
	return m_isEnable;
}
void Particle::IsEnable(const bool& isEnable)
{
	m_isEnable = isEnable;
}