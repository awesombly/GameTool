#pragma once
#include "Renderer.h"
#include "Particle.h"

#pragma pack(push, 1)
struct ParticleData
{
	D3DXMATRIX matWorld;
	float numTexture;
	D3DXVECTOR4 color;
	//bool isEnable;
};

struct ParticleInfo
{
	bool m_isBillBoard				= true;
	bool m_isRepeat					= true;
	bool m_isScreen					= false;
	//bool DUMMY						= false;
	bool m_isScalarScale			= true;

	float m_lifeTime				= 9999.0f;
	float m_curLife					= 0.0f;
	float m_spawnInterval			= 0.0f;		// 생성 간격
	UINT  m_maxParticleCount		= 0;
	UINT  m_minInitCount			= 0;
	UINT  m_maxInitCount			= 0;
	// 초기 SRT
	D3DXVECTOR3 m_minInitPosition	= Vector3::Zero;
	D3DXVECTOR3 m_maxInitPosition	= Vector3::Zero;
	D3DXVECTOR3 m_minInitScale		= Vector3::One;
	D3DXVECTOR3 m_maxInitScale		= Vector3::One;
	D3DXVECTOR3 m_minInitRotation	= Quaternion::Base;
	D3DXVECTOR3 m_maxInitRotation	= Quaternion::Base;
	// 생명주기별 크기, 색상
	D3DXVECTOR3 m_minScalePerLife	= Vector3::One;
	D3DXVECTOR3 m_maxScalePerLife	= Vector3::One;
	//D3DXVECTOR4 m_minLifePerColor;
	//D3DXVECTOR4 m_maxLifePerColor;

	float m_minLifeCycle			= 0.0f;
	float m_maxLifeCycle			= 0.0f;

	D3DXVECTOR3 m_minDirection		= Vector3::Zero;
	D3DXVECTOR3 m_maxDirection		= Vector3::Zero;
	// 이속
	float m_minMaxMoveSpeed			= 0.0f;
	float m_maxMaxMoveSpeed			= 0.0f;
	float m_minAccMoveSpeed			= 0.0f;
	float m_maxAccMoveSpeed			= 0.0f;
	float m_minCurMoveSpeed			= 0.0f;
	float m_maxCurMoveSpeed			= 0.0f;
	// 회전
	D3DXQUATERNION m_minDirAngle	= Quaternion::Base;
	D3DXQUATERNION m_maxDirAngle	= Quaternion::Base;
	float m_minRotateSpeed			= 0.0f;
	float m_maxRotateSpeed			= 0.0f;

	D3DXVECTOR4 m_minColor			= Color::White;
	D3DXVECTOR4 m_maxColor			= Color::White;

	D3DXVECTOR3 m_dirGravity		= Vector3::Down;
	float m_minGravityPower			= 0.0f;
	float m_maxGravityPower			= 0.0f;
};
#pragma pack(pop)



class ParticleSystem : public ParticleInfo, public Renderer
{
private:
	Particle* m_pParticle;
	forward_list<Particle*> m_particleList;
	stack<Particle*>		m_disabledParticle;

	ID3D11Buffer*		 m_pBuffers[2];				  // 정점 + 인스턴스
	ID3D11Buffer*		 m_pInstanceBuffer = nullptr; // 데이터 등록할 버퍼
	vector<ParticleData> m_dataList;				  // 버퍼 등록 데이터
	// 정점 버퍼 세팅용
	UINT Strides[2];				// 정점 크기
	UINT Offsets[2];				// 시작 오프셋

	bool  m_isFollow			= false;
	bool  m_needUpdateBuffer	= false;
	float m_spawnFrame			= 0.0f;
private:
	void	SpawnParticle()											noexcept;
	HRESULT Create()												noexcept override;
	// 버퍼 생성
	HRESULT CreateInstanceBuffer()									noexcept;
	void	UpdateInstanceBuffer(ID3D11DeviceContext* pDContext)	noexcept;
	void	UpdateConstBuffer(ID3D11DeviceContext* pDContext)		noexcept override;
	// 인스턴스 추가, 삭제
	void	AddInstance(Particle* pParticle)						noexcept;
	void	RemoveInstance(Particle* pParticle)						noexcept;

	Component* cloneAddition()										noexcept override;
public:
	void SetParticle(Particle* pParticle, const bool& preParticleDelete = true)	noexcept;
	Particle* GetParticle()						noexcept;
	// 리셋
	void Update()								noexcept override;
	// 빌보드, 반복여부
	bool isBillBoard()							noexcept;
	void isBillBoard(const bool& isBillBoard)	noexcept;
	bool isRepeat()								noexcept;
	void isRepeat(const bool& isRepeat)			noexcept;
	bool isScreen()								noexcept;
	void isScreen(const bool& isScreen)			noexcept;
	bool isScalarScale()						noexcept;
	void isScalarScale(const bool& isScalar)	noexcept;
	bool isFollow()								noexcept;
	void isFollow(const bool& isFollow)			noexcept;

	bool PostRender(ID3D11DeviceContext* pDContext)		noexcept override;
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept override;
private:
	//ParticleSystem() = default;
public:
	ParticleSystem(const wstring_view& myName, Particle* pParticle, const wstring_view& srcName = L"", const string_view& vertexShaderName = "VS_Particle", const string_view& pixelShaderName = "PS_SingleInstance");
	virtual ~ParticleSystem() = default;
};