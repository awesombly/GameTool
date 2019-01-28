#include "Renderer.h"
#include "ObjectManager.h"

UINT Renderer::Stride = sizeof(Vertex_PNCT);
UINT Renderer::Offset = 0;

void Renderer::SetInfo(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_ppCamera = &ObjectManager::CurCamera;
	m_myName = myName;
	m_comptType = eComType;
	SetShaderLayout(vertexShaderName, pixelShaderName);
	if (srcName.empty())
		m_srcName = myName;
	else
		m_srcName = srcName;
	SetSpriteList(m_srcName);

	m_cbMaterial.useLight = 1.0f;
	m_cbMaterial.useShadow = 0.5f;		// 쉐도우 비율
	m_cbMaterial.useEnviMap = 0.0f;
	m_cbMaterial.useNormalMap= 0.0f;
	CreateConstBuffer(&m_cbMaterial, sizeof(m_cbMaterial), &m_pMaterialCBuffer);
}

bool Renderer::Init() noexcept
{
	m_comptType = EComponent::Renderer;
	return true;
}

bool Renderer::Frame(const float& spf, const float& accTime) noexcept
{
	if (!m_isEnable) return false;

	// 스프라이트 프레임 계산
	if (m_curSprite->m_frame != 0.0f)
	{
		m_frameCount += spf;
		if (m_frameCount >= m_curSprite->m_frame)
		{
			m_frameCount = 0.0f;
			if (++m_curSprite == m_pSpriteList->end())
				EndSpriteEvent();
			UpdateTextureUV();
		}
	}
	return true;
	accTime;
}

bool Renderer::PrevRender(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	UpdateConstBuffer(pDContext);
	// 리소스를 픽셀 쉐이더에 바인딩(시작 인덱스, 쉐이더 자원 갯수, 리소스뷰)
	pDContext->IASetInputLayout(m_pInputLayout);
	// 오브젝트 쉐이더, 버퍼, 리소스 설정 -> 드로우
	pDContext->VSSetShader(m_pVShader, nullptr, 0);			// 정점쉐이더
	pDContext->PSSetShader(m_pPShader, nullptr, 0);			// 픽셀 쉐이더
	pDContext->GSSetShader(nullptr, nullptr, 0);			// 기하 쉐이더	
	pDContext->VSSetConstantBuffers(0, 1, &m_pMatrixCBuffer);		// 상수(행렬)
	if (m_pMaterialCBuffer != nullptr)
	{
		pDContext->VSSetConstantBuffers(3, 1, &m_pMaterialCBuffer);	// 상수(그림자)
		pDContext->PSSetConstantBuffers(3, 1, &m_pMaterialCBuffer);	// 상수(그림자)
	}

	// 텍스쳐 리소스 적용 
	if (m_isMultiTexture)
	{
		for (char index = 0; index < m_pSpriteList->size(); ++index)
		{
			(*m_pSpriteList)[index].m_pTexture->SetShaderResource(pDContext, index);
		}
	}
	else
	{
		m_curSprite->m_pTexture->SetShaderResource(pDContext);
		pDContext->PSSetShaderResources(1, 1, &m_pNormalMap);
		pDContext->PSSetShaderResources(2, 1, &m_pEnviMap);
	}
	return true;
}

bool Renderer::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	//pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pDContext->DrawIndexed(m_indexCount, 0, 0);
	return true;
}

bool Renderer::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}

bool Renderer::Release() noexcept
{
	return true;
}

void Renderer::SetLightRate(const float& value)	noexcept
{
	m_cbMaterial.useLight = value;
}
void Renderer::SetShadowRate(const float& value) noexcept
{
	m_cbMaterial.useShadow = value;
}
float Renderer::GetLightRate() const noexcept
{
	return m_cbMaterial.useLight;
}
float Renderer::GetShadowRate()	const noexcept
{
	return m_cbMaterial.useShadow;
}

void Renderer::SetEnviromentMap(const wstring_view& enviTexName, const EEnviType& eEnviType) noexcept
{
	// 비어있으면 해제
	if (enviTexName.empty())
	{
		m_pEnviMap = nullptr;
		m_cbMaterial.useEnviMap = 0.0f;
		return;
	}

	m_pEnviMap = DxManager::GetInstance().GetTexture(enviTexName)->GetSRView();
	
	// 환경 쉐이더 설정
	//m_pVShader = DxManager::GetInstance().m_VShaderList["VS_Envi"];
	switch (eEnviType)
	{
	case EEnviType::Fresnel:
	{
		m_cbMaterial.useEnviMap = 2.1f;
		//m_pPShader = DxManager::GetInstance().m_PShaderList["PS_Fresnel"];
	}break;
	case EEnviType::Refraction:
	{
		m_cbMaterial.useEnviMap = 3.1f;
		//m_pPShader = DxManager::GetInstance().m_PShaderList["PS_Refraction"];
	}break;
	case EEnviType::Basic:
	default:
	{
		m_cbMaterial.useEnviMap = 1.1f;
	//	m_pPShader = DxManager::GetInstance().m_PShaderList["PS_Envi"];
	}break;
	}
}

void Renderer::SetNormalMap(const wstring_view& normalTexName) noexcept
{
	// 비어있으면 해제
	if (normalTexName.empty())
	{
		m_pNormalMap = nullptr;
		m_cbMaterial.useNormalMap = 0.0f;
		return;
	}

	if (m_pNormalMap == nullptr)
	{
		// 접선벡터 계산
		D3DXVECTOR3 tan, pBiNormal, pNormal;
		int iIndex = 0;
		int i0, i1, i2, i3, i4, i5;

		int vertexSize = (int)m_vertexList.size();
		for (int i = 0; i < vertexSize; i += 4)
		{
			if (i + 6 >= vertexSize)
			{
				while (i < vertexSize)
				{
					m_vertexList[i++].tan = Vector3::Zero;
				}
				break;
			}
			i0 = m_indexList[iIndex + 0];
			i1 = m_indexList[iIndex + 1];
			i2 = m_indexList[iIndex + 2];
			i3 = m_indexList[iIndex + 3];
			i4 = m_indexList[iIndex + 4];
			i5 = m_indexList[iIndex + 5];
			CreateTangentSpaceVectors(&m_vertexList[i0].pos, &m_vertexList[i1].pos, &m_vertexList[i2].pos,
				m_vertexList[i0].tex.x, m_vertexList[i0].tex.y,
				m_vertexList[i1].tex.x, m_vertexList[i1].tex.y,
				m_vertexList[i2].tex.x, m_vertexList[i2].tex.y,
				&tan, &pBiNormal, &pNormal);
			//m_vertexList[i+0].n  = pNormal;
			m_vertexList[i + 0].tan = tan;

			CreateTangentSpaceVectors(&m_vertexList[i1].pos, &m_vertexList[i2].pos, &m_vertexList[i0].pos,
				m_vertexList[i1].tex.x, m_vertexList[i1].tex.y,
				m_vertexList[i2].tex.x, m_vertexList[i2].tex.y,
				m_vertexList[i0].tex.x, m_vertexList[i0].tex.y,
				&tan, &pBiNormal, &pNormal);
			//vertices[i+1].n  = pNormal;
			m_vertexList[i + 1].tan = tan;
			CreateTangentSpaceVectors(&m_vertexList[i2].pos, &m_vertexList[i0].pos, &m_vertexList[i1].pos,
				m_vertexList[i2].tex.x, m_vertexList[m_indexList[2]].tex.y,
				m_vertexList[i0].tex.x, m_vertexList[m_indexList[0]].tex.y,
				m_vertexList[i1].tex.x, m_vertexList[m_indexList[1]].tex.y,
				&tan, &pBiNormal, &pNormal);
			//m_vertexList[i+2].n  = pNormal;
			m_vertexList[i + 2].tan = tan;

			CreateTangentSpaceVectors(&m_vertexList[i5].pos, &m_vertexList[i3].pos, &m_vertexList[i4].pos,
				m_vertexList[i5].tex.x, m_vertexList[i5].tex.y,
				m_vertexList[i3].tex.x, m_vertexList[i3].tex.y,
				m_vertexList[i4].tex.x, m_vertexList[i4].tex.y,
				&tan, &pBiNormal, &pNormal);
			//m_vertexList[i+3].n  = pNormal;
			m_vertexList[i + 3].tan = tan;

			iIndex += 6;
		}
	}

	// 노말맵 설정
	m_pNormalMap = DxManager::GetInstance().GetTexture(normalTexName)->GetSRView();
	m_cbMaterial.useNormalMap = 1.0f;

	m_pVShader = DxManager::GetInstance().m_VShaderList["VS_Final"];
	m_pPShader = DxManager::GetInstance().m_PShaderList["PS_Final"];

	///	// 노말맵 쉐이더
	///	m_pVShader = DxManager::GetInstance().m_VShaderList["VS_Normal"];
	///	m_pPShader = DxManager::GetInstance().m_PShaderList["PS_Normal"];
	///}
	///else 
}

//DWORD Renderer::EncodeVectorAsDWORDColor(D3DXVECTOR3* vVector) noexcept
//{
//	return 0;
//}

void Renderer::CreateTangentSpaceVectors(D3DXVECTOR3 *v0, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2,
										  const D3DXVECTOR2& uv0, const D3DXVECTOR2& uv1, const D3DXVECTOR2& uv2,
										  D3DXVECTOR3 *pTangent, D3DXVECTOR3 *pBiNormal, D3DXVECTOR3 *pNormal) noexcept
{
	D3DXVECTOR3 vEdge1 = *v1 - *v0;
	D3DXVECTOR3 vEdge2 = *v2 - *v0;
	D3DXVec3Normalize(&vEdge1, &vEdge1);
	D3DXVec3Normalize(&vEdge2, &vEdge2);
	// UV delta
	D3DXVECTOR2 deltaUV1 = uv1 - uv0;
	D3DXVECTOR2 deltaUV2 = uv2 - uv0;
	D3DXVec2Normalize(&deltaUV1, &deltaUV1);
	D3DXVec2Normalize(&deltaUV2, &deltaUV2);


	D3DXVECTOR3 biNormal;
	float fDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	if (fabsf(fDet) < 1e-6f)
	{
		*pTangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		biNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		*pTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y)*fDet;
		biNormal = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x)*fDet;
	}
	D3DXVec3Normalize(pTangent, pTangent);
	D3DXVec3Normalize(&biNormal, &biNormal);

	D3DXVec3Cross(pBiNormal, pNormal, pTangent);
	float crossinv = (D3DXVec3Dot(pBiNormal, &biNormal) < 0.0f) ? -1.0f : 1.0f;
	*pBiNormal *= crossinv;
}

void Renderer::CreateTangentSpaceVectors(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3,
										  const float& _u1, const float& _v1, const float& _u2, const float& _v2, const float& _u3, const float& _v3,
										  D3DXVECTOR3* pTangent, D3DXVECTOR3* pBiNormal, D3DXVECTOR3* pNormal) noexcept
{
	D3DXVECTOR3 vEdge1 = *v2 - *v1;
	D3DXVECTOR3 vEdge2 = *v3 - *v1;
	float fEdge1_U = _u2 - _u1;
	float fEdge1_V = _v2 - _v1;
	float fEdge2_U = _u3 - _u1;
	float fEdge2_V = _v3 - _v1;

	float fDenominator = fEdge1_U * fEdge2_V - fEdge2_U * fEdge1_V;

	if (fDenominator < 0.0001f && fDenominator > -0.0001f)
	{
		*pTangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		*pBiNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		*pNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		float fScale1 = 1.0f / fDenominator;

		D3DXVECTOR3 T;
		D3DXVECTOR3 B;
		D3DXVECTOR3 N;

		T = D3DXVECTOR3((fEdge2_V * vEdge1.x - fEdge1_V * vEdge2.x) * fScale1,
			(fEdge2_V * vEdge1.y - fEdge1_V * vEdge2.y) * fScale1,
						(fEdge2_V * vEdge1.z - fEdge1_V * vEdge2.z) * fScale1);

		B = D3DXVECTOR3((-fEdge2_U * vEdge1.x + fEdge1_U * vEdge2.x) * fScale1,
			(-fEdge2_U * vEdge1.y + fEdge1_U * vEdge2.y) * fScale1,
						(-fEdge2_U * vEdge1.z + fEdge1_U * vEdge2.z) * fScale1);

		D3DXVec3Cross(&N, &T, &B);

		float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) +
			(B.x * N.y * T.z - B.z * N.y * T.x) +
								(N.x * T.y * B.z - N.z * T.y * B.x));

		D3DXVECTOR3 vTemp;

		(*pTangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2;
		(*pTangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
		(*pTangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
		D3DXVec3Normalize(&(*pTangent), &(*pTangent));

		(*pBiNormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2);
		(*pBiNormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
		(*pBiNormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
		D3DXVec3Normalize(&(*pBiNormal), &(*pBiNormal));

		(*pNormal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2;
		(*pNormal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
		(*pNormal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
		D3DXVec3Normalize(&(*pNormal), &(*pNormal));
		//*pBiNormal = *pBiNormal * -1.0f;
	}
}


void Renderer::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	// 행렬 버퍼 갱신
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	{
		MappedResource = { 0, };
		// 접근하고 수정하고 접근해제
		pDContext->Map(m_pMatrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		CB_VSMatrix* pConstData = (CB_VSMatrix*)MappedResource.pData;		// 바꿀 객체

		static D3DXMATRIX matNormalTransform;
		D3DXMatrixInverse(&matNormalTransform, nullptr, &m_pParent->GetWorldMatrix());
		D3DXMatrixTranspose(&matNormalTransform, &matNormalTransform);

		*pConstData = (*m_ppCamera)->m_cbVS;
		pConstData->m_matWorld = m_pParent->GetWorldMatrix();
		pConstData->matNormal = matNormalTransform;

		pDContext->Unmap(m_pMatrixCBuffer, 0);
	}

	// 메테리얼
	if (m_pMaterialCBuffer != nullptr)
	{
		MappedResource = { 0, };
		// 접근하고 수정하고 접근해제
		pDContext->Map(m_pMaterialCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		CB_Material* pMaterialData = (CB_Material*)MappedResource.pData;		// 바꿀 객체

		auto pLight = ObjectManager::Lights.front();
		D3DXMatrixTranspose(&m_cbMaterial.matShadow, &m_pParent->GetWorldMatrix());
		m_cbMaterial.matShadow = m_cbMaterial.matShadow * pLight->m_cbMaterial.matShadow;
		D3DXMatrixTranspose(&m_cbMaterial.matShadow, &m_cbMaterial.matShadow);
		m_cbMaterial.NumKernel = 3;
		m_cbMaterial.ObjectID = 0;
		*pMaterialData = m_cbMaterial;
		pDContext->Unmap(m_pMaterialCBuffer, 0);
	}
}


// 정점에 대한 정보를 GPU에 전달
HRESULT Renderer::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = (UINT)(m_vertexList.size() * sizeof(m_vertexList.front()));	// 정점크기 * 갯수
	initDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 모드(CPU->GPU 접근제어)
	initDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버퍼 타입
	initDesc.CPUAccessFlags = 0;						// CPU 접근 제어
	initDesc.MiscFlags = 0;
	//initDesc.StructureByteStride = 0;
	//initData.SysMemPitch = 0;
	//initData.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA InitData = { 0, };
	InitData.pSysMem = &m_vertexList.at(0);		// GPU에 전달 할 메모리

	if (m_pVertexBuffer != nullptr)
		m_pVertexBuffer->Release();
	//					버퍼 생성(버퍼정보,	   소스 정보, 대상 버퍼)
	return DxManager::GetDevice()->CreateBuffer(&initDesc, &InitData, &m_pVertexBuffer);
}

HRESULT Renderer::CreateIndexBuffer(const UINT& indexCount, const bool& needInit)
{
	// 인덱스 리스트 생성
	m_indexCount = indexCount;
	if (needInit)
	{
		m_indexList.resize(m_indexCount);
		if (indexCount % 6 == 0)
		{
			for (WORD index = 0, val = 0; index < indexCount - 1; val += 4)
			{
				m_indexList[index++] = val;		m_indexList[index++] = val + 1;	m_indexList[index++] = val + 2;
				m_indexList[index++] = val + 2;	m_indexList[index++] = val + 1;	m_indexList[index++] = val + 3;
			}
		}
		else
		{
			for (WORD index = 0; index < indexCount; ++index)
			{
				m_indexList[index] = index;
			}
		}
	}

	D3D11_BUFFER_DESC initDesc{ 0, };
	initDesc.ByteWidth = m_indexCount * sizeof(WORD);			// 버퍼 사이즈
	initDesc.Usage = D3D11_USAGE_DEFAULT;
	initDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// 버퍼 타입
	initDesc.CPUAccessFlags = 0;
	initDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{ 0, };
	initData.pSysMem = &m_indexList.at(0);
	return DxManager::GetDevice()->CreateBuffer(&initDesc, &initData, &m_pIndexBuffer);
}

HRESULT Renderer::CreateConstBuffer(const void* data, const UINT& dataSize, ID3D11Buffer** pConstBuffer)
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
		ErrorMessage(__FUNCTION__ + " ->  Null Parameter"s);
		return E_FAIL;
	}
	else if (*pConstBuffer != nullptr)
	{
		(*pConstBuffer)->Release();
	}
	return DxManager::GetDevice()->CreateBuffer(&initDesc, &initData, pConstBuffer);
}


// 쉐이더, 레이아웃 맵에서 반환
void Renderer::SetShaderLayout(const string_view& vertexName, const string_view& pixelName)
{
	m_pPShader = DxManager::Get().m_PShaderList[pixelName.data()];
	m_pVShader = DxManager::Get().m_VShaderList[vertexName.data()];
	m_pInputLayout = DxManager::Get().m_LayoutList[vertexName.data()];
}

void Renderer::SetPixelShader(const string_view& pixelName)
{
	m_pPShader = DxManager::Get().m_PShaderList[pixelName.data()];
}


void Renderer::SetSpriteList(const wstring_view& srcName) noexcept
{
	if (!srcName.empty())
		m_srcName = srcName.data();
	m_pSpriteList = ObjectManager::Get().GetSpriteList(m_srcName);
	m_curSprite = m_pSpriteList->begin();
	UpdateTextureUV();
}

void Renderer::SetSpriteList(vector<Sprite>* pSpriteList) noexcept
{
	m_pSpriteList = pSpriteList;
	m_curSprite = m_pSpriteList->begin();
	UpdateTextureUV();
}

vector<Sprite>::iterator& Renderer::GetCurSprite() noexcept
{
	return m_curSprite;
}



void Renderer::UpdateTextureUV()
{
	static D3DXVECTOR4 posVector;
	//auto width = m_curSprite->m_pTexture->GetTexWidth();
	//auto height = m_curSprite->m_pTexture->GetTexHeight();
	//posVector = {	 m_curSprite->m_textureUV.x / width,
	//				 m_curSprite->m_textureUV.y / height,
	//				(m_curSprite->m_textureUV.x + m_curSprite->m_textureUV.z) / width,
	//				(m_curSprite->m_textureUV.y + m_curSprite->m_textureUV.w) / height };

	posVector = { m_curSprite->m_textureUV.x,
				  m_curSprite->m_textureUV.y,
				 (m_curSprite->m_textureUV.x + m_curSprite->m_textureUV.z),
				 (m_curSprite->m_textureUV.y + m_curSprite->m_textureUV.w) };

	auto size = m_vertexList.size();
	//if (size % 4 != 0)
	//{
	//	ErrorMessage__FUNCTION__ + " -> vertexList is small!"s);
	//	return;
	//}
	for (int i = 0; i < size; i += 4)
	{
		if (i + 3 >= size)
			break;
		m_vertexList[i].tex = { posVector.x, posVector.y };
		m_vertexList[i + 1].tex = { posVector.z, posVector.y };
		m_vertexList[i + 2].tex = { posVector.x, posVector.w };
		m_vertexList[i + 3].tex = { posVector.z, posVector.w };
	}
	if(m_pVertexBuffer != nullptr)
		DxManager::GetDContext()->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}
void Renderer::EndSpriteEvent()
{
	m_curSprite = m_pSpriteList->begin();
}

void Renderer::SetCamera(Camera** ppCamera)
{
	m_ppCamera = ppCamera;
}

void Renderer::SetColor(ID3D11DeviceContext* pDContext, const float& red, const float& green, const float& blue)
{
	for (auto& iter : m_vertexList)
	{
		iter.col.x = red;
		iter.col.y = green;
		iter.col.z = blue;
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}
void Renderer::SetColor(ID3D11DeviceContext* pDContext, const D3DXVECTOR4& color)
{
	for (auto& iter : m_vertexList)
	{
		iter.col = color;
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

void Renderer::SetAlpha(ID3D11DeviceContext* pDContext, const float& alpha)
{
	for (auto& iter : m_vertexList)
	{
		iter.col.w = alpha;
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

bool Renderer::isMultiTexture() noexcept
{
	return m_isMultiTexture;
}
void Renderer::isMultiTexture(const bool& isMulti) noexcept
{
	m_isMultiTexture = isMulti;
}

Component* Renderer::clone() noexcept
{
	//return (Component*)(new Renderer(*this));
	return (new Renderer(*this))->cloneAddition();
}

Component* Renderer::cloneAddition() noexcept
{
	m_pVertexBuffer = nullptr;
	CreateVertexBuffer();
	return this;
}