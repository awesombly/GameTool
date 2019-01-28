#pragma once
#include "header.h"
#include "IDxBasis.h"

class GameObject;

enum class EComponent : char {
	Animation, 
	Renderer, Instancer,
	UI,
	Collider, Transformer, Timer, //Phsics,
	Etc
};

class Component : public IDxBasis
{
public:
	GameObject* m_pParent;
	EComponent	m_comptType;
	
	wstring		m_myName;

	bool m_isEnable = true;
public:
	bool isEnable() const noexcept
	{
		return m_isEnable;
	}
	void isEnable(const bool& isEnable) noexcept
	{
		m_isEnable = isEnable;
	}

	//virtual bool Init()											noexcept override = 0;
	//virtual bool Frame(const float& spf, const float& accTime)	noexcept override = 0;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override = 0;
	//virtual bool Release()										noexcept override = 0;
	virtual void Update()		noexcept {};
	virtual Component* clone()	noexcept = 0;
public:
	Component(Component&)  = default;
	Component(Component&&) = default;
	Component& operator = (const Component&) = default;
	Component& operator = (Component&&)		 = default;
	Component() = default;
	virtual ~Component() = default;
};