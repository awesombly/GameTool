#include "Raycast.h"
#include "Renderer.h"
#include "ObjectManager.h"
#include "RLine.h"
#include "Collider.h"

//GameObject* Raycast::m_pDrawRay = nullptr;
D3DXVECTOR3 Raycast::m_start;
D3DXVECTOR3 Raycast::m_direction;
D3DXVECTOR3 Raycast::m_ray;
D3DXVECTOR3 Raycast::HitPoint;



bool Raycast::Raycasting(const Collider* pTarget/*, D3DXVECTOR3* pHitPoint = nullptr, const D3DXMATRIX* pWorldMatrix*/) noexcept
{
	auto targetPos = pTarget->GetCenter();

	auto point = m_start + m_direction * VectorDot(m_direction, targetPos);
	float length = VectorLength(point - targetPos);
	if (length <= pTarget->GetWorldRadius())
	{
		//ErrorMessage(pTarget->m_pParent->m_myName + L", Succese!,  length : " + to_wstring(length) + L", radius : " + to_wstring(pTarget->GetWorldRadius()));
		return true;
	}
	//ErrorMessage(pTarget->m_pParent->m_myName + L", length : " + to_wstring(length) + L", radius : " + to_wstring(pTarget->GetWorldRadius()));
	return false;
}

bool Raycast::Raycasting(const Renderer* pTarget, D3DXVECTOR3* pHitPoint, const D3DXMATRIX* pWorldMatrix) noexcept
{
	D3DXVECTOR3 faceVertex[3], faceNormal, tempVertex;
	WORD i = 0;
	while (i < pTarget->m_indexList.size())
	{
		faceNormal = (pTarget->m_vertexList[pTarget->m_indexList[i]].nor +
					  pTarget->m_vertexList[pTarget->m_indexList[i + 1]].nor +
					  pTarget->m_vertexList[pTarget->m_indexList[i + 2]].nor) / 3;

		faceVertex[0] = pTarget->m_vertexList[pTarget->m_indexList[i++]].pos;
		faceVertex[1] = pTarget->m_vertexList[pTarget->m_indexList[i++]].pos;
		faceVertex[2] = pTarget->m_vertexList[pTarget->m_indexList[i++]].pos;

		static D3DXMATRIX matTransLocal;
		if(pWorldMatrix != nullptr)
			D3DXMatrixTranspose(&matTransLocal, pWorldMatrix);
		else
			D3DXMatrixTranspose(&matTransLocal, &pTarget->m_pParent->GetWorldMatrix());
		for (auto& iter : faceVertex)
		{
			D3DXVec3TransformCoord(&iter, &iter, &matTransLocal);
		}
		D3DXVec3TransformNormal(&faceNormal, &faceNormal, &matTransLocal);
		D3DXVec3Normalize(&faceNormal, &faceNormal);

		float distance = D3DXVec3Dot(&faceNormal, &m_ray);
		tempVertex = faceVertex[0] - m_start;
		float tDistance = D3DXVec3Dot(&faceNormal, &tempVertex);
		float t = tDistance / distance;

		// 평면 방정식 통과 -> 교착점 in 폴리곤
		if (t >= 0.0f && t <= 1.0f)
		{
			D3DXVECTOR3 e0, e1, inter, vNormal;
			HitPoint = m_start + m_ray * t;
			// 교차점과 각 선간 외적 부호가 같아야함
			e0 = faceVertex[2] - faceVertex[1];
			e1 = faceVertex[0] - faceVertex[1];
			inter = HitPoint - faceVertex[1];

			D3DXVec3Cross(&vNormal, &e0, &inter);
			D3DXVec3Normalize(&vNormal, &vNormal);
			t = D3DXVec3Dot(&faceNormal, &vNormal);
			if (t < 0.0f) continue;
			D3DXVec3Cross(&vNormal, &inter, &e1);
			D3DXVec3Normalize(&vNormal, &vNormal);
			t = D3DXVec3Dot(&faceNormal, &vNormal);
			if (t < 0.0f) continue;

			e0 = faceVertex[0] - faceVertex[2];
			e1 = faceVertex[1] - faceVertex[2];
			inter = HitPoint - faceVertex[2];

			D3DXVec3Cross(&vNormal, &e0, &inter);
			D3DXVec3Normalize(&vNormal, &vNormal);
			t = D3DXVec3Dot(&faceNormal, &vNormal);
			if (t < 0.0f) continue;
			D3DXVec3Cross(&vNormal, &inter, &e1);
			D3DXVec3Normalize(&vNormal, &vNormal);
			t = D3DXVec3Dot(&faceNormal, &vNormal);
			if (t < 0.0f) continue;
			// 테스트 통과시 true
			if (pHitPoint != nullptr)
				*pHitPoint = HitPoint;
			return true;
		}
	}
	// 모든 페이스 테스트 미통과시 false
	return false;
}

bool Raycast::Raycasting(const Renderer* pTarget, const D3DXVECTOR3& start, const D3DXVECTOR3& direction, const float& range, D3DXVECTOR3* pHitPoint) noexcept
{
	SetRaycast(start, direction, range);
	return Raycasting(pTarget, pHitPoint);
}

void Raycast::SetRaycast(const D3DXVECTOR3& start, const D3DXVECTOR3& direction, const float& range) noexcept
{
	m_start = start;
	m_direction = direction;
	m_ray = direction * range;
}


void Raycast::SetMousePick(const POINT& cursor, const float& range) noexcept
{
	// ray와 객체간의 좌표계를 맞춰야함, ray와 객체 모두 World로 하도록 함
	// 화면 좌표 -> 투영 좌표 -> 뷰 좌표 -> 월드 좌표
	static D3DXVECTOR3 start, direction;
	start = Vector3::Zero;
	direction.x =  (2.0f * cursor.x / Window::getClientRect().right - 1)  / ObjectManager::CurCamera->m_matProj._11;
	direction.y = -(2.0f * cursor.y / Window::getClientRect().bottom - 1) / ObjectManager::CurCamera->m_matProj._22;
	direction.z = 1.0f;
	// 현재 start, end = 뷰 좌표계, 뷰->월드로 변환,, direction 정규화
	static D3DXMATRIX matInView;
	//if(ObjectManager::CurCamera->GetParent() == nullptr)
	//	matInView = ObjectManager::CurCamera->m_matView;
	//else
	//	matInView = ObjectManager::CurCamera->m_matView * ObjectManager::CurCamera->GetParent()->GetWorldMatrix();
	D3DXMatrixInverse(&matInView, nullptr, &ObjectManager::CurCamera->m_matView);
	// direction은 방향성만을 가지는 벡터기 때문에, 카메라 뷰 행렬의 이동값은 무시함
	D3DXVec3TransformCoord(&start, &start, &matInView);
	D3DXVec3TransformNormal(&direction, &direction, &matInView);
	D3DXVec3Normalize(&direction, &direction);
	// 레이 설정
	SetRaycast(start, direction, range);
}


void Raycast::DrawRay(ID3D11DeviceContext* pDContext, const D3DXVECTOR4& color) noexcept
{
	static auto pLine	 = new RLine(L"Ray");
	static auto pLineObj = new GameObject(L"Ray", pLine);
	//pLine->SetLineInfo(pDContext, m_start, m_start + m_ray, color);
	pLine->SetLineInfo(pDContext, m_start, m_start + m_ray, color);
	pLineObj->Frame(0.0f, 0.0f);
	pLineObj->Render(pDContext);
}