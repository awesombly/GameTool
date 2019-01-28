#include "RLine.h"
#include "ObjectManager.h"

RLine::RLine(const wstring_view& myName, const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_ppCamera = &ObjectManager::CurCamera;
	m_myName = myName;
	SetShaderLayout(vertexShaderName, pixelShaderName);
	//if (srcName.empty())
		//m_srcName = L"None";
	//else
		//m_srcName = srcName;
	Create();
}

bool RLine::Init() noexcept
{
	Renderer::Init();
	m_comptType = EComponent::Etc;
	return true;
}

bool RLine::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;
	//Renderer::Frame(spf, accTime);
	return true;
	spf; accTime;
}

bool RLine::PrevRender(ID3D11DeviceContext* pDContext) noexcept
{
	// 리소스를 픽셀 쉐이더에 바인딩(시작 인덱스, 쉐이더 자원 갯수, 리소스뷰)
	pDContext->IASetInputLayout(m_pInputLayout);
	// 오브젝트 쉐이더, 버퍼, 리소스 설정 -> 드로우
	UpdateConstBuffer(pDContext);
	pDContext->VSSetConstantBuffers(0, 1, &m_pMatrixCBuffer);	// 상수 버퍼
	pDContext->VSSetShader(m_pVShader, nullptr, 0);			// 정점쉐이더
	pDContext->PSSetShader(m_pPShader, nullptr, 0);			// 픽셀 쉐이더
	return true;
}

bool RLine::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	static UINT stride = (UINT)sizeof(Vertex_PC);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &Offset);
	//pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//pDContext->DrawIndexed(2, 0, 0);
	pDContext->Draw(2, 0);
	return true;
}

bool RLine::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (m_isDebugLine && !Input::isDebug) return false;
	// 레스터라이저 설정
	DxManager::GetInstance().SetRasterizerState(ERasterS::Line);
	Renderer::Render(pDContext);
	DxManager::GetInstance().SetRasterizerState(ERasterS::Current);
	return true;
}

bool RLine::Release()	noexcept
{
	Renderer::Release();
	return true;
}

void RLine::SetLineInfo(ID3D11DeviceContext* pDContext, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR4& color, const bool& isDebugLine) noexcept
{
	m_isDebugLine = isDebugLine;
	m_vertexList[0].pos = start;
	m_vertexList[1].pos = end;
	m_vertexList[0].col = color;
	m_vertexList[1].col = color;
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

HRESULT RLine::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// 정점 리스트
	m_vertexList.resize(2);

	// 2개의 점 표현
	m_vertexList[0] = { { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f,  1.0f, 1.0f } };
	m_vertexList[1] = { { 1.0f,  0.0f, 0.0f }, { 0.0f, 1.0f,  1.0f, 1.0f } };

	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer(2) ))
		return E_FAIL;
	Init();
	return S_OK;
}

HRESULT RLine::CreateVertexBuffer()
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

bool RLine::isDebugLine()
{
	return m_isDebugLine;
}
void RLine::isDebugLine(const bool& isDebugLine)
{
	m_isDebugLine = isDebugLine;
}

Component* RLine::clone() noexcept
{
	//return (Component*)(new RLine(*this));
	return (new RLine(*this))->cloneAddition();
}