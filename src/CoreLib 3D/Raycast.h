#pragma once
#include "header.h"

class Renderer;
class Collider;

class Raycast
{
private:
	//static GameObject* m_pDrawRay;
	static D3DXVECTOR3 m_start;
	static D3DXVECTOR3 m_direction;
public:
	static D3DXVECTOR3 m_ray;
	static D3DXVECTOR3 HitPoint;
public:
	// 설정후 발사
	static bool Raycasting(const Renderer* pTarget, const D3DXVECTOR3& start, const D3DXVECTOR3& direction, const float& range, D3DXVECTOR3* pHitPoint = nullptr) noexcept;
	// 설정된 방향으로 발사
	static bool Raycasting(const Renderer* pTarget, D3DXVECTOR3* pHitPoint = nullptr, const D3DXMATRIX* pWorldMatrix = nullptr)	noexcept;
	static bool Raycasting(const Collider* pTarget/*, D3DXVECTOR3* pHitPoint = nullptr, const D3DXMATRIX* pWorldMatrix = nullptr*/)	noexcept;
	// start에서 direction * range로 레이 설정
	static void SetRaycast(const D3DXVECTOR3& start, const D3DXVECTOR3& direction, const float& range)	noexcept;
	// 화면 커서 위치로 레이 설정
	static void SetMousePick(const POINT& cursor, const float& range)	noexcept;
	static void DrawRay(ID3D11DeviceContext* pDContext, const D3DXVECTOR4& color = Color::Blue)			noexcept;
public:
	Raycast() = default;
	virtual ~Raycast() = default;
};

