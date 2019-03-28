#pragma once
#include "Component.h"

class GameObject;

class CEventTimer : public Component
{
public:
	pair<void(*)(GameObject*, void*), void*> TimerEvent;
	float m_curDelay   = 0.0f;
	float m_EventDelay = 0.0f;

	void Update()										noexcept override;
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	CEventTimer();
	CEventTimer(const float& seconds);
	virtual ~CEventTimer() = default;
};

