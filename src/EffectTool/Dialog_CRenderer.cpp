#include "stdafx.h"
#include "EffectTool.h"
#include "Dialog_CRenderer.h"
#include "afxdialogex.h"
#include "Dialog_Hierarchy.h"
#include "Dialog_Inspector.h"

Dialog_CRenderer* Dialog_CRenderer::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_CRenderer, CFormView)

Dialog_CRenderer::Dialog_CRenderer()
	: CFormView(IDD_DIALOG_CRenderer)
{
	Instance = this;
}

Dialog_CRenderer::~Dialog_CRenderer()
{}
	

void Dialog_CRenderer::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PresetList, m_TextureList);
	DDX_Control(pDX, IDC_R_onDiffuse, m_onDiffuse);
	DDX_Control(pDX, IDC_R_onCubeMap, m_onCubeMap);
	DDX_Control(pDX, IDC_R_onNormalMap, m_onNormalMap);
	DDX_Control(pDX, IDC_R_onEffect, m_onEffect);
	DDX_Control(pDX, IDC_R_onSprite, m_onSprite);
	DDX_Control(pDX, IDC_R_onEffect3d, m_onEffect3D);
	DDX_Control(pDX, IDC_R_onHeightMap, m_onHeightMap);
	DDX_Control(pDX, IDC_R_onEtc, m_onEtc);
	DDX_Control(pDX, IDC_R_EnviType, m_EnviType);
	DDX_Control(pDX, IDC_R_ColorX, m_ColorX);
	DDX_Control(pDX, IDC_R_ColorY, m_ColorY);
	DDX_Control(pDX, IDC_R_ColorZ, m_ColorZ);
	DDX_Control(pDX, IDC_R_ColorW, m_ColorW);
	DDX_Control(pDX, IDC_P_isEnable, m_isEnable);
	DDX_Control(pDX, ID_R_EnviApply2, m_HeightApply);
	DDX_Control(pDX, ID_R_EnviReset2, m_HeightRelease);
	DDX_Control(pDX, ID_R_NormalApply, m_NormalApply);
	DDX_Control(pDX, ID_R_NormalReset, m_NormalRelease);
	DDX_Control(pDX, ID_R_EnviApply, m_EnviApply);
	DDX_Control(pDX, ID_R_EnviReset, m_EnviRelease);
	DDX_Control(pDX, IDC_R_HeightCellSize, m_HeightCellSize);
	DDX_Control(pDX, IDC_R_OffsetX, m_OffsetX);
	DDX_Control(pDX, IDC_R_OffsetY, m_OffsetY);
	DDX_Control(pDX, IDC_R_SpriteFrame, m_SpriteFrame);
	DDX_Control(pDX, IDC_R_StartOffsetX, m_StartOffsetX);
	DDX_Control(pDX, IDC_R_StartOffsetY, m_StartOffsetY);
	DDX_Control(pDX, IDC_R_SpriteCount, m_SpriteCount);
	DDX_Control(pDX, IDC_R_LightRate, m_LightRate);
	DDX_Control(pDX, IDC_R_ShadowRate, m_ShadowRate);
	DDX_Control(pDX, ID_R_LightApply, m_LightApply);
	DDX_Control(pDX, ID_R_ShadowApply, m_ShadowApply);
}

Dialog_CRenderer* Dialog_CRenderer::CreateOne(CWnd* pParent)
{
	Dialog_CRenderer* pForm = new Dialog_CRenderer();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

// 텍스쳐 파일 로드
void Dialog_CRenderer::InitTextureList() noexcept
{
	// 텍스쳐 로드
	std::thread initTexture(&Dialog_CRenderer::LoadTextureList, this);
	initTexture.detach();

	//// 스프라이트 로드
	//std::thread initSprite(&Dialog_CRenderer::LoadSpriteList, this);
	//initSprite.detach();
}

void Dialog_CRenderer::LoadTextureList() noexcept
{
	// 환경매핑 타입 설정
	m_EnviType.AddString(L"Basic");
	m_EnviType.AddString(L"Fresnel");
	m_EnviType.AddString(L"Refraction");
	m_EnviType.SetCurSel(0);

	/// 디렉터리 순회
	filesystem::path fileDir = L"../../data/texture";
	if (filesystem::is_directory(fileDir))
	{
		for (const auto& iter : filesystem::directory_iterator(fileDir))
		{
			// 파일 추가
			if (filesystem::is_regular_file(iter.status()))
			{
				auto pTexture = DxManager::GetInstance().GetTexture(iter.path().filename().c_str());
				TexItem texItem;
				texItem.Name = iter.path().filename();
				texItem.pTexture = pTexture;
				texItem.eTexType = ETextureType::Diffuse;
				m_TexItemMap[ETextureType::Diffuse].emplace_back(std::move(texItem));
				//int index = m_TextureList.AddString(iter.path().filename().c_str());
				//m_TextureList.SetItemData(index, (DWORD_PTR)&pTexture);
			}
			/// 디렉터리일시
			else if (filesystem::is_directory(iter.status()))
			{
				ETextureType eTexType = ETextureType::Etc;
				if (iter.path().filename() == "NormalMap")
					eTexType = ETextureType::NormalMap;
				else if (iter.path().filename() == "CubeMap")
					eTexType = ETextureType::CubeMap;
				else if (iter.path().filename() == "HeightMap")
					eTexType = ETextureType::HeightMap;
				else if (iter.path().filename() == "Effect")
					eTexType = ETextureType::Effect;
				else if (iter.path().filename() == "Effect3d")
					eTexType = ETextureType::Effect3D;

				// 파일 추가
				for (const auto& inIter : filesystem::directory_iterator(iter.path()))
				{
					if (filesystem::is_regular_file(inIter.status()))
					{
						filesystem::path url = inIter.path().parent_path().filename().wstring() + L"/" + inIter.path().filename().c_str();
						auto pTexture = DxManager::GetInstance().GetTexture(url.c_str());
						if (pTexture == nullptr)
							continue;
						TexItem texItem;
						texItem.Name = url.c_str();
						texItem.pTexture = pTexture;
						texItem.eTexType = eTexType;

						m_TexItemMap[eTexType].emplace_back(std::move(texItem));
					}
				}
			}
		}
	}
	UpdateTextureList();
}

void Dialog_CRenderer::LoadSpriteList() noexcept
{
	// sprite 파일 읽음
	ObjectManager::Get().ReadSpriteScript();
	// 스프라이트 추가
	for (auto& iter : ObjectManager::Get().GetSpriteList())
	{
		TexItem texItem;
		texItem.Name = iter.first;
		texItem.pTexture = (Texture*)&iter.second;
		texItem.eTexType = ETextureType::Sprite;
		m_TexItemMap[ETextureType::Sprite].emplace_back(std::move(texItem));
	}
	UpdateTextureList();
}


// 다이얼로그 갱신
void Dialog_CRenderer::UpdateEdit(const Component* pComp) noexcept
{
	if (pComp == nullptr) return;
	m_dontUpdate = true;
	m_isEnable.SetCheck(pComp->isEnable());
	// 텍스쳐 리스트 갱신
	UpdateTextureList();
	// 컬러 갱신
	D3DXVECTOR4 color = ((Renderer*)pComp)->m_vertexList.front().col;
	m_ColorX.SetWindowTextW(std::to_wstring(color.x).c_str());
	m_ColorY.SetWindowTextW(std::to_wstring(color.y).c_str());
	m_ColorZ.SetWindowTextW(std::to_wstring(color.z).c_str());
	m_ColorW.SetWindowTextW(std::to_wstring(color.w).c_str());

	m_HeightCellSize.SetWindowTextW(L"1.0");
	
	m_OffsetX.SetWindowTextW(L"1.0");
	m_OffsetY.SetWindowTextW(L"1.0");
	m_StartOffsetX.SetWindowTextW(L"1.0");
	m_StartOffsetY.SetWindowTextW(L"1.0");
	m_SpriteFrame.SetWindowTextW(L"1.0");
	m_SpriteCount.SetWindowTextW(L"1.0");

	m_LightRate.SetWindowTextW(std::to_wstring(((Renderer*)pComp)->GetLightRate()).c_str());
	m_ShadowRate.SetWindowTextW(std::to_wstring(((Renderer*)pComp)->GetShadowRate()).c_str());

	m_dontUpdate = false;

	UpdateEditEnable(((Renderer*)pComp)->m_eRenderType);
}

// 랜더러 타입별 활성화 설정
void Dialog_CRenderer::UpdateEditEnable(const ERenderType& eRendType) noexcept
{
	switch (eRendType)
	{
	case ERenderType::Basic:
	{
		m_HeightApply.EnableWindow(0);
		m_HeightRelease.EnableWindow(0);
		m_HeightCellSize.EnableWindow(0);
		m_NormalApply.EnableWindow(1);
		m_NormalRelease.EnableWindow(1);
		m_EnviApply.EnableWindow(1);
		m_EnviRelease.EnableWindow(1);
		m_EnviType.EnableWindow(1);
		m_ColorX.EnableWindow(1);
		m_ColorY.EnableWindow(1);
		m_ColorZ.EnableWindow(1);
		m_ColorW.EnableWindow(1);
		m_LightRate.EnableWindow(1);
		m_LightApply.EnableWindow(1);
		m_ShadowRate.EnableWindow(1);
		m_ShadowApply.EnableWindow(1);
	}	break;
	case ERenderType::HeightMap:
	{
		m_HeightApply.EnableWindow(1);
		m_HeightRelease.EnableWindow(1);
		m_HeightCellSize.EnableWindow(1);
		m_NormalApply.EnableWindow(1);
		m_NormalRelease.EnableWindow(1);
		m_EnviApply.EnableWindow(1);
		m_EnviRelease.EnableWindow(1);
		m_EnviType.EnableWindow(1);
		m_ColorX.EnableWindow(1);
		m_ColorY.EnableWindow(1);
		m_ColorZ.EnableWindow(1);
		m_ColorW.EnableWindow(1);
		m_LightRate.EnableWindow(1);
		m_LightApply.EnableWindow(1);
		m_ShadowRate.EnableWindow(1);
		m_ShadowApply.EnableWindow(1);
	}	break;
	case ERenderType::Particle:
	{
		m_HeightApply.EnableWindow(0);
		m_HeightRelease.EnableWindow(0);
		m_HeightCellSize.EnableWindow(0);
		m_NormalApply.EnableWindow(0);
		m_NormalRelease.EnableWindow(0);
		m_EnviApply.EnableWindow(0);
		m_EnviRelease.EnableWindow(0);
		m_EnviType.EnableWindow(0);
		m_ColorX.EnableWindow(0);
		m_ColorY.EnableWindow(0);
		m_ColorZ.EnableWindow(0);
		m_ColorW.EnableWindow(0);
		m_LightRate.EnableWindow(0);
		m_LightApply.EnableWindow(0);
		m_ShadowRate.EnableWindow(0);
		m_ShadowApply.EnableWindow(0);
	}	break;
	}
}

void Dialog_CRenderer::UpdateTextureList() noexcept
{
	m_TextureList.ResetContent();

	for (auto& iter : m_TexItemMap)
	{
		switch (iter.first)
		{
		case ETextureType::Diffuse:
		{
		if (!m_onDiffuse.GetCheck())
			continue;
		}	break;
		case ETextureType::CubeMap:
		{
			if (!m_onCubeMap.GetCheck())
				continue;
		}	break;
		case ETextureType::NormalMap:
		{
			if (!m_onNormalMap.GetCheck())
				continue;
		}	break;
		case ETextureType::HeightMap:
		{
			if (!m_onHeightMap.GetCheck())
				continue;
		}	break;
		case ETextureType::Effect:
		{
			if (!m_onEffect.GetCheck())
				continue;
		}	break;
		case ETextureType::Effect3D:
		{
			if (!m_onEffect3D.GetCheck())
				continue;
		}	break;
		case ETextureType::Sprite:
		{
			if (!m_onSprite.GetCheck())
				continue;
		}	break;
		case ETextureType::Etc:
		{
			if (!m_onEtc.GetCheck())
				continue;
		}	break;
		}
		
		for (auto& inIter : iter.second)
		{
			int index = m_TextureList.AddString(inIter.Name.c_str());
			m_TextureList.SetItemData(index, (DWORD_PTR)&inIter);
		}
	}
}


BEGIN_MESSAGE_MAP(Dialog_CRenderer, CFormView)
	ON_BN_CLICKED(ID_PresetLoad, &Dialog_CRenderer::OnBnClickedPresetload)
	ON_BN_CLICKED(IDC_R_onDiffuse, &Dialog_CRenderer::OnBnClickedRondiffuse)
	ON_BN_CLICKED(IDC_R_onCubeMap, &Dialog_CRenderer::OnBnClickedRoncubemap)
	ON_BN_CLICKED(IDC_R_onNormalMap, &Dialog_CRenderer::OnBnClickedRonnormalmap)
	ON_BN_CLICKED(IDC_R_onHeightMap, &Dialog_CRenderer::OnBnClickedRonheightmap)
	ON_BN_CLICKED(IDC_R_onEtc, &Dialog_CRenderer::OnBnClickedRonetc)
	ON_BN_CLICKED(ID_R_DiffuseApply, &Dialog_CRenderer::OnBnClickedRDiffuseapply)
	ON_BN_CLICKED(ID_R_NormalApply, &Dialog_CRenderer::OnBnClickedRNormalapply)
	ON_BN_CLICKED(ID_R_EnviApply, &Dialog_CRenderer::OnBnClickedREnviapply)
	ON_BN_CLICKED(ID_R_DiffuseReset, &Dialog_CRenderer::OnBnClickedRDiffusereset)
	ON_BN_CLICKED(ID_R_NormalReset, &Dialog_CRenderer::OnBnClickedRNormalreset)
	ON_BN_CLICKED(ID_R_EnviReset, &Dialog_CRenderer::OnBnClickedREnvireset)
	ON_CBN_SELCHANGE(IDC_R_EnviType, &Dialog_CRenderer::OnCbnSelchangeREnvitype)
	ON_EN_CHANGE(IDC_R_ColorX, &Dialog_CRenderer::OnEnChangeRColorx)
	ON_EN_CHANGE(IDC_R_ColorY, &Dialog_CRenderer::OnEnChangeRColory)
	ON_EN_CHANGE(IDC_R_ColorZ, &Dialog_CRenderer::OnEnChangeRColorz)
	ON_EN_CHANGE(IDC_R_ColorW, &Dialog_CRenderer::OnEnChangeRColorw)
	ON_BN_CLICKED(IDC_P_isEnable, &Dialog_CRenderer::OnBnClickedPisenable)
	ON_BN_CLICKED(IDC_R_onEffect, &Dialog_CRenderer::OnBnClickedRoneffect)
	ON_BN_CLICKED(IDC_R_onSprite, &Dialog_CRenderer::OnBnClickedRonsprite)
	ON_BN_CLICKED(IDC_R_onEffect3d, &Dialog_CRenderer::OnBnClickedRoneffect3d)
	ON_BN_CLICKED(ID_R_EnviApply2, &Dialog_CRenderer::OnBnClickedREnviapply2)
	ON_BN_CLICKED(ID_R_EnviReset2, &Dialog_CRenderer::OnBnClickedREnvireset2)
	ON_BN_CLICKED(ID_R_DiffuseApply2, &Dialog_CRenderer::OnBnClickedRDiffuseapply2)
	ON_BN_CLICKED(ID_R_LightApply, &Dialog_CRenderer::OnBnClickedRLightapply)
	ON_BN_CLICKED(ID_R_ShadowApply, &Dialog_CRenderer::OnBnClickedRShadowapply)
END_MESSAGE_MAP()


// ============================ Dialog_CRenderer 메시지 처리기 ===========================



// 텍스쳐 로드
void Dialog_CRenderer::OnBnClickedPresetload()
{
	// 불러오기 버튼
	CFileDialog fileDia(true);				// true = 불러오기, false = 저장인듯
	if (fileDia.DoModal() != IDOK)
		return;

	std::filesystem::path fpath = fileDia.GetPathName().GetBuffer();
	for (auto& iter : m_TexItemMap)
	{
		for (auto& inIter : iter.second)
		{
			if (fpath.filename() == inIter.Name)
			{
				MessageBox(L"이미 등록된 객체 입니다.", L"Ang~?");
				return;
			}
		}
	}
	TexItem texItem;
	texItem.Name = fpath.filename();// fileDia.GetFileName().GetString();
	texItem.pTexture = DxManager::GetInstance().GetTexture(fpath.c_str(), true);
	//auto pTexture = DxManager::GetInstance().GetTexture(fileDia.GetFileName().GetString());

	//int index = m_TextureList.AddString(fileDia.GetFileName().GetString());
	//m_TextureList.SetItemData(index, (DWORD_PTR)&texItem);

	m_TexItemMap[ETextureType::Diffuse].emplace_back(std::move(texItem));
	UpdateTextureList();
}

// 활성화
void Dialog_CRenderer::OnBnClickedPisenable()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		return;
	}

	pComp->isEnable(m_isEnable.GetCheck());
}

// 텍스쳐 필터
void Dialog_CRenderer::OnBnClickedRondiffuse()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRoncubemap()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRonnormalmap()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRonheightmap()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRonetc()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRoneffect()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRonsprite()
{
	UpdateTextureList();
}
void Dialog_CRenderer::OnBnClickedRoneffect3d()
{
	UpdateTextureList();
}



// 텍스쳐 매핑(디퓨즈)
void Dialog_CRenderer::OnBnClickedRDiffuseapply()
{
	int index = m_TextureList.GetCurSel();
	if (index == LB_ERR)
	{
		MessageBox(L"텍스쳐를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}
	
	// 텍스쳐 적용
	auto pTexItem = (TexItem*)m_TextureList.GetItemData(index);
	if (pTexItem->eTexType == ETextureType::Sprite)
	{
		pComp->SetSpriteList((vector<Sprite>*)pTexItem->pTexture);
		pComp->m_srcName = pTexItem->Name;
		return;
	}

	pComp->SetSpriteList(pTexItem->Name);
}
// 노말
void Dialog_CRenderer::OnBnClickedRNormalapply()
{
	int index = m_TextureList.GetCurSel();
	if (index == LB_ERR)
	{
		MessageBox(L"텍스쳐를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}

	// 텍스쳐 적용
	auto pTexItem = (TexItem*)m_TextureList.GetItemData(index);
	if (pTexItem->eTexType == ETextureType::Sprite)
	{
		MessageBox(L"스프라이트는 Diffuse 만 가능합니다.", L"삐빅-");
		return;
	}

	pComp->SetNormalMap(pTexItem->Name);
}
// 환경
void Dialog_CRenderer::OnBnClickedREnviapply()
{
	int index = m_TextureList.GetCurSel();
	if (index == LB_ERR)
	{
		MessageBox(L"텍스쳐를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}
	// 텍스쳐 적용
	auto pTexItem = (TexItem*)m_TextureList.GetItemData(index);
	if (pTexItem->eTexType == ETextureType::Sprite)
	{
		MessageBox(L"스프라이트는 Diffuse 만 가능합니다.", L"삐빅-");
		return;
	}

	pComp->SetEnviromentMap(pTexItem->Name, (EEnviType)m_EnviType.GetCurSel());
}

// 텍스쳐 매핑 해제
void Dialog_CRenderer::OnBnClickedRDiffusereset()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}
	pComp->SetSpriteList(L"None.png");
}

void Dialog_CRenderer::OnBnClickedRNormalreset()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}
	pComp->SetNormalMap(L"");
}

void Dialog_CRenderer::OnBnClickedREnvireset()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}
	pComp->SetEnviromentMap(L"");
}

// 환경맵 타입
void Dialog_CRenderer::OnCbnSelchangeREnvitype()
{
	///
}

// 컬러 설정
void Dialog_CRenderer::OnEnChangeRColorx()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (m_dontUpdate || 
		pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		return;
	}

	try
	{
		D3DXVECTOR4 color;
		m_ColorX.GetWindowTextW(m_tempString);
		color.x = std::stof(m_tempString.GetBuffer());
		m_ColorY.GetWindowTextW(m_tempString);
		color.y = std::stof(m_tempString.GetBuffer());
		m_ColorZ.GetWindowTextW(m_tempString);
		color.z = std::stof(m_tempString.GetBuffer());
		m_ColorW.GetWindowTextW(m_tempString);
		color.w = std::stof(m_tempString.GetBuffer());
		pComp->SetColor(DxManager::GetDContext(), color);
	}
	catch (...)
	{
	}
}

void Dialog_CRenderer::OnEnChangeRColory()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (m_dontUpdate ||
		pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		return;
	}

	try
	{
		D3DXVECTOR4 color;
		m_ColorX.GetWindowTextW(m_tempString);
		color.x = std::stof(m_tempString.GetBuffer());
		m_ColorY.GetWindowTextW(m_tempString);
		color.y = std::stof(m_tempString.GetBuffer());
		m_ColorZ.GetWindowTextW(m_tempString);
		color.z = std::stof(m_tempString.GetBuffer());
		m_ColorW.GetWindowTextW(m_tempString);
		color.w = std::stof(m_tempString.GetBuffer());
		pComp->SetColor(DxManager::GetDContext(), color);
	}
	catch (...)
	{
	}
}

void Dialog_CRenderer::OnEnChangeRColorz()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (m_dontUpdate || 
		pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		return;
	}

	try
	{
		D3DXVECTOR4 color;
		m_ColorX.GetWindowTextW(m_tempString);
		color.x = std::stof(m_tempString.GetBuffer());
		m_ColorY.GetWindowTextW(m_tempString);
		color.y = std::stof(m_tempString.GetBuffer());
		m_ColorZ.GetWindowTextW(m_tempString);
		color.z = std::stof(m_tempString.GetBuffer());
		m_ColorW.GetWindowTextW(m_tempString);
		color.w = std::stof(m_tempString.GetBuffer());
		pComp->SetColor(DxManager::GetDContext(), color);
	}
	catch (...)
	{
	}
}

void Dialog_CRenderer::OnEnChangeRColorw()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (m_dontUpdate || 
		pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		return;
	}

	try
	{
		D3DXVECTOR4 color;
		m_ColorX.GetWindowTextW(m_tempString);
		color.x = std::stof(m_tempString.GetBuffer());
		m_ColorY.GetWindowTextW(m_tempString);
		color.y = std::stof(m_tempString.GetBuffer());
		m_ColorZ.GetWindowTextW(m_tempString);
		color.z = std::stof(m_tempString.GetBuffer());
		m_ColorW.GetWindowTextW(m_tempString);
		color.w = std::stof(m_tempString.GetBuffer());
		pComp->SetColor(DxManager::GetDContext(), color);
	}
	catch (...)
	{
	}
}

// 높이맵 설정
void Dialog_CRenderer::OnBnClickedREnviapply2()
{
	static auto pHeightEvent = [](void* Dia_Renderer, void*) {
		auto pCRender = (Dialog_CRenderer*)Dia_Renderer;
		pCRender->ApplyHeightMap();
	};
	ObjectManager::PostFrameEvent.emplace(pHeightEvent, this, nullptr);
}

void Dialog_CRenderer::ApplyHeightMap() noexcept
{
	int index = m_TextureList.GetCurSel();
	if (index == LB_ERR)
	{
		MessageBox(L"텍스쳐를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pHegiht = (HeightMap*)Dialog_Inspector::Instance->SelectComponent;
	if (pHegiht == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pHegiht->m_comptType != EComponent::Renderer ||
		pHegiht->m_eRenderType != ERenderType::HeightMap)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}

	// 높이맵 적용
	auto pTexItem = (TexItem*)m_TextureList.GetItemData(index);

	try
	{
		m_HeightCellSize.GetWindowTextW(m_tempString);
		float cellSize = std::stof(m_tempString.GetBuffer());
		m_OffsetX.GetWindowTextW(m_tempString);
		float offsetX = std::stof(m_tempString.GetBuffer());
		m_OffsetY.GetWindowTextW(m_tempString);
		float offsetY = std::stof(m_tempString.GetBuffer());
		pHegiht->CreateHeightMap(DxManager::GetDContext(), pTexItem->Name, cellSize, offsetX, offsetY);
	}
	catch (...)
	{
		MessageBox(L"잘못된 입력값이 있습니다.", L"삐빅-");
		return;
	}
}



// 높이맵 해제
void Dialog_CRenderer::OnBnClickedREnvireset2()
{
	auto pHegiht = (HeightMap*)Dialog_Inspector::Instance->SelectComponent;
	if (pHegiht == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pHegiht->m_comptType != EComponent::Renderer ||
		pHegiht->m_eRenderType != ERenderType::HeightMap)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}

	try
	{
		m_HeightCellSize.GetWindowTextW(m_tempString);
		float cellSize = std::stof(m_tempString.GetBuffer());
		m_OffsetX.GetWindowTextW(m_tempString);
		float offsetX = std::stof(m_tempString.GetBuffer());
		m_OffsetY.GetWindowTextW(m_tempString);
		float offsetY = std::stof(m_tempString.GetBuffer());
		pHegiht->Create((WORD)cellSize, (WORD)cellSize, 1, offsetX, offsetY);
	}
	catch (...)
	{
		MessageBox(L"잘못된 입력값이 있습니다.", L"삐빅-");
		return;
	}
}

// 스프라이트 생성
void Dialog_CRenderer::OnBnClickedRDiffuseapply2()
{
	int index = m_TextureList.GetCurSel();
	if (index == LB_ERR)
	{
		MessageBox(L"텍스쳐를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (pComp == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	if (pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.\n(Not Renderer)", L"삐빅-");
		return;
	}
	// 텍스쳐 적용
	auto pTexItem = (TexItem*)m_TextureList.GetItemData(index);
	if (pTexItem->eTexType == ETextureType::Sprite)
	{
		MessageBox(L"이미 Sprite 입니다.", L"삐빅-");
		return;
	}

	try
	{
		m_SpriteFrame.GetWindowTextW(m_tempString);
		float frame = std::stof(m_tempString.GetBuffer());
		m_SpriteCount.GetWindowTextW(m_tempString);
		float count = std::stof(m_tempString.GetBuffer());

		m_OffsetX.GetWindowTextW(m_tempString);
		float offsetX = std::stof(m_tempString.GetBuffer());
		m_OffsetY.GetWindowTextW(m_tempString);
		float offsetY = std::stof(m_tempString.GetBuffer());

		m_StartOffsetX.GetWindowTextW(m_tempString);
		float width = std::stof(m_tempString.GetBuffer());
		m_StartOffsetY.GetWindowTextW(m_tempString);
		float height = std::stof(m_tempString.GetBuffer());

		TexItem texItem;
		texItem.Name = pTexItem->Name + L" (sprite)";

		auto& sprite = ObjectManager::Get().GetSpriteList()[texItem.Name];
		int spriteNum = 1;
		while (!sprite.empty())
		{
			texItem.Name = pTexItem->Name + L" (sprite)" + std::to_wstring(++spriteNum);
			sprite = ObjectManager::Get().GetSpriteList()[texItem.Name];
		}
		// script 출력
		std::wofstream spriteStream(L"../../data/script/sprite.txt", std::ios::app);
		//if(spriteStream.is_open())
		filesystem::path spriteName = pTexItem->Name;
		spriteStream << endl << spriteName.filename().c_str() << L" " << pTexItem->Name << endl;

		float curOffsetX = 0.0f;
		float curOffsetY = 0.0f;
		for (int i = 0; i < count; ++i)
		{
			sprite.emplace_back(pTexItem->pTexture, frame, 
				D3DXVECTOR4(curOffsetX, curOffsetY, width, height));
			// 파일 출력
			D3DXVECTOR4 posVector = { curOffsetX * sprite.back().m_pTexture->GetTexWidth(),
									  curOffsetY * sprite.back().m_pTexture->GetTexHeight(),
										width * sprite.back().m_pTexture->GetTexWidth(), 
										height * sprite.back().m_pTexture->GetTexHeight() };
			spriteStream << frame << L" " << posVector.x << L" " << posVector.y << L" " << posVector.z << L" " << posVector.w << endl;
			//spriteStream << frame << L" " << curOffsetX << L" " << curOffsetY << L" " << width << L" " << height << endl;

			// 오프셋 증감
			curOffsetX += offsetX;
			curOffsetY += offsetY;
			if (curOffsetX >= 1.0f)
			{
				curOffsetX -= 1.0f;
				curOffsetY += height;
			}
			if (curOffsetY >= 1.0f)
			{
				curOffsetY -= 1.0f;
				curOffsetX += width;
			}
		}
		spriteStream.close();

		texItem.pTexture = (Texture*)&sprite;
		texItem.eTexType = ETextureType::Sprite;
		m_TexItemMap[ETextureType::Sprite].emplace_back(std::move(texItem));
		UpdateTextureList();
	}
	catch (...)
	{
		MessageBox(L"잘못된 입력값이 있습니다.", L"삐빅-");
		return;
	}
}

// 라이트 비율 설정
void Dialog_CRenderer::OnBnClickedRLightapply()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (m_dontUpdate ||
		pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.", L"삐빅-");
		return;
	}

	try
	{
		m_LightRate.GetWindowTextW(m_tempString);
		pComp->SetLightRate(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
		MessageBox(L"잘못된 입력값이 있습니다.", L"삐빅-");
		return;
	}
}

// 쉐도우 비율 설정
void Dialog_CRenderer::OnBnClickedRShadowapply()
{
	auto pComp = (Renderer*)Dialog_Inspector::Instance->SelectComponent;
	if (m_dontUpdate ||
		pComp == nullptr ||
		pComp->m_comptType != EComponent::Renderer)
	{
		MessageBox(L"잘못된 접근 입니다.", L"삐빅-");
		return;
	}

	try
	{
		m_ShadowRate.GetWindowTextW(m_tempString);
		pComp->SetShadowRate(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
		MessageBox(L"잘못된 입력값이 있습니다.", L"삐빅-");
		return;
	}
}
