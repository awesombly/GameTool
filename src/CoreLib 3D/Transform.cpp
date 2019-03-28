#include "Transform.h"
#include "GameObject.h"


void Transform::LookAtVector(D3DXVECTOR3 target) noexcept
{
	//target -= m_position;
	m_rotation = { sinf(acosf(target.x)), cosf(acosf(target.x)), 0.0f, 1.0f };
}

void Transform::LookAtVectorY(float targetY) noexcept
{
	// targetY -= m_position.y;
	m_rotation.y = acos(targetY);
}

void Transform::SetFocus(const D3DXVECTOR3& target) noexcept
{
	D3DXVECTOR3 forward, dirTarget;
	forward = GetForward();
	forward.y = 0.0f;
	dirTarget = target - m_position;
	dirTarget.y = 0.0f;
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&dirTarget, &dirTarget);
	float fRadian = acosf(D3DXVec3Dot(&dirTarget, &forward));

	if (_isnanf(fRadian))
	{
		//ErrorMessage(L"난 숫자 발생 : " + to_wstring(fRadian));
		m_rotation.y = 0.0f;
		return;
	}

	if (VectorDot(Normalize(GetRight()), dirTarget) > 0.0f)
		m_rotation.y += fRadian;
	else
		m_rotation.y -= fRadian;
}

void Transform::SetFocusDir(const D3DXVECTOR3& direction) noexcept
{
	D3DXVECTOR3 forward, dirTarget;
	forward = GetForward();
	forward.y = 0.0f;
	dirTarget = direction;
	dirTarget.y = 0.0f;
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&dirTarget, &dirTarget);
	float fRadian = acosf(D3DXVec3Dot(&dirTarget, &forward));

	if (_isnanf(fRadian))
	{
		//ErrorMessage(L"난 숫자 발생 : " + to_wstring(fRadian));
		m_rotation.y = 0.0f;
		return;
	}

	if (VectorDot(Normalize(GetRight()), dirTarget) > 0.0f)
		m_rotation.y += fRadian;
	else
		m_rotation.y -= fRadian;
}

void Transform::SetFocusZ(const D3DXVECTOR3& target) noexcept
{
	D3DXVECTOR3 forward, dirTarget;
	forward = GetForward();
	forward.x = 0.0f;
	dirTarget = -target;
	dirTarget.x = 0.0f;
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&dirTarget, &dirTarget);
	float fRadian = acosf(D3DXVec3Dot(&dirTarget, &forward));

	if (VectorDot(Normalize(GetRight()), forward) > 0.0f)
		m_rotation.x += fRadian;
	else
		m_rotation.x -= fRadian;
}

Transform::Transform(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const D3DXVECTOR3& scale) noexcept
{
	m_position = position;
	m_rotation = rotation;
	m_scale	   = scale;
}

void Transform::Transformation(const Transform& transform) noexcept
{
	m_position += transform.m_position;
	m_rotation += transform.m_rotation;
	m_scale    += transform.m_scale;
}                                  
void Transform::Translate(const D3DXVECTOR3& position) noexcept
{
	m_position += position;
}
void Transform::Translate(const float& x, const float& y, const float& z) noexcept
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}
void Transform::Translate(const float& x, const float& y) noexcept
{
	m_position.x += x;
	m_position.y += y;
}
void Transform::Rotate(const D3DXQUATERNION& rotation) noexcept
{
	m_rotation += rotation;
}
void Transform::Rotate(const D3DXVECTOR3& rotation) noexcept
{
	m_rotation.x += rotation.x;
	m_rotation.y += rotation.y;
	m_rotation.z += rotation.z;
}
void Transform::Rotate(const float& x, const float& y, const float& z) noexcept
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;
}
void Transform::Rotate(const float& x, const float& y) noexcept
{
	m_rotation.x += x;
	m_rotation.y += y;
}
void Transform::Scaling(const D3DXVECTOR3& scale) noexcept
{
	m_scale += scale;
}
void Transform::Scaling(const float& x, const float& y, const float& z) noexcept
{
	m_scale.x += x;
	m_scale.y += y;
	m_scale.z += z;
}

void Transform::SetTransform(const Transform& transform) noexcept
{
	*this = transform;
}
void Transform::SetPosition(const D3DXVECTOR3& position) noexcept
{
	m_position = position;
}
void Transform::SetPosition(const float& x, const float& y, const float& z) noexcept
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
void Transform::SetPositionX(const float& x) noexcept
{
	m_position.x = x;
}
void Transform::SetPositionY(const float& y) noexcept
{
	m_position.y = y;
}
void Transform::SetPositionZ(const float& z) noexcept
{
	m_position.z = z;
}
void Transform::SetRotation(const D3DXQUATERNION& rotation) noexcept
{
	m_rotation = rotation;
}
void Transform::SetRotation(const D3DXVECTOR3& rotation) noexcept
{
	m_rotation.x = rotation.x;
	m_rotation.y = rotation.y;
	m_rotation.z = rotation.z;
}
void Transform::SetRotation(const float& x, const float& y, const float& z) noexcept
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}
void Transform::SetRotationX(const float& x) noexcept
{
	m_rotation.x = x;
}
void Transform::SetRotationY(const float& y) noexcept
{
	m_rotation.y = y;
}
void Transform::SetRotationZ(const float& z) noexcept
{
	m_rotation.z = z;
}
void Transform::SetScale(const D3DXVECTOR3& scale) noexcept
{
	m_scale = scale;
}
void Transform::SetScale(const float& x, const float& y, const float& z) noexcept
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}
void Transform::SetScaleX(const float& x) noexcept
{
	m_scale.x = x;
}
void Transform::SetScaleY(const float& y) noexcept
{
	m_scale.y = y;
}
void Transform::SetScaleZ(const float& z) noexcept
{
	m_scale.z = z;
}
const Transform& Transform::GetTransform() const noexcept
{
	return *this;
}
const D3DXVECTOR3& Transform::GetPosition() const noexcept
{
	return m_position;
}
const D3DXQUATERNION& Transform::GetRotation() const noexcept
{
	return m_rotation;
}
const D3DXVECTOR3& Transform::GetScale() const noexcept
{
	return m_scale;
}
const float Transform::GetScaleAverage() const noexcept
{
	return (m_scale.x + m_scale.y + m_scale.z) / 3.0f;
}

const D3DXVECTOR3& Transform::GetForward()	const noexcept
{
	return m_Look;
}
const D3DXVECTOR3  Transform::GetBackward()	const noexcept
{
	return -m_Look;
}
const D3DXVECTOR3& Transform::GetRight()	const noexcept
{
	return m_Side;
}
const D3DXVECTOR3  Transform::GetLeft()		const noexcept
{
	return -m_Side;
}
const D3DXVECTOR3& Transform::GetUp()	const noexcept
{
	return m_Up;
}
const D3DXVECTOR3  Transform::GetDown()		const noexcept
{
	return -m_Up;
}