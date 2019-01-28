#include "CPlaneShadow.h"
#include "ObjectManager.h"

CPlaneShadow::CPlaneShadow(Renderer* pRenderer)
{
	m_comptType = EComponent::Etc;
	m_pTargetRender = pRenderer;
	Init();
}

bool CPlaneShadow::Init() noexcept
{
	m_pPShader = DxManager::GetInstance().m_PShaderList["PS_PlaneShadow"];
	return true;
}

bool CPlaneShadow::Frame(const float& spf, const float& accTime)	noexcept
{
	return true;
	spf; accTime;
}

bool CPlaneShadow::Render(ID3D11DeviceContext* pDContext) noexcept
{
	// 평면 쉐도우
	static D3DXMATRIX matShadow;
	D3DXVECTOR4 pLight = D3DXVECTOR4(ObjectManager::Lights.front()->GetPosition(), 1.0f);
	//D3DXVec4Normalize(&pLight, &pLight); pLight.w = 0.0f;
	//matShadow = CreateMatrixShadow(&D3DXVECTOR4(m_Plane), &pLight);
	D3DXMatrixShadow(&matShadow, &pLight, &m_Plane);

	DxManager::GetInstance().SetRasterizerState(ERasterS::CullBack);
	D3DXMATRIX matTrans, matParent = m_pParent->GetWorldMatrix();
	D3DXMatrixTranspose(&matTrans, &matParent);
	matTrans = matTrans * matShadow;
	D3DXMatrixTranspose(&matShadow, &matTrans);
	m_pParent->SetWorldMatrix(matShadow);
	
	m_pTargetRender->Renderer::PrevRender(pDContext);
	pDContext->PSSetShader(m_pPShader, nullptr, 0);
	m_pTargetRender->Renderer::PostRender(pDContext);
	
	m_pParent->SetWorldMatrix(matParent);
	DxManager::GetInstance().SetRasterizerState(ERasterS::Current);
	return true;
	pDContext;
}

bool CPlaneShadow::Release()	noexcept
{
	return true;
}

Component* CPlaneShadow::clone() noexcept
{
	return (Component*)new CPlaneShadow(*this);
}


D3DXMATRIX CPlaneShadow::CreateMatrixShadow(D3DXVECTOR4* pPlane,
									  D3DXVECTOR4* pLight)
{
	//pPlane->Normal();	
	//Vector3 vPlane, vLight;
	//vPlane = Vector3( pPlane->x, pPlane->y, pPlane->z ) * -1.0f;
	//vLight = Vector3( pLight->x, pLight->y, pLight->z );// 라이트 디렉션이 바뀌지 않는다.
	//float b = (vPlane | Vector3(0.0f, -pPlane->w, 0.0f));
	//float a = vPlane | vLight;	
	//float D = b - a;
	//float c = *pPlane | *pLight;
	//_11 = vPlane.x * pLight->x+D;	_12 = vPlane.x * pLight->y;	
	//_13 = vPlane.x * pLight->z;	_14 = vPlane.x * pLight->w;
	//_21 = vPlane.y * pLight->x;	_22 = vPlane.y * pLight->y+D;	
	//_23 = vPlane.y * pLight->z;	_24 = vPlane.y * pLight->w;	
	//_31 = vPlane.z * pLight->x;	_32 = vPlane.z * pLight->y;	
	//_33 = vPlane.z * pLight->z+D;	_34 = vPlane.z * pLight->w;
	////_41 = -(a+D) * pLight->x;	_42 = -(a+D) * pLight->y;	
	////_43 = -(a+D) * pLight->z;	_44 = -a;
	//_41 = -pPlane->w * pLight->x;	_42 = -pPlane->w * pLight->y;	
	//_43 = -pPlane->w * pLight->z;	_44 = -a;
	//return *this;

	D3DXMATRIX mat;
	D3DXVECTOR4 plane, light;
	D3DXVec4Normalize(pPlane, pPlane);
	//pPlane->Normal();
	plane = (*pPlane) * -1.0f;
	light = (*pLight);// * -1.0f;
	float D = -D3DXVec4Dot(&plane, &light);
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}