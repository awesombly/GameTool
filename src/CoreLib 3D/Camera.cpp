#include "Camera.h"
#include "ObjectManager.h"
#include "SoundManager.h"

Camera* Camera::SelectCamera = nullptr;

Camera::Camera(const wstring_view& myName, const bool& isPerspect) noexcept
{
	// 변수 초기화
	Init();
	m_myName = myName;
	m_objType = EObjType::Camera;
	m_isPerspect = isPerspect;
	// 행렬 설정
	SetViewMatrix();
	SetProjMatrix();
	// 
	UpdateMatrix();
}

bool Camera::Init()	noexcept
{
	m_Look = { 0.0f, 0.0f, 1.0f };
	m_Side = { 1.0f, 0.0f, 0.0f };
	m_Up = { 0.0f, 1.0f, 0.0f };

	m_Aspect = (float)Window::getClientRect().right / (float)Window::getClientRect().bottom;
	m_FOV = PI * 0.5f;
	m_Near = 0.1f;
	m_Far = 2000.0f;
	SetProjMatrix();

	m_position = { 0.0f, 0.0f, -6.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_maxMoveSpeed = 140.0f;
	m_accMoveSpeed = 50.0f;
	m_curMoveSpeed = 0.0f;

	m_rotateSpeed = 1.5f;

	m_lerpMoveSpeed = 3.5f;
	m_lerpRotateSpeed = 5.0f;
	m_armLength = 6.0f;
	return true;
}

bool Camera::Frame(const float& spf, const float& accTime)	noexcept
{
	if (m_pParent == nullptr &&
		Input::GetInstance().GetKeyState(VK_SHIFT) == EKeyState::HOLD)
	{
		static bool isMoving, isRotating;
		isMoving = false;
		isRotating = false;
#pragma region MouseCheck
		// 마우스 휠,좌,우 버튼 이동
		Translate(m_Look * Input::GetWheelScroll() * spf * 3);
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::HOLD)
		{
			Translate((-m_Side * Input::GetMouseMovePos().x + m_Up * Input::GetMouseMovePos().y) * 0.18f);
		}
		if (Input::GetKeyState(EMouseButton::Right) == EKeyState::HOLD)
		{
			Rotate(Input::GetMouseMovePos().y * 0.005f, Input::GetMouseMovePos().x * 0.005f);
		}
#pragma endregion
#pragma region KeyCheck
		// FOV 값 조절
		if (Input::GetInstance().GetKeyState('Z') == EKeyState::HOLD)
		{
			m_FOV -= spf;
			SetProjMatrix();
		}
		if (Input::GetInstance().GetKeyState('C') == EKeyState::HOLD)
		{
			m_FOV += spf;
			SetProjMatrix();
		}
		if (Input::GetInstance().GetKeyState('X') == EKeyState::HOLD)
		{
			m_FOV = PI * 0.5f;
			SetProjMatrix();
		}
		// 원근 <-> 직교
		if (Input::GetInstance().GetKeyState(VK_DIVIDE) == EKeyState::DOWN)
		{
			isPerspect(!m_isPerspect);
		}
		// 초기화
		if (Input::GetInstance().GetKeyState(VK_NUMPAD5) == EKeyState::DOWN ||
			Input::GetInstance().GetKeyState('R') == EKeyState::DOWN)
		{
			Init();
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD4) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState('A') == EKeyState::HOLD)
		{
			m_position += GetLeft() * m_curMoveSpeed * spf;
			isMoving = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD6) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState('D') == EKeyState::HOLD)
		{
			m_position += GetRight() * m_curMoveSpeed * spf;
			isMoving = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD8) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState('E') == EKeyState::HOLD)
		{
			m_position += GetUp() * m_curMoveSpeed * spf;
			isMoving = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD2) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState('Q') == EKeyState::HOLD)
		{
			m_position += GetDown() * m_curMoveSpeed * spf;
			isMoving = true;
		}
		if (Input::GetInstance().GetKeyState(VK_ADD) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState('W') == EKeyState::HOLD)
		{
			m_position += GetForward() * m_curMoveSpeed *spf;
			isMoving = true;
		}
		if (Input::GetInstance().GetKeyState(VK_SUBTRACT) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState('S') == EKeyState::HOLD)
		{
			m_position += GetBackward() * m_curMoveSpeed *spf;
			isMoving = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD1) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState(VK_LEFT) == EKeyState::HOLD)
		{
			Rotate(Quaternion::Left * m_rotateSpeed * spf);
			isRotating = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD3) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState(VK_RIGHT) == EKeyState::HOLD)
		{
			Rotate(Quaternion::Right * m_rotateSpeed * spf);
			isRotating = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD7) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState(VK_UP) == EKeyState::HOLD)
		{
			Rotate(Quaternion::Down * m_rotateSpeed * spf);
			isRotating = true;
		}
		if (Input::GetInstance().GetKeyState(VK_NUMPAD9) == EKeyState::HOLD ||
			Input::GetInstance().GetKeyState(VK_DOWN) == EKeyState::HOLD)
		{
			Rotate(Quaternion::Up * m_rotateSpeed * spf);
			isRotating = true;
		}

		// 카메라 이동 조절
		if (isMoving && m_curMoveSpeed < m_maxMoveSpeed)
			m_curMoveSpeed += m_accMoveSpeed * spf;
		else if (m_curMoveSpeed > m_accMoveSpeed)
			m_curMoveSpeed -= m_curMoveSpeed * 0.6f * spf;
		// 카메라 회전
		if (isRotating)
			m_rotateSpeed += 0.2f * spf;
		else if (m_rotateSpeed > 1.0f)
			m_rotateSpeed -= m_rotateSpeed * 0.5f * spf;
#pragma endregion
	}


	if (m_pParent != nullptr)
	{
		m_curPosition = Lerp(m_curPosition + GetBackward() * m_armLength, m_pParent->GetWorldPosition(), spf * m_lerpMoveSpeed);
		m_curRotation = Lerp(m_curRotation, m_pParent->GetWorldRotation(), spf * m_lerpRotateSpeed);
		//D3DXQuaternionSlerp(&m_curRotation, &m_curRotation, &m_pParent->GetRotation(), spf);
	}

	GameObject::Frame(spf, accTime);
	return true;
	accTime;
}

bool Camera::Render(ID3D11DeviceContext* pDContext)			noexcept
{
	GameObject::Render(pDContext);
	//UpdateConstBuffer(pDContext);
	return true;
}

bool Camera::Release() noexcept
{
	GameObject::Release();
	return true;
}


void Camera::SetViewMatrix() noexcept
{
	// 뷰 행렬 반환
	D3DXMatrixLookAtLH(&m_matView, &m_position, &m_Look, &m_Up);

	D3DXMATRIX matInverseView;
	D3DXMatrixInverse(&matInverseView, nullptr, &m_matView);
	D3DXVECTOR3* pZ = (D3DXVECTOR3*)&matInverseView._31;
	// 뷰 행렬 오일러 각 계산
	m_rotation.x = atan2f(pZ->x, pZ->z);
	m_rotation.y = -atan2f(pZ->y,
		sqrtf(pZ->x * pZ->x + pZ->z * pZ->z));

	m_Side.x = m_matView._11;
	m_Side.y = m_matView._21;
	m_Side.z = m_matView._31;
	m_Up.x = m_matView._12;
	m_Up.y = m_matView._22;
	m_Up.z = m_matView._32;
	m_Look.x = m_matView._13;
	m_Look.y = m_matView._23;
	m_Look.z = m_matView._33;
}

void Camera::SetProjMatrix() noexcept
{
	// 투영 행렬
	m_Aspect = (float)Window::getClientRect().right / (float)Window::getClientRect().bottom;
	if (m_isPerspect)
	{
		// 원근 투영				   (리턴, 시야각, 가로세로 비, 가시 거리)
		D3DXMatrixPerspectiveFovLH(&m_matProj, m_FOV, m_Aspect, m_Near, m_Far);
	}
	else
	{
		m_position = { 0.0f, 0.0f, 0.0f };
		// 직교 투영 (리턴, 가로, 세로, 가시거리)
		D3DXMatrixOrthoLH(&m_matProj, 10.0f, 10.0f, 0.0f, 200.0f);
		//D3DXMatrixOrthoOffCenterLH(&m_matProj, 5, 10, 0, 5, m_Near, m_Far);
	}
}

void Camera::UpdateMatrix() noexcept
{
	// 단위행렬 반환
	D3DXMatrixIdentity(&m_matWorld);
	static D3DXQUATERNION quaternion;
	if (m_pParent != nullptr)
	{
		static D3DXMATRIX matTarget;
		// 벡터를 쿼터니언으로 변환
		D3DXQuaternionRotationYawPitchRoll(&quaternion, m_curRotation.y, 0.0f, 0.0f);
		D3DXMatrixAffineTransformation(&matTarget, 1.0f, NULL, &quaternion, &m_curPosition);

		D3DXQuaternionRotationYawPitchRoll(&quaternion, m_rotation.y, m_rotation.x, m_rotation.z);
		D3DXMatrixAffineTransformation(&m_matWorld, 1.0f, NULL, &quaternion, &m_position);
		m_matWorld = m_matWorld * matTarget;
	}
	else
	{
		D3DXQuaternionRotationYawPitchRoll(&quaternion, m_rotation.y, m_rotation.x, m_rotation.z);
		D3DXMatrixAffineTransformation(&m_matWorld, 1.0f, NULL, &quaternion, &m_position);
	}
	D3DXMatrixTranspose(&m_matLocal, &m_matWorld);

	D3DXMatrixInverse(&m_matView, NULL, &m_matWorld);
	m_Side.x = m_matView._11;
	m_Side.y = m_matView._21;
	m_Side.z = m_matView._31;
	m_Up.x = m_matView._12;
	m_Up.y = m_matView._22;
	m_Up.z = m_matView._32;
	m_Look.x = m_matView._13;
	m_Look.y = m_matView._23;
	m_Look.z = m_matView._33;

	// 행렬곱
	//m_matWorld = m_matView * m_matProj;
	//m_matWorld = Matrix::Identity;
	// 전치 행렬화
	m_cbVS.m_matWorld = m_matLocal;
	D3DXMatrixTranspose(&m_cbVS.m_matView, &m_matView);
	D3DXMatrixTranspose(&m_cbVS.m_matProj, &m_matProj);
	m_cbVS.m_meshColor = { Color::White };
	m_cbVS.matNormal = Matrix::Identity;
}

void Camera::SetParent(GameObject* pParent) noexcept
{
	m_pParent = pParent;
	if (pParent == nullptr)
		return;
	m_curPosition = pParent->GetPosition();
	m_curRotation = pParent->GetRotation();
}

void Camera::SetOrthoWH(const float& width, const float& height) noexcept
{
	D3DXMatrixOrthoLH(&m_matProj, width, height, 0.0f, 200.0f);
	UpdateMatrix();
}

void Camera::isPerspect(const bool& isPerspect) noexcept
{
	m_isPerspect = isPerspect;
	SetProjMatrix();
	UpdateMatrix();
}

void Camera::SetMainCamera() noexcept
{
	SoundManager::Get().m_pListenerPos = &m_position;
}