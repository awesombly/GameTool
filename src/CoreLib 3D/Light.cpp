#include "Light.h"
#include "ObjectManager.h"

Light::Light(const wstring_view& myName, const ELight& eLightType, const bool& isPerspect) noexcept
{
	// 변수 초기화
	Init();
	m_ppCamera = &ObjectManager::CurCamera;
	m_myName = myName;
	m_objType = EObjType::Light;
	m_lightType = eLightType;
	m_isPerspect = isPerspect;
	// 행렬 설정
	SetViewMatrix();
	SetProjMatrix();
	// 
	UpdateMatrix();

	m_cbLightData.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_cbLightData.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_cbLightData.Specular = { 1.0f, 1.0f, 1.0f, 10.0f };

	m_cbMaterial.NumKernel = 3;
	m_cbMaterial.ObjectID = 0;

	CreateConstBuffer(&m_cbEnviData, sizeof(m_cbEnviData), &m_pEnviCBuffer);
	CreateConstBuffer(&m_cbLightData, sizeof(m_cbLightData), &m_pLightCBuffer);
	CreateConstBuffer(&m_cbMaterial, sizeof(m_cbMaterial), &m_pMaterialCBuffer);
}


bool Light::Init()	noexcept
{
	Camera::Init();
	//m_FOV = PI * 0.8f;
	return true;
}

bool Light::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable)	return false;
 	GameObject::Frame(spf, accTime);
	return true;
	accTime;
}

bool Light::Render(ID3D11DeviceContext* pDContext)			noexcept
{
	if (!m_isEnable)	return false;
	UpdateConstBuffer(pDContext);
	pDContext->VSSetConstantBuffers(1, 1, &m_pEnviCBuffer);		// 상수(환경)
	pDContext->PSSetConstantBuffers(1, 1, &m_pEnviCBuffer);		// 상수(환경)
	pDContext->VSSetConstantBuffers(2, 1, &m_pLightCBuffer);	// 상수(조명)
	pDContext->PSSetConstantBuffers(2, 1, &m_pLightCBuffer);	// 상수(조명)
	pDContext->VSSetConstantBuffers(3, 1, &m_pMaterialCBuffer);	// 상수(그림자)
	pDContext->PSSetConstantBuffers(3, 1, &m_pMaterialCBuffer);	// 상수(그림자)
	GameObject::Render(pDContext);
	return true;
}

bool Light::Release() noexcept
{
	GameObject::Release();
	return true;
}

void Light::SetParent(GameObject* pParent) noexcept
{
	GameObject::SetParent(pParent);
}



void Light::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	/// 쉐도우 버퍼 갱신
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	{
		MappedResource = { 0, };
		// 접근하고 수정하고 접근해제
		pDContext->Map(m_pMaterialCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		CB_Material* pConstData = (CB_Material*)MappedResource.pData;		// 바꿀 객체

		static D3DXMATRIX matTexture = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
											    , 0.0f, -0.5f, 0.0f, 0.0f
											    , 0.0f, 0.0f, 1.0f, 0.0f
											    , 0.5f, 0.5f, 0.0f, 1.0f);
		//D3DXMatrixTranspose(&matTexture, &matTexture);
		m_cbMaterial.matShadow = m_matView * m_matProj * matTexture;
		//D3DXMatrixTranspose(&m_cbMaterial.matShadow, &m_cbMaterial.matShadow);
		m_cbMaterial.NumKernel = 3;
		m_cbMaterial.ObjectID = 0;
		*pConstData = m_cbMaterial;
		pDContext->Unmap(m_pMaterialCBuffer, 0);
	}

	/// 환경 버퍼 갱신
	{
		pDContext->Map(m_pEnviCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		CB_Environment* pEnviData = (CB_Environment*)MappedResource.pData;

		m_cbEnviData.LightVector = { GetPosition(), 1.33f };
		//m_cbEnviData.matNormal = Matrix::Identity;
		m_cbEnviData.EyePos = { (*m_ppCamera)->GetPosition(), 100.0f };
		m_cbEnviData.EyeDir = { (*m_ppCamera)->GetForward(), 100.0f };

		*pEnviData = m_cbEnviData;
		pDContext->Unmap(m_pEnviCBuffer, 0);
	}

	// 조명 버퍼 갱신
	//if (m_pLightCBuffer != nullptr)
	//{
	//	pDContext->Map(m_pLightCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	//	CB_Light* pMatData = (CB_Light*)MappedResource.pData;
	//
	//	*pMatData = m_cbLightData;
	//	pDContext->Unmap(m_pLightCBuffer, 0);
	//}
}


HRESULT Light::CreateConstBuffer(const void* data, const UINT& dataSize, ID3D11Buffer** pConstBuffer) noexcept
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = dataSize;						// 버퍼 사이즈
	initDesc.Usage = D3D11_USAGE_DYNAMIC;				// 동적 리소스, GPU와 CPU를 통해 접근 가능
	initDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 버퍼 타입
	initDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	initDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0, };
	initData.pSysMem = data;

	if (pConstBuffer == nullptr)
	{
		ErrorMessage(__FUNCTION__ " ->  Null Parameter"s);
		return E_FAIL;
	}
	else
	{
		return DxManager::GetDevice()->CreateBuffer(&initDesc, &initData, pConstBuffer);
	}
}