#pragma once
#include "Component.h"
#include "Transform.h"

// 애니메이션시 노말값 : 노말트랜스폼 - 월드의 역행렬의 전치를 곱 ?

struct Anim : Transform
{
	D3DXMATRIX  matScaleAxis;

	friend class AseParser;
	friend class MaxImporter;
	friend class Dialog_Hierarchy;
};

class CAnimation : public Component
{
private:
	D3DXMATRIX m_matAnim;			// 애님 행렬
	vector<Anim> m_Animations;		// 애니메이션 리스트

	UINT m_curAnimIndex = 0;		// 애님 인덱스
	float m_animSpeed	= 1.0f;		// 애님 속도
	float m_animFrame	= 0.0f;		// 애님 프레임 간격
	float m_frameCount	= 0.0f;		// 애님 계산용
public:
	void Update()										noexcept override;

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	friend class AseParser;
	friend class MaxImporter;
	friend class Dialog_Hierarchy;
	CAnimation();
	virtual ~CAnimation() = default;
};