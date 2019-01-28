#include "FrustumBox.h"
#include "ObjectManager.h"




FrustumBox::FrustumBox(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName,
					   const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, eComType, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool FrustumBox::Init() noexcept
{
	Renderer::Init();
	return true;
}

bool FrustumBox::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!Input::isDebug) return false;
	Renderer::Frame(spf, accTime);
	return true;
	spf; accTime;
}

bool FrustumBox::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!Input::isDebug) return false;
	DxManager::GetInstance().SetViewPort(EViewPort::MiniMap);
	ObjectManager::GetInstance().SetCurCamera(ECamera::MiniMap);
	//UpdateMatrix();
	PrevRender(pDContext);
	PostRender(pDContext);
	ObjectManager::GetInstance().SetCurCamera(ECamera::Main);
	DxManager::GetInstance().SetViewPort(EViewPort::Main);
	return true;
}

bool FrustumBox::Release()	noexcept
{
	Renderer::Release();
	return true;
}

HRESULT FrustumBox::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// 정점 리스트
	m_vertexList.resize(24);

	int i = 0;
	// 정면, PNCT
	m_vertexList[i++] = { { -1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = { { 1.0f, 1.0f,  0.0f },{ 0.0f, 0.0f, -1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = { { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 우측
	m_vertexList[i++] = { { 1.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = { { 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = { { 1.0f, -1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = { { 1.0f, -1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 후면
	m_vertexList[i++] = { { 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = { { -1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = { { 1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = { { -1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 좌측
	m_vertexList[i++] = { { -1.0f, 1.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = { { -1.0f, 1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = { { -1.0f, -1.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = { { -1.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 상단
	m_vertexList[i++] = { { -1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = { { 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = { { -1.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = { { 1.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 하단
	m_vertexList[i++] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = { { 1.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = { { -1.0f, -1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = { { 1.0f, -1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f },
						  { 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };

	D3DXMATRIX matViewProj = (*m_ppCamera)->m_matView * (*m_ppCamera)->m_matProj;
	D3DXMatrixInverse(&matViewProj, nullptr, &matViewProj);

	for (i = 0; i < 24; ++i)
	{
		D3DXVec3TransformCoord(&m_vertexList[i].pos, &m_vertexList[i].pos, &matViewProj);
	}

	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer(36)))
		return E_FAIL;
	Init();
	return S_OK;
}

Component* FrustumBox::clone() noexcept
{
	//return (Component*)(new FrustumBox(*this));
	return (new FrustumBox(*this))->cloneAddition();
}