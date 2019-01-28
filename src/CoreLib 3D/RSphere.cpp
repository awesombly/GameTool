#include "RSphere.h"
#include "ObjectManager.h"

RSphere::RSphere(const UINT& slices, const wstring_view& myName, const wstring_view& srcName,
			 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_radius = 1.0f;
	m_slices = slices;
	SetInfo(myName, EComponent::Renderer, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool RSphere::Init() noexcept
{
	Renderer::Init();
	return true;
}

bool RSphere::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);
	return true;
}

bool RSphere::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool RSphere::Release()	noexcept
{
	Renderer::Release();
	return true;
}



HRESULT RSphere::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// 구체 생성
	CreateSphere(m_slices, m_radius);

	vector<Vertex_PNCT> tempVertex;
	std::swap(m_vertexList, tempVertex);
	// 버텍스 중복 제거 + 인덱스 추가
	bool notFound = false;
	size_t iVertex = 0, vertexSize = 0;
	size_t tempSize = tempVertex.size();
	for (size_t temp = 0; temp < tempSize; ++temp)
	{
		notFound = true;
		vertexSize = m_vertexList.size();
		for (iVertex = 0; iVertex  < vertexSize; ++iVertex)
		{
			if (tempVertex[temp] == m_vertexList[iVertex])
			{
				m_indexList.push_back((WORD)iVertex);
				notFound = false;
				break;
			}
		}
		if (notFound)
		{
			m_vertexList.emplace_back(std::move(tempVertex[temp]));
			m_indexList.push_back((WORD)m_vertexList.size() - 1);
		}
	}

	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer((UINT)m_indexList.size(), false)))
		return E_FAIL;
	Init();
	return S_OK;
}

void RSphere::CreateSphere(const UINT& slices, const float& radius)
{
	m_radius = radius;
	vector<D3DXVECTOR3> vertexArray;
	MakeSpherePoint(&vertexArray, slices);

	int iPos = 0, iNext = 0;
	for (DWORD out = 0; out < slices; ++out)
	{
		for (DWORD in = 0; in < slices; ++in)
		{
			if (in == slices - 1)
				iNext = 0;
			else
				iNext = in + 1;
			Vertex_PNCT v0, v1, v2, v3, v4, v5;
			iPos = (out * slices * 6) + (in * 6);
			v0.pos = vertexArray[out * slices + in];
			v1.pos = vertexArray[out * slices + iNext];

			if (out != slices - 1)
				v2.pos = vertexArray[(out + 1) * slices + in];
			else
				v2.pos = { 0.0f, -m_radius, 0.0f };
			
			v3.pos = v2.pos;
			v4.pos = v1.pos;

			if (out != slices - 1)
				v5.pos = vertexArray[(out + 1) * slices + iNext];
			else
				v5.pos = { 0.0f, -m_radius, 0.0f };

			D3DXVec3Normalize(&v0.nor, &v0.pos);
			D3DXVec3Normalize(&v1.nor, &v1.pos);
			D3DXVec3Normalize(&v2.nor, &v2.pos);
			D3DXVec3Normalize(&v3.nor, &v3.pos);
			D3DXVec3Normalize(&v4.nor, &v4.pos);
			D3DXVec3Normalize(&v5.nor, &v5.pos);

			v0.col = v1.col = v2.col = v3.col = v4.col = v5.col = Vector4::One;
			// 구 텍스쳐 좌표
			v0.tex = { atan2f(v0.nor.z, v0.nor.x) / (PI * 2.0f) + 0.5f, asinf(-v0.nor.y) / PI + 0.5f };
			v1.tex = { atan2f(v1.nor.z, v1.nor.x) / (PI * 2.0f) + 0.5f, asinf(-v1.nor.y) / PI + 0.5f };
			v2.tex = { atan2f(v2.nor.z, v2.nor.x) / (PI * 2.0f) + 0.5f, asinf(-v2.nor.y) / PI + 0.5f };
			v3.tex = { atan2f(v3.nor.z, v3.nor.x) / (PI * 2.0f) + 0.5f, asinf(-v3.nor.y) / PI + 0.5f };
			v4.tex = { atan2f(v4.nor.z, v4.nor.x) / (PI * 2.0f) + 0.5f, asinf(-v4.nor.y) / PI + 0.5f };
			v5.tex = { atan2f(v5.nor.z, v5.nor.x) / (PI * 2.0f) + 0.5f, asinf(-v5.nor.y) / PI + 0.5f };

			// v0.tex.x(0.94f) ---- v1.tex.x(0.0f) 일 경우, v1.tex.x = 1.0f로 조정?
			if (fabsf(v1.tex.x - v0.tex.x) > 0.9f)
			{
				if (fabsf(v1.tex.x) <= 0.01f)
					v1.tex.x = 1.0f;
				if (fabsf(v0.tex.x) <= 0.01f)
					v0.tex.x = 1.0f;
			}
			if (fabsf(v2.tex.x - v0.tex.x) > 0.9f)
			{
				if (fabsf(v2.tex.x) <= 0.01f)
					v2.tex.x = 1.0f;
				if (fabsf(v0.tex.x) <= 0.01f)
					v0.tex.x = 1.0f;
			}
			if (fabsf(v4.tex.x - v3.tex.x) > 0.9f)
			{
				if (fabsf(v4.tex.x) <= 0.01f)
					v4.tex.x = 1.0f;
				if (fabsf(v3.tex.x) <= 0.01f)
					v3.tex.x = 1.0f;
			}
			if (fabsf(v5.tex.x - v3.tex.x) > 0.9f)
			{
				if (fabsf(v5.tex.x) <= 0.01f)
					v5.tex.x = 1.0f;
				if (fabsf(v3.tex.x) <= 0.01f)
					v3.tex.x = 1.0f;
			}
			m_vertexList.emplace_back(std::move(v0));
			m_vertexList.emplace_back(std::move(v1));
			m_vertexList.emplace_back(std::move(v2));
			m_vertexList.emplace_back(std::move(v3));
			m_vertexList.emplace_back(std::move(v4));
			m_vertexList.emplace_back(std::move(v5));
		}
	}
	vertexArray.clear();
}

void RSphere::MakeSpherePoint(vector<D3DXVECTOR3>* pSpherePoints, const UINT& slices)
{
	pSpherePoints->resize(slices * slices);

	for (DWORD out = 0; out < slices; ++out)
	{
		float theta = (PI * out) / slices;
		for (DWORD in = 0; in < slices; ++in)
		{
			int iPos = out * slices + in;
			float phi = (2 * PI * in) / slices;
			(*pSpherePoints)[iPos].x = m_radius * (float)(sinf(theta) * cosf(phi));
			(*pSpherePoints)[iPos].z = m_radius * (float)(sinf(theta) * sinf(phi));
			(*pSpherePoints)[iPos].y = m_radius * (float)cosf(theta);
		}
	}
}


Component* RSphere::clone() noexcept
{
	//return (Component*)(new RSphere(*this));
	return (new RSphere(*this))->cloneAddition();
}