#include "CEventTimer.h"
#include "ObjectManager.h"



CEventTimer::CEventTimer()
{
	m_myName = L"EventTimer";
	m_comptType = EComponent::Timer;
	Init();
}

CEventTimer::CEventTimer(const float& seconds)
{
	m_myName = L"EventTimer";
	m_comptType = EComponent::Timer;
	Init();
	m_EventDelay = seconds;
}


bool CEventTimer::Init() noexcept
{
	m_curDelay = 0.0f;
	m_isEnable = true;
	static auto pEvent = [](void* pTimer, void*) {
		//((CEventTimer*)pTimer)->isEnable(false);
		ObjectManager::Get().RemoveObject(((CEventTimer*)pTimer)->m_pParent);
	};
	TimerEvent = { pEvent, this, nullptr };
	return true;
}

bool CEventTimer::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;

	m_curDelay += spf;
	if (m_curDelay >= m_EventDelay)
	{
		m_curDelay = 0.0f;
		auto& [timerEvent, param1, param2] = TimerEvent;
		timerEvent(param1, param2);
	}
	return true;
	accTime;
}

bool CEventTimer::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool CEventTimer::Release()	noexcept
{
	return true;
}

Component* CEventTimer::clone() noexcept
{
	return (Component*)new CEventTimer(*this);
}