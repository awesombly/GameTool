#pragma once
#include "GameObject.h"

struct CB_VSMatrix
{
	D3DXMATRIX  m_matWorld;
	D3DXMATRIX  m_matView;
	D3DXMATRIX  m_matProj;
	D3DXMATRIX  matNormal;
	D3DXVECTOR4 m_meshColor;
};

class Camera : public GameObject
{
protected:
	// 부모와 보간된 좌표, 각도
	D3DXVECTOR3	m_curPosition;
	D3DXQUATERNION m_curRotation;
	float m_Aspect;			// 종횡비
	// 최대속, 가속, 현재속
	float m_maxMoveSpeed;
	float m_accMoveSpeed;
	float m_curMoveSpeed;
	// 회전속
	float m_rotateSpeed;

	bool m_isPerspect = true;		// 원근이니
public:
	float m_FOV;			// 시야각
	float m_Near;			// 최소 가시 거리
	float m_Far;			// 최대 가시 거리
	// 보간 속도
	float m_lerpMoveSpeed;
	float m_lerpRotateSpeed;
	// 암 길이
	float m_armLength;
public:
	static Camera* SelectCamera;	// 선택된 카메라

	D3DXMATRIX  m_matWorld;			// 월드
	D3DXMATRIX  m_matView;			// 뷰
	D3DXMATRIX  m_matProj;			// 투영
	CB_VSMatrix	m_cbVS;				// 변환될 행렬
protected:
	// 뷰 행렬, 투영 행렬 세팅
	void SetViewMatrix()										noexcept;
	void SetProjMatrix()										noexcept;
	// 행렬 갱신
	void UpdateMatrix()											noexcept override;	
public:
	virtual void SetParent(GameObject* pParent)					noexcept override;
	void SetOrthoWH(const float& width, const float& height)	noexcept;
	void isPerspect(const bool& isPerspect)						noexcept;

	void SetMainCamera()										noexcept;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
protected:
	Camera() = default;
public:
	Camera(const wstring_view& myName, const bool& isPerspect = true) noexcept;
	virtual ~Camera() = default;
};