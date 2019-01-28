#include "RPlane.h"
#include "ObjectManager.h"


RPlane::RPlane(const wstring_view& myName, const wstring_view& srcName,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, EComponent::Renderer, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool RPlane::Init() noexcept
{
	Renderer::Init();
	return true;
}

bool RPlane::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);
	return true;
}

bool RPlane::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool RPlane::Release()	noexcept
{
	Renderer::Release();
	return true;
}




HRESULT RPlane::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// 정점 리스트
	m_vertexList.resize(4);
	
	m_vertexList[0] = { { -1.0f, 1.0f,  0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 0.0f, 0.0f } };
	m_vertexList[1] = { { 1.0f, 1.0f,  0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 1.0f, 0.0f } };
	m_vertexList[2] = { { -1.0f, -1.0f, 0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 0.0f, 1.0f } };
	m_vertexList[3] = { { 1.0f, -1.0f, 0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 1.0f, 1.0f } };
	// 버퍼들 생성
	if(FAILED(CreateVertexBuffer()) ||
	   FAILED(CreateIndexBuffer(6)))
		return E_FAIL;
	Init();
	return S_OK;
}

Component* RPlane::clone() noexcept
{
	return (new RPlane(*this))->cloneAddition();
}