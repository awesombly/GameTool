#include "CAnimation.h"
#include "ObjectManager.h"



CAnimation::CAnimation()
{
	m_myName = L"Animation";
	m_comptType = EComponent::Animation;
	Init();
}

bool CAnimation::Init() noexcept
{
	return true;
}

void CAnimation::Update() noexcept
{
	if (!m_Animations.empty())
	{
		static D3DXMATRIX		matScale, matRotation;
		static D3DXVECTOR3		lerpScale, lerpPosition;
		static D3DXQUATERNION	lerpRotation;

		static UINT nextIndex = 0;
		nextIndex = m_curAnimIndex + 1;
		if (nextIndex >= m_Animations.size())
			nextIndex = 0;

		lerpScale = Lerp(m_Animations[m_curAnimIndex].GetScale(), m_Animations[nextIndex].GetScale(), m_frameCount / m_animFrame);
		D3DXQuaternionSlerp(&lerpRotation, &m_Animations[m_curAnimIndex].GetRotation(), &m_Animations[nextIndex].GetRotation(), m_frameCount / m_animFrame);
		lerpPosition = Lerp(m_Animations[m_curAnimIndex].GetPosition(), m_Animations[nextIndex].GetPosition(), m_frameCount / m_animFrame);

		D3DXMatrixScaling(&matScale, lerpScale.x, lerpScale.y, lerpScale.z);
		D3DXMatrixRotationQuaternion(&matRotation, &lerpRotation);
		D3DXMatrixInverse(&m_matAnim, nullptr, &m_Animations[m_curAnimIndex].matScaleAxis);

		m_matAnim = m_matAnim * matScale * m_Animations[m_curAnimIndex].matScaleAxis * matRotation;
		m_matAnim._41 = lerpPosition.x;
		m_matAnim._42 = lerpPosition.y;
		m_matAnim._43 = lerpPosition.z;
		m_pParent->m_matLocal = m_pParent->m_matLocal * m_matAnim;
	}
}

bool CAnimation::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_Animations.empty())
	{
		//if (Input::GetKeyState(VK_TAB) != EKeyState::HOLD)
		//{
			m_frameCount += spf * m_animSpeed;
		//}
		//else
		//{
		//	m_frameCount += spf * m_animSpeed * 0.35f;
		//}

		// 애니메이션 작업
		if (m_frameCount >= m_animFrame)
		{
			m_frameCount = 0.0f;
			if (++m_curAnimIndex >= m_Animations.size())
				m_curAnimIndex = 0;
		}
		else if (m_frameCount <= -m_animFrame)
		{
			m_frameCount = 0.0f;
			if (--m_curAnimIndex < 0)
				m_curAnimIndex = (UINT)m_Animations.size();
		}
	}
	return true;
	accTime;
}

bool CAnimation::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool CAnimation::Release()	noexcept
{
	return true;
}


Component* CAnimation::clone() noexcept
{
	return (Component*)new CAnimation(*this);
}