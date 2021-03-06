
// EffectTool.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "EffectTool.h"
#include "MainFrm.h"

#include "EffectToolDoc.h"
#include "EffectToolView.h"
#include "Dialog_Preset.h"
#include "Dialog_CRenderer.h"
#include "Dialog_Hierarchy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CoreMFC CEffectToolApp::CoreLib;

MaxImporter* CoreMFC::pParser;

// CEffectToolApp

BEGIN_MESSAGE_MAP(CEffectToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEffectToolApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEffectToolApp 생성

CEffectToolApp::CEffectToolApp()
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EffectTool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CEffectToolApp 개체입니다.

CEffectToolApp theApp;


// CEffectToolApp 초기화

BOOL CEffectToolApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEffectToolDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CEffectToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();



	// ====================================== 초기화 부분 =====================================================
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CEffectToolView* pGameView = (CEffectToolView*)pMainFrame->GetActiveView();
	Window::m_hWnd = pGameView->m_hWnd;

	CRect rect;
	pGameView->GetClientRect(&rect);
	Window::Instance->SetClientRect(rect.left, rect.top, rect.right, rect.bottom);

	CoreLib.GameRun();

	return TRUE;
}

int CEffectToolApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEffectToolApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CEffectToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEffectToolApp 사용자 지정 로드/저장 방법

void CEffectToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CEffectToolApp::LoadCustomState()
{
}

void CEffectToolApp::SaveCustomState()
{
}

// CEffectToolApp 메시지 처리기




bool CoreMFC::Init() noexcept
{
	// 폰트 설정
	WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 50, L"Yu Gothic");
	WriteManager::Get().SetFontSizeAlign(20, EAlign::Near, EAlign::Near);

	// 프리셋 리스트 초기화(쓰레드)
	Dialog_CRenderer::Instance->LoadSpriteList();
	Dialog_CRenderer::Instance->InitTextureList();

	// 기본 컴포넌트 등록
	Dialog_Inspector::Instance->AddCompPreset(new RPlane(L"Plane", L"None.png"));
	Dialog_Inspector::Instance->AddCompPreset(new RCube(L"Cube", L"None.png"));
	Dialog_Inspector::Instance->AddCompPreset(new RSphere(20, L"Sphere", L"None.png"));
	Dialog_Inspector::Instance->AddCompPreset(new RSphere(10, L"RowSphere", L"None.png"));
	auto pCollider = new Collider(1);
	pCollider->m_myName = L"ColliderSphere";
	Dialog_Inspector::Instance->AddCompPreset(pCollider);
	pCollider = new ColliderAABB(3.0f, -Vector3::One, Vector3::One);
	pCollider->m_myName = L"ColliderAABB";
	Dialog_Inspector::Instance->AddCompPreset(pCollider);
	pCollider = new ColliderOBB(3.0f, -Vector3::One, Vector3::One);
	pCollider->m_myName = L"ColliderOBB";
	Dialog_Inspector::Instance->AddCompPreset(pCollider);
	Dialog_Inspector::Instance->AddCompPreset(new CTransformer());
	Dialog_Inspector::Instance->AddCompPreset(new CEventTimer(100.0f));
	Dialog_Inspector::Instance->AddCompPreset(new ParticleSystem(L"ParticleSystem", new Particle(), L"UI/cat.png"));
	//Dialog_Inspector::Instance->AddCompPreset(new RLine(L"Line"));
	ObjectManager::Get().SetProtoComponent(new RLine(L"Line"));

	// 기본 객체 등록
	auto pObject = new GameObject(L"GameObject", EObjType::Object);
	ObjectManager::Get().SetProtoObject(pObject);
	Dialog_Preset::Instance->m_ListBox.AddString(pObject->m_myName.data());

	auto pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Plane");
	pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
	pObject = new GameObject(L"Plane", { pRenderer, pCollider }, EObjType::Object);
	ObjectManager::Get().SetProtoObject(pObject);
	Dialog_Preset::Instance->m_ListBox.AddString(pObject->m_myName.data());

	pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Cube");
	pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
	pObject = new GameObject(L"Cube", { pRenderer, pCollider }, EObjType::Object);
	ObjectManager::Get().SetProtoObject(pObject);
	Dialog_Preset::Instance->m_ListBox.AddString(pObject->m_myName.data());

	pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Sphere");
	pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderSphere");
	pObject = new GameObject(L"Sphere", { pRenderer, pCollider }, EObjType::Object);
	ObjectManager::Get().SetProtoObject(pObject);
	Dialog_Preset::Instance->m_ListBox.AddString(pObject->m_myName.data());

	// UI
	pObject = new GameObject(L"UI", new PlaneUI(L"PlaneUI", L"Silver.png"));
	pObject->m_objType = EObjType::UI;
	ObjectManager::Get().SetProtoObject(pObject);

	// UI 생성
	pObject = ObjectManager::Get().TakeObject(L"UI");
	pObject->SetScale(Vector3::One * 0.5f);
	pObject->SetPosition(-4.0f, -3.0f, 0.0f);

	// 월드 중심축
	RLine*		pSelectLines[3];
	pSelectLines[0] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
	pSelectLines[1] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
	pSelectLines[2] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
	pSelectLines[0]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Left * 1000, Vector3::Right * 1000, Color::Red, true);
	pSelectLines[1]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Up * 1000, Vector3::Down * 1000, Color::Green, true);
	pSelectLines[2]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Forward * 1000, Vector3::Backward * 1000, Color::Blue, true);
	ObjectManager::Get().PushObject(new GameObject(L"Center", { pSelectLines[0], pSelectLines[1], pSelectLines[2] }));

	// 스카이박스
	m_pSkyBox = new GameObject(L"SkyBox", { new SkySphere(20, L"SkySphere", L"CubeMap/grassenvmap1024.dds"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	m_pSkyBox->SetScale(Vector3::One * 100);
	m_pSkyBox->isGlobal(true);

	// 라이트
	auto pTrans = new CTransformer(Vector3::Up * 150.0f, Quaternion::Up * PI * 0.35f, Vector3::One);
	pTrans->TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
		pParent->SetTransform(*pTrans);
		pParent->Translate({ cosf(0.5f * accTime) * 200.0f, 0.0f, sinf(0.5f * accTime) * 200.0f });
		pParent->SetFocus(Vector3::Zero);
		return; spf; accTime; pTrans;
	};
	ObjectManager::Get().Lights.front()->AddComponent({ pTrans });
	// 라이트 자식
	auto pShpere = (Renderer*)ObjectManager::Get().TakeComponent(L"Sphere");
	pShpere->SetShaderLayout("VS_Basic", "PS_Basic");
	pObject = new GameObject(L"Sun", pShpere);
	pObject->SetScale(Vector3::One * 7);
	//pObject->m_objType = EObjType::Dummy;
	pObject->SetParent(ObjectManager::Get().Lights.front());

	// 프러스텀 박스
	m_pFrustumBox = new GameObject(L"Frustum", new FrustumBox(L"FrustumBox", EComponent::Renderer, L"None.png"));
	m_pFrustumBox->SetParent(ObjectManager::Cameras[ECamera::Main]);

	// 높이 맵
	//pCollider = new ColliderOBB({ -300.0f, -53.0f, -300.0f }, { 300.0f, 53.0f, 300.0f });
	m_pHeightMap = new HeightMap(L"HeightMap", EComponent::Renderer, L"mounds.jpg");
	auto pMap = new GameObject(L"HeightMap", { m_pHeightMap }, EObjType::Map);
	//pHeightMap->m_pPhysics->m_mass = 100.0f;
	//pHeightMap->SetGravityScale(0.0f);
	//pHeightMap->usePhysics(false);
	m_pHeightMap->CreateHeightMap(DxManager::GetDContext(), L"HeightMap/HEIGHT_MOUNDS.bmp", 10, 1.0f);
	m_pHeightMap->SetNormalMap(L"NormalMap/wall_NM_height.dds");
	m_pHeightMap->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Fresnel);
	pMap->SetScale(0.6f, 0.8f, 0.6f);
	pMap->Translate(Vector3::Down * 100.0f);
	ObjectManager::Get().PushObject(pMap);

	// 객체 좌표축 선
	pSelectLines[0] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
	pSelectLines[1] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
	pSelectLines[2] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
	pSelectLines[0]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Left * 100, Vector3::Right * 100, Color::Red);
	pSelectLines[1]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Up * 100, Vector3::Down * 100, Color::Green);
	pSelectLines[2]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Forward * 100, Vector3::Backward * 100, Color::Blue);
	m_pSelectLine = new GameObject(L"SelectLine", { pSelectLines[0], pSelectLines[1], pSelectLines[2] });
	m_pSelectLine->isStatic(true);

	// 파티클
	auto pParticle = new ParticleSystem(L"ParticleSystem", new Particle(), L"UI/cat.png");
	pParticle->m_spawnInterval = 0.013f;
	pParticle->m_maxParticleCount = 9999;
	pParticle->m_minLifeCycle = 7.0f;
	pParticle->m_maxLifeCycle = 7.0f;
	pParticle->m_minInitPosition = {-200.0f, 0.0f, -200.0f};
	pParticle->m_maxInitPosition = { 200.0f, 0.0f, 200.0f };
	pParticle->m_minDirection = { -1.0f, -1.0f, -1.0f };
	pParticle->m_maxDirection = { 1.0f, -1.0f, 1.0f};
	pParticle->m_minMaxMoveSpeed = 20.0;
	pParticle->m_maxMaxMoveSpeed = 20.0;
	pParticle->m_minAccMoveSpeed = 1.0f;
	pParticle->m_maxAccMoveSpeed = 2.0f;
	pParticle->m_minCurMoveSpeed = 3.0f;
	pParticle->m_maxCurMoveSpeed = 10.0f;
	pParticle->m_minDirAngle = Quaternion::Zero;
	pParticle->m_maxDirAngle = Quaternion::Zero;
	pParticle->m_minRotateSpeed = 0.0f;
	pParticle->m_maxRotateSpeed = 0.0f;
	pParticle->m_minColor = { 0.5f, 0.5f, 0.5f, 0.5f };
	pParticle->m_maxColor = Color::White * 2;
	pParticle->m_minGravityPower = 5.0f;
	pParticle->m_maxGravityPower = 10.0f;
	//Dialog_Inspector::Instance->AddCompPreset(pParticle);

	auto pParticler = new GameObject(L"ParticleSystem", { pParticle/*, new CTransformer(Vector3::Zero, Quaternion::Left * 0.25f, Vector3::Zero)*/ }, EObjType::Effect);
	pParticler->Translate(Vector3::Up * 200);
	ObjectManager::Get().PushObject(pParticler);

	// 하이어라키 등록
	ObjectManager::Get().Cameras[ECamera::Main]->isGlobal(true);
	ObjectManager::Get().Lights.front()->isGlobal(true);
	m_pSkyBox->isGlobal(true);
	m_pFrustumBox->isGlobal(true);
	Dialog_Hierarchy::Instance->AddTreeItem(ObjectManager::Get().Cameras[ECamera::Main]);
	Dialog_Hierarchy::Instance->AddTreeItem(ObjectManager::Get().Cameras[ECamera::MiniMap]);
	Dialog_Hierarchy::Instance->AddTreeItem(ObjectManager::Get().Lights.front());
	Dialog_Hierarchy::Instance->AddTreeItem(m_pSkyBox);
	Dialog_Hierarchy::Instance->AddTreeItem(pMap);
	Dialog_Hierarchy::Instance->AddTreeItem(pParticler);

	// 노말 환경
	pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Cube");
	//pRenderer->SetNormalMap(L"NormalMap/tileADOT3.jpg");
	//pRenderer->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Refraction);
	//pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
	pObject = new GameObject(L"Object1", { pRenderer/*, pCollider*/, new CTransformer(Vector3::Zero, Quaternion::Right * 2.0f, Vector3::Zero) }, EObjType::Object);
	pObject->Translate(Vector3::Backward * 30.0f + Vector3::Up * 30.0f);
	pObject->SetScale(Vector3::One * 10.0f);
	ObjectManager::Get().PushObject(pObject);
	Dialog_Hierarchy::Instance->AddTreeItem(pObject);

	pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Cube");
	//pRenderer->SetNormalMap(L"NormalMap/wall_NM_height.dds");
	//pRenderer->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Fresnel);
	//pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
	pObject = new GameObject(L"Object2", { pRenderer /*pCollider*/ /*new CTransformer(Vector3::Zero, Quaternion::Up, Vector3::Zero)*/ }, EObjType::Object);
	pObject->Translate(Vector3::Backward * 30.0f + Vector3::Left * 30.0f + Vector3::Up * 30);
	pObject->SetScale(Vector3::One * 7.0f);
	ObjectManager::Get().PushObject(pObject);
	Dialog_Hierarchy::Instance->AddTreeItem(pObject);

	pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Sphere");
	//pRenderer->SetNormalMap(L"NormalMap/Earth_NormalMap.dds");
	//pRenderer->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Basic);
	//pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderSphere");
	pObject = new GameObject(L"Object3", { pRenderer/*, pCollider, new CTransformer(Vector3::Up, Quaternion::Base, Vector3::One * 0.5f)*/ }, EObjType::Object);
	pObject->Translate(Vector3::Backward * 30.0f + Vector3::Right * 30.0f + Vector3::Up * 30);
	pObject->SetScale(Vector3::One * 10.0f);
	ObjectManager::Get().PushObject(pObject);
	Dialog_Hierarchy::Instance->AddTreeItem(pObject);


	//SoundManager::Get().Load("BGM/Mischievous Masquerade", false, FMOD_LOOP_NORMAL);
	//SoundManager::Get().Load("BGM/Sandbag Mini Game", false, FMOD_LOOP_NORMAL);
	//SoundManager::Get().Load("BGM/Brawl Breaks", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("BGM/PLAY ROUGH", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().SetBGM("BGM/PLAY ROUGH");
	
	SoundManager::Get().Load("shout1.mp3");
	return true;
}

bool CoreMFC::Frame()	noexcept
{
	//if (Input::GetKeyState('V') == EKeyState::DOWN)
	//{
	//	SoundManager::Get().PlayQueue("shout1.mp3", Vector3::Zero, 800.0f);
	//}

	// 마우스 피킹
	if ((Input::GetKeyState(VK_CONTROL) == EKeyState::HOLD  ||
		 Input::GetKeyState(VK_SHIFT)   == EKeyState::HOLD) &&
		 Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
	{
		for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Object))
		{
			MousePicking(iter);
		}
	}
	// 선택 객체 이동
	if (Input::GetKeyState(VK_CONTROL) == EKeyState::HOLD)
	{
		ObjectMoveCheck(Timer::SPF);
	}
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		switch (iter->m_eCollider)
		{
		case ECollider::AABB:
		{
			iter->m_mapHeight = ((ColliderAABB*)iter)->GetLength().y * 0.5f + m_pHeightMap->GetMapHeight(iter->m_pParent->GetPosition());
		}	break;
		case ECollider::OBB:
		{
			iter->m_mapHeight = ((ColliderOBB*)iter)->GetExtents().y + m_pHeightMap->GetMapHeight(iter->m_pParent->GetPosition());
		}	break;
		case ECollider::Sphere:
		{
			iter->m_mapHeight = ((Collider*)iter)->GetWorldRadius() * 0.5f + m_pHeightMap->GetMapHeight(iter->m_pParent->GetPosition());
		}	break;
		default:
			break;
		}
	}

	// 객체 행동
	DxManager::Get().Frame();
	m_pSkyBox->Frame(Timer::SPF, Timer::AccumulateTime);
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	m_pFrustumBox->Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

bool CoreMFC::Render()	noexcept
{
	// 드로우
	DxManager::Get().Render();
	m_pSkyBox->Render(DxManager::Get().GetDContext());
	ObjectManager::Get().Render(DxManager::Get().GetDContext());
	// 바운딩 박스 표시
	if (Input::isDebug)
	{
		static GameObject* pBox = nullptr;
		static GameObject* pSphere = nullptr;
		if (pBox == nullptr || pSphere == nullptr)
		{
			pBox = new GameObject(L"DebugBox", ObjectManager::Get().TakeComponent(L"Cube"));
			pBox->isGlobal(true);
			pSphere = new GameObject(L"DebugSphere", ObjectManager::Get().TakeComponent(L"RowSphere"));
			pSphere->isGlobal(true);
		}

		DxManager::Get().SetRasterizerState(ERasterS::Wireframe);
		for (auto& iter : ObjectManager::Get().GetColliderList())
		{
			switch (iter->m_eCollider)
			{
			case ECollider::AABB:
			{
				pBox->SetPosition(iter->GetCenter());
				pBox->SetRotation(Quaternion::Base);
				pBox->SetScale(((ColliderAABB*)iter)->GetLength() * 0.5f);
				pBox->Frame(0.0f, 0.0f);
				pBox->Render(DxManager::GetDContext());
			}	break;
			case ECollider::OBB:
			{
				pBox->SetPosition(iter->GetCenter());
				pBox->SetRotation(iter->m_pParent->GetRotation());
				pBox->SetScale(((ColliderOBB*)iter)->GetExtents());
				pBox->Frame(0.0f, 0.0f);
				pBox->Render(DxManager::GetDContext());
			}	break;
			case ECollider::Sphere:
			{
			}	break;
			}
			pSphere->SetPosition(iter->GetCenter());
			pSphere->SetRotation(Quaternion::Base);
			pSphere->SetScale(iter->GetWorldRadius() * Vector3::One);
			pSphere->Frame(0.0f, 0.0f);
			pSphere->Render(DxManager::GetDContext());
		}
		DxManager::Get().SetRasterizerState(ERasterS::Current);
	}
	if (auto pObject = Dialog_Inspector::SelectObject;
		pObject != nullptr)
	{
		if (pObject->m_objType != EObjType::Camera)
		{
			// 객체 좌표축 출력
			m_pSelectLine->SetWorldMatrix(Dialog_Inspector::SelectObject->GetWorldMatrix());
			m_pSelectLine->Render(DxManager::Get().GetDContext());
			// 외곽선 출력
			if (Input::isDebug && pObject->m_objType != EObjType::Map)
			{
				// 외곽 랜더
				auto RendererList = (forward_list<Renderer*>*)pObject->GetComponentList(EComponent::Renderer);
				if (RendererList != nullptr)
				{
					DxManager::Get().SetDepthStencilState(EDepthS::D_Off_NoWrite);
					for (auto* pRenderer : *RendererList)
					{
						if (pRenderer != nullptr)
						{
							pRenderer->PrevRender(DxManager::Get().GetDContext());
							DxManager::GetDContext()->VSSetShader(DxManager::Get().m_VShaderList["VS_OutLine"], nullptr, 0);
							DxManager::GetDContext()->PSSetShader(DxManager::Get().m_PShaderList["PS_OutLine"], nullptr, 0);
							pRenderer->PostRender(DxManager::Get().GetDContext());
						}
					}
					// 내부 랜더
					DxManager::Get().SetDepthStencilState(EDepthS::D_Less_S_Always);
					DxManager::Get().SetSamplerState(1, ESamTextureS::Clamp, ESamFilterS::Linear);
					DxManager::Get().SetSamplerState(2, ESamTextureS::Border, ESamFilterS::CompLinearPoint, 0, D3D11_COMPARISON_LESS);

					DxManager::GetDContext()->PSSetShaderResources(4, 1, &DxManager::Get().m_RTDSViewShadow.m_pTexSRViews[0]);
					pObject->Render(DxManager::GetDContext());

					DxManager::Get().SetSamplerState(1, ESamTextureS::Current, ESamFilterS::Current);
					DxManager::Get().SetSamplerState(2, ESamTextureS::Current, ESamFilterS::Current);
					DxManager::Get().SetDepthStencilState(EDepthS::Current);
				}
			}
		}
	}
	m_pFrustumBox->Render(DxManager::GetDContext());
	SoundManager::Get().Render();

	//WriteManager::Get().SetFontSize(50);
	//WriteManager::Get().DrawTextW({ getClientRect().right * 0.05f , getClientRect().bottom * 0.98f, 400, 400 }, L"앙앙~ 테스트 Test ~ 123,,");
	return true;
}

bool CoreMFC::Release()	noexcept
{
	return true;
}


void CoreMFC::MousePicking(GameObject* pObject) noexcept
{
	//static std::pair<GameObject*, float> SelectObject;

	Raycast::SetMousePick(Input::GetCursor(), 1000.0f);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Renderer* pComp = nullptr;
	if (pObject->GetComponentList(EComponent::Renderer) != nullptr)
		pComp = (Renderer*)pObject->GetComponentList(EComponent::Renderer)->front();
	//else
		//pComp = pObject->m_pInstance;

	if (pComp != nullptr)
	{
		if (Raycast::Raycasting((Renderer*)pComp, nullptr, &pObject->GetWorldMatrix()))
		{
			GameObject* pParent = pObject;
			while (pObject->GetParent() != nullptr)
			{
				pParent = pObject->GetParent();
			}
			ErrorMessage(pParent->m_myName + L" : RayHit!");
			auto item = Dialog_Hierarchy::Instance->m_TreeList.GetRootItem();
			auto pItemObj = Dialog_Hierarchy::Instance->m_TreeList.GetItemData(item);
			while ((GameObject*)pItemObj != pParent)
			{
				item = Dialog_Hierarchy::Instance->m_TreeList.GetNextSiblingItem(item);
				pItemObj = Dialog_Hierarchy::Instance->m_TreeList.GetItemData(item);
			}

			Dialog_Hierarchy::Instance->m_TreeList.SelectItem(item);
			return;
		}
		else
		{
			//ErrorMessage(""s + to_string(Raycast::m_ray.x) + ", " + to_string(Raycast::m_ray.y) + ", " + to_string(Raycast::m_ray.z));
		}
	}

	//// 자식 순회(박스로 해야할듯..)
	//if (pObject->GetChildList() != nullptr)
	//{
	//	for (auto& pChild : *pObject->GetChildList())
	//	{
	//		//MousePicking(pChild);
	//	}
	//}
}

void CoreMFC::ObjectMoveCheck(const float& spf) noexcept
{
	//////////////////// 키보드 이동
	auto pObject = Dialog_Inspector::SelectObject;
	if (pObject == nullptr)
		return;
	if (Input::GetKeyState('F') == EKeyState::DOWN)
	{
		ObjectManager::Cameras[ECamera::Main]->SetPosition(pObject->GetWorldPosition());
		ObjectManager::Cameras[ECamera::Main]->Translate(ObjectManager::Cameras[ECamera::Main]->GetForward() * -(13.0f + D3DXVec3Length(&pObject->GetScale()) / 3));
	}
	if (Input::GetKeyState('R') == EKeyState::HOLD)
	{
		pObject->SetTransform({ Vector3::Zero, Quaternion::Zero, Vector3::One });
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		pObject->Translate(pObject->GetLeft() * 10.0f * spf);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		pObject->Translate(pObject->GetRight() * 10.0f *  spf);
	}
	if (Input::GetKeyState('E') == EKeyState::HOLD)
	{
		pObject->Translate(pObject->GetUp() * 10.0f * spf);
	}
	if (Input::GetKeyState('Q') == EKeyState::HOLD)
	{
		pObject->Translate(pObject->GetDown() * 10.0f * spf);
	}
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		pObject->Translate(pObject->GetForward() * 10.0f * spf);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		pObject->Translate(pObject->GetBackward() * 10.0f * spf);
	}
	////////////////////// 키보드 회전
	if (Input::Get().GetKeyState(VK_LEFT) == EKeyState::HOLD)
	{
		pObject->Rotate(Quaternion::Left * spf);
	}
	if (Input::Get().GetKeyState(VK_RIGHT) == EKeyState::HOLD)
	{
		pObject->Rotate(Quaternion::Right * spf);
	}
	if (Input::Get().GetKeyState(VK_UP) == EKeyState::HOLD)
	{
		pObject->Rotate(Quaternion::Down * spf);
	}
	if (Input::Get().GetKeyState(VK_DOWN) == EKeyState::HOLD)
	{
		pObject->Rotate(Quaternion::Up * spf);
	}
	/////////////////// 마우스
	pObject->Translate(ObjectManager::CurCamera->GetForward() * Input::GetWheelScroll() * 2.0f * spf);
	if (Input::GetKeyState(EMouseButton::Left) == EKeyState::HOLD)
	{
		pObject->Translate((-ObjectManager::CurCamera->GetLeft() * Input::GetMouseMovePos().x - ObjectManager::CurCamera->GetUp() * Input::GetMouseMovePos().y) * 0.1f);
	}
	if (Input::GetKeyState(EMouseButton::Right) == EKeyState::HOLD)
	{
		pObject->Rotate(0.0f, -Input::GetMouseMovePos().x * 0.004f, Input::GetMouseMovePos().y * 0.004f);
	}

	// 인스펙터 갱신
	Dialog_Inspector::Instance->UpdateInspector();
}