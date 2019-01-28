#pragma once
#include "Component.h"
#include "Transform.h"



class CTransformer : public Component, public Transform
{
public:
	// Transform* pParent, Transform* pTrans, const float& spf, const float& accTime
	void (*TransEvent)(Transform* pParent, Transform* pTrans, const float& spf, const float& accTime);
	
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	CTransformer();
	CTransformer(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const D3DXVECTOR3& scale = Vector3::Zero);
	virtual ~CTransformer() = default;
};

