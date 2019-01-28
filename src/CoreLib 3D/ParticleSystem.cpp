#include "ParticleSystem.h"
#include "ObjectManager.h"


ParticleSystem::ParticleSystem(const wstring_view& myName, Particle* pParticle, const wstring_view& srcName, const string_view& vertexShaderName, const string_view& pixelShaderName)
{
	m_ppCamera = &ObjectManager::CurCamera;
	//m_isMultiTexture = true;
	m_pParticle = pParticle;
	SetInfo(myName, EComponent::Renderer, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool ParticleSystem::Init() noexcept
{
	m_eRenderType = ERenderType::Particle;
	//m_curParticle = m_particleList.begin();
	m_curSprite = m_pSpriteList->begin();
	m_pBuffers[0] = m_pVertexBuffer;
	m_pBuffers[1] = m_pInstanceBuffer;
	Strides[0] = sizeof(Vertex_PNCT);
	Strides[1] = sizeof(ParticleData);
	Offsets[0] = 0;
	Offsets[1] = 0;
	return true;
}

bool ParticleSystem::Frame(const float& spf, const float& accTime) noexcept
{
	if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);
	// ============================= 파티클, 인스턴싱부 =================================
#pragma region Create
	m_curLife += spf;
	if (m_curLife >= m_lifeTime)
	{
		if (m_isRepeat)
		{
			m_curLife = 0.0f;
			Update();
		}
		else
		{
			m_isEnable = false;
			ObjectManager::Get().DisableObject(m_pParent);
			//ObjectManager::Get().DisableComponent(this);
			//ObjectManager::Get().RemoveObject(m_pParent);
		}
		return false;
	}

	m_spawnFrame += spf;
	while (m_spawnFrame >= m_spawnInterval)
	{
		if (m_maxParticleCount > (m_dataList.size() - m_disabledParticle.size()))
		{
			m_spawnFrame -= m_spawnInterval;
			SpawnParticle();
			continue;
		}
		else if (m_isRepeat)
		{
			Update();
		}
		break;
	}
	if(m_needUpdateBuffer)
		CreateInstanceBuffer();
#pragma endregion
#pragma region Frame
	static D3DXMATRIX matBill;
	matBill = (ObjectManager::Cameras[ECamera::Main])->m_matView;
	matBill._41 = 0.0f;	matBill._42 = 0.0f;	matBill._43 = 0.0f;

	static WORD curData = 0;
	curData = (WORD)-1;
	for(auto& iter : m_particleList)
	{
		++curData;
		if (!iter->IsEnable())
		{
			//m_dataList[curData].numTexture = -0.9f;
			continue;
		}
		if (!iter->Frame(spf, accTime))
		{
			m_dataList[curData].numTexture = -0.9f;
			m_disabledParticle.push(iter);
			continue;
		}
		m_dataList[curData].numTexture = 1.0f;
		// 이동 처리(중력 방향때메 빼둠)
		iter->Translate((iter->m_direction * iter->m_curMoveSpeed + m_dirGravity * iter->m_curGravity) * spf);
		// 행렬 계산
		static D3DXMATRIX matScale, matRotation;
		D3DXMatrixScaling(&matScale, iter->m_scale.x, iter->m_scale.y, iter->m_scale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, iter->m_rotation.y, iter->m_rotation.x, iter->m_rotation.z);
		m_dataList[curData].matWorld = matScale * matRotation;
		if (m_isFollow)
		{
			m_dataList[curData].matWorld._41 = m_pParent->GetPosition().x + iter->m_position.x;
			m_dataList[curData].matWorld._42 = m_pParent->GetPosition().y + iter->m_position.y;
			m_dataList[curData].matWorld._43 = m_pParent->GetPosition().z + iter->m_position.z;
		}
		else
		{
			m_dataList[curData].matWorld._41 = iter->m_position.x;
			m_dataList[curData].matWorld._42 = iter->m_position.y;
			m_dataList[curData].matWorld._43 = iter->m_position.z;
		}
		D3DXMatrixTranspose(&m_dataList[curData].matWorld, &m_dataList[curData].matWorld);
		
		if (m_isBillBoard)
		{
			m_dataList[curData].matWorld *= matBill;
		}
		//m_dataList[curData].numTexture += 1.0f;
		//if(m_dataList[curData].numTexture >= m_pSpriteList->size())
		//	m_dataList[curData].numTexture = 0.1f;
		m_dataList[curData].color = iter->m_color;
	}
#pragma endregion
	DxManager::GetDContext()->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
	return true;
	accTime;
}

bool ParticleSystem::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	pDContext->IASetVertexBuffers(0, 2, m_pBuffers, Strides, Offsets);
	pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pDContext->DrawIndexedInstanced(m_indexCount, (UINT)m_dataList.size(), 0, 0, 0);
	return true;
}

bool ParticleSystem::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable || m_dataList.empty() || m_pInstanceBuffer == nullptr)	return false;
	// 인스턴스 갱신
	DxManager::Get().SetDepthStencilState(EDepthS::D_Less_NoWrite);
	UpdateInstanceBuffer(pDContext);
	PrevRender(pDContext);
	PostRender(pDContext);
	DxManager::Get().SetDepthStencilState(EDepthS::Current);
	return true;
}

bool ParticleSystem::Release() noexcept
{
	Renderer::Release();
	for (auto& iter : m_particleList)
	{
		delete iter;
	}
	return true;
}

void ParticleSystem::SpawnParticle() noexcept
{
	static Particle* pParticle = nullptr;
	// 대기풀 없으면 복사, 있으면 빼옴
	if (m_disabledParticle.empty())
	{
		// 파티클 복사, 추가
		pParticle = m_pParticle->clone();
		AddInstance(pParticle);
		m_needUpdateBuffer = true;
	}
	else
	{
		pParticle = m_disabledParticle.top();
		m_disabledParticle.pop();
		//pParticle->m_position = Vector3::Zero;
		//pParticle->m_scale = Vector3::One;
		//pParticle->m_rotation = Quaternion::Base;
	}
	// 초기화
	pParticle->IsEnable(true);
	pParticle->m_frameCount = 0.0f;
	pParticle->m_curGravity = 0.0f;
	pParticle->SetLifeCycle(m_minLifeCycle, m_maxLifeCycle);

	// 크기
	if (m_isScalarScale)
	{
		pParticle->SetInitScale(m_minInitScale.x, m_maxInitScale.x);
		pParticle->SetScalePerLife(m_minScalePerLife.x, m_maxScalePerLife.x);
	}
	else
	{
		pParticle->SetInitScale(m_minInitScale, m_maxInitScale);
		pParticle->SetScalePerLife(m_minScalePerLife, m_maxScalePerLife);
	}
	pParticle->m_scale = Product(pParticle->m_scale, m_pParent->GetWorldScale());
	// 위치
	pParticle->SetInitPosition(m_minInitPosition, m_maxInitPosition);
	if(!m_isFollow)
		pParticle->m_position += m_pParent->GetWorldPosition();
	// 방향
	pParticle->SetDirection(m_minDirection, m_maxDirection);
	D3DXVec3TransformNormal(&pParticle->m_direction, &pParticle->m_direction, &m_pParent->GetRotationMatrix());
	// 이속
	pParticle->SetMaxMoveSpeed(m_minMaxMoveSpeed, m_maxMaxMoveSpeed);
	pParticle->SetAccMoveSpeed(m_minAccMoveSpeed, m_maxAccMoveSpeed);
	pParticle->SetCurMoveSpeed(m_minCurMoveSpeed, m_maxCurMoveSpeed);
	// 회전
	pParticle->SetInitRotation(m_minInitRotation, m_maxInitRotation);
	//pParticle->m_rotation -= m_pParent->GetWorldRotation();
	pParticle->SetDirAngle(m_minDirAngle, m_maxDirAngle);
	pParticle->SetRotateSpeed(m_minRotateSpeed, m_maxRotateSpeed);
	// 등등
	pParticle->SetColor(m_minColor, m_maxColor);
	pParticle->SetGravityPower(m_minGravityPower, m_maxGravityPower);
}

HRESULT ParticleSystem::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);

	// 정점 리스트
	m_vertexList.resize(4);

	m_vertexList[0] = { { -0.5f, 0.5f,  0.0f },
	{ 0.0f, 0.0f, -1.0f },
	{ 1.0f, 1.0f,  1.0f, 0.1f },
	{ 0.0f, 0.0f } };
	m_vertexList[1] = { { 0.5f, 0.5f,  0.0f },
	{ 0.0f, 0.0f, -1.0f },
	{ 1.0f, 1.0f,  1.0f, 0.1f },
	{ 1.0f, 0.0f } };
	m_vertexList[2] = { { -0.5f, -0.5f, 0.0f },
	{ 0.0f, 0.0f, -1.0f },
	{ 1.0f, 1.0f,  1.0f, 0.1f },
	{ 0.0f, 1.0f } };
	m_vertexList[3] = { { 0.5f, -0.5f, 0.0f },
	{ 0.0f, 0.0f, -1.0f },
	{ 1.0f, 1.0f,  1.0f, 0.1f },
	{ 1.0f, 1.0f } };
	// 버퍼들 생성
	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer(6)))
		return E_FAIL;

 	Init();
	return S_OK;
}


void ParticleSystem::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	// 상수버퍼를 갱신
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	// 접근하고 수정하고 접근해제
	pDContext->Map(m_pMatrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CB_VSMatrix* pConstData = (CB_VSMatrix*)MappedResource.pData;		// 바꿀 객체
	
	*pConstData = (*m_ppCamera)->m_cbVS;
	if (!m_isScreen)
	{
		pConstData->m_matWorld = Matrix::Identity;
	}
		
	pDContext->Unmap(m_pMatrixCBuffer, 0);
	pDContext;
}

void ParticleSystem::UpdateInstanceBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	// 인스턴스 버퍼 갱신
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	// 접근하고 수정하고 접근해제
	pDContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	ParticleData* pInstance = (ParticleData*)MappedResource.pData;		// 바꿀 객체

	memcpy(pInstance, &m_dataList.at(0), sizeof(ParticleData) * m_dataList.size());
	pDContext->Unmap(m_pInstanceBuffer, 0);
}

HRESULT ParticleSystem::CreateInstanceBuffer() noexcept
{
	m_needUpdateBuffer = false;
	if (m_dataList.empty())
		return E_FAIL;
	static D3D11_BUFFER_DESC initDesc;
	initDesc = { 0, };
	initDesc.ByteWidth = (UINT)(sizeof(ParticleData) * m_dataList.size());	// 데이터 크기 * 갯수
	initDesc.Usage = D3D11_USAGE_DYNAMIC;				// 사용 모드(CPU->GPU 접근제어)
	initDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버퍼 타입
	initDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU 접근 제어
	initDesc.MiscFlags = 0;
	//initDesc.StructureByteStride = 0;
	//initData.SysMemPitch = 0;
	//initData.SysMemSlicePitch = 0;

	static D3D11_SUBRESOURCE_DATA InitData;
	InitData = { 0, };
	InitData.pSysMem = &m_dataList.at(0);		// GPU에 전달 할 메모리

	if (m_pInstanceBuffer != nullptr)
	{
		static ID3D11Buffer* newBuffer;
		DxManager::GetDevice()->CreateBuffer(&initDesc, &InitData, &newBuffer);
		std::swap(m_pInstanceBuffer, newBuffer);
		newBuffer->Release();
		//DxManager::GetDContext()->UpdateSubresource(m_pInstanceBuffer, 0, 0, &m_dataList.at(0), 0, 0);
		//DxManager::GetDContext()->CopyResource(m_pInstanceBuffer, tempBuffer);
		m_pBuffers[1] = m_pInstanceBuffer;
		return S_OK;
	}
	//					버퍼 생성(버퍼정보,	   소스 정보, 대상 버퍼)
	DxManager::GetDevice()->CreateBuffer(&initDesc, &InitData, &m_pInstanceBuffer);
	m_pBuffers[1] = m_pInstanceBuffer;
	return S_OK;
}

void ParticleSystem::AddInstance(Particle* pParticle) noexcept
{
	m_particleList.push_front(pParticle);
	m_dataList.push_back({});
	//if(m_pInstanceBuffer == nullptr)
		//CreateInstanceBuffer();
	//DxManager::GetDContext()->UpdateSubresource(m_pInstanceBuffer, 0, nullptr, &m_dataList.at(0), 0, 0);
}

void ParticleSystem::RemoveInstance(Particle* pParticle) noexcept
{
	auto&& iter = find(m_particleList.begin(), m_particleList.end(), pParticle);
	if (iter != m_particleList.end())
	{
		m_dataList.pop_back();
		m_particleList.remove(*iter);
		m_needUpdateBuffer = true;
		//CreateInstanceBuffer();
		//DxManager::GetDContext()->UpdateSubresource(m_pInstanceBuffer, 0, nullptr, &m_dataList.at(0), 0, 0);
	}
}


void ParticleSystem::SetParticle(Particle* pParticle, const bool& preParticleDelete) noexcept
{
	if (m_pParticle != nullptr && preParticleDelete)
		delete m_pParticle;
	m_pParticle = pParticle;
}

Particle* ParticleSystem::GetParticle() noexcept
{
	return m_pParticle;
}

void ParticleSystem::Update() noexcept
{
	static auto clearEvent = [](void* pVoid, void*) {
		auto pParticle = ((ParticleSystem*)pVoid);
		for (auto& iter : pParticle->m_particleList)
		{
			delete iter;
		}
		pParticle->m_particleList.clear();
		pParticle->m_dataList.clear();
		while (!pParticle->m_disabledParticle.empty())
			pParticle->m_disabledParticle.pop();
		pParticle->m_curLife = 0.0f;
		pParticle->m_frameCount = 0.0f;
		pParticle->isEnable(true);
		///
		auto count = (int)(RandomNormal() * (pParticle->m_maxInitCount - pParticle->m_minInitCount) + pParticle->m_minInitCount);
		for (int i = 0; i < count; ++i)
		{
			pParticle->SpawnParticle();
		}
	};
	ObjectManager::PostFrameEvent.emplace(clearEvent, this, nullptr);
}


bool ParticleSystem::isBillBoard() noexcept
{
	return m_isBillBoard;
}
void ParticleSystem::isBillBoard(const bool& isBillBoard) noexcept
{
	m_isBillBoard = isBillBoard;
}

bool ParticleSystem::isRepeat()	noexcept
{
	return m_isRepeat;
}
void ParticleSystem::isRepeat(const bool& isRepeat) noexcept
{
	m_isRepeat = isRepeat;
}

bool ParticleSystem::isScreen()	noexcept
{
	return m_isScreen;
}
void ParticleSystem::isScreen(const bool& isScreen) noexcept
{
	m_isScreen = isScreen;
}

bool ParticleSystem::isScalarScale() noexcept
{
	return m_isScalarScale;
}
void ParticleSystem::isScalarScale(const bool& isScalar) noexcept
{
	m_isScalarScale = isScalar;
}

bool ParticleSystem::isFollow() noexcept
{
	return m_isFollow;
}
void ParticleSystem::isFollow(const bool& isFollow) noexcept
{
	m_isFollow = isFollow;
}


Component* ParticleSystem::clone() noexcept
{
	return (new ParticleSystem(*this))->cloneAddition();
}

Component* ParticleSystem::cloneAddition() noexcept
{
	Renderer::cloneAddition();
	///Update();
	// 파티클 초기화
	m_pParticle = m_pParticle->clone();
	m_curLife = 0.0f;
	m_frameCount = 0.0f;

	m_particleList.clear();
	m_dataList.clear();
	while (!m_disabledParticle.empty())
		m_disabledParticle.pop();

	m_pInstanceBuffer = nullptr;
	CreateInstanceBuffer();
	Init();
	return this;
}