#include "RCube.h"
#include "ObjectManager.h"




RCube::RCube(const wstring_view& myName, const wstring_view& srcName,
				const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, EComponent::Renderer, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool RCube::Init() noexcept
{
	Renderer::Init();
	return true;
}

bool RCube::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);
	return true;
}

bool RCube::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool RCube::Release()	noexcept
{
	Renderer::Release();
	return true;
}


HRESULT RCube::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// 정점 리스트
	m_vertexList.resize(24);

	int i = 0;
	// 정면, PNCT
	m_vertexList[i++] = {	{ -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f } };
	m_vertexList[i++] = {	{ 1.0f, 1.0f,  -1.0f }, { 0.0f, 0.0f, -1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f } };
	m_vertexList[i++] = {	{ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f } };
	m_vertexList[i++] = {	{  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f }, { 1.0f, 1.0f } };
	// 우측
	m_vertexList[i++] = {	{ 1.0f, 1.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = {	{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = {	{ 1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = {	{ 1.0f, -1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 후면
	m_vertexList[i++] = {	{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = {	{ -1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = {	{ 1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = {	{ -1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 좌측
	m_vertexList[i++] = {	{ -1.0f, 1.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = {	{ -1.0f, 1.0f, -1.0f },{ -1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = {	{ -1.0f, -1.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = {	{ -1.0f, -1.0f, -1.0f },{ -1.0f, 0.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 상단
	m_vertexList[i++] = {	{ -1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = {	{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = {	{ -1.0f, 1.0f, -1.0f },{ 0.0f, 1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = {	{ 1.0f, 1.0f, -1.0f },{ 0.0f, 1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };
	// 하단
	m_vertexList[i++] = {	{ -1.0f, -1.0f, -1.0f },{ 0.0f, -1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 0.0f } };
	m_vertexList[i++] = {	{ 1.0f, -1.0f, -1.0f },{ 0.0f, -1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 0.0f } };
	m_vertexList[i++] = {	{ -1.0f, -1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } };
	m_vertexList[i++] = {	{ 1.0f, -1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f },
							{ 1.0f, 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } };

	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer(36)))
		return E_FAIL;
	Init();
	return S_OK;
}

Component* RCube::clone() noexcept
{
	//return (Component*)(new RCube(*this));
	return (new RCube(*this))->cloneAddition();
}