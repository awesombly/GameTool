#include "stdafx.h"
#include "EffectTool.h"
#include "afxdialogex.h"
#include "Dialog_CParticle.h"
#include "Dialog_Inspector.h"

Dialog_CParticle* Dialog_CParticle::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_CParticle, CFormView)

Dialog_CParticle::Dialog_CParticle()
	: CFormView(IDD_DIALOG_CParticle)
{
	Instance = this;
}

Dialog_CParticle::~Dialog_CParticle()
{}


void Dialog_CParticle::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_P_Interval, m_Interval);
	DDX_Control(pDX, IDC_P_MaxCount, m_MaxCount);
	DDX_Control(pDX, IDC_P_LifeMin, m_LifeMin);
	DDX_Control(pDX, IDC_P_LifeMax, m_LifeMax);
	DDX_Control(pDX, IDC_P_GravityMin, m_GravityMin);
	DDX_Control(pDX, IDC_P_GravityMax, m_GravityMax);
	DDX_Control(pDX, IDC_P_DirMinX, m_DirectionMinX);
	DDX_Control(pDX, IDC_P_DirMinY, m_DirectionMinY);
	DDX_Control(pDX, IDC_P_DirMinZ, m_DirectionMinZ);
	DDX_Control(pDX, IDC_P_DirMaxX, m_DirectionMaxX);
	DDX_Control(pDX, IDC_P_DirMaxY, m_DirectionMaxY);
	DDX_Control(pDX, IDC_P_DirMaxZ, m_DirectionMaxZ);
	DDX_Control(pDX, IDC_P_SpeedMin, m_StartSpeedMin);
	DDX_Control(pDX, IDC_P_SpeedMax, m_StartSpeedMax);
	DDX_Control(pDX, IDC_P_AccelSpeedMin, m_AccelSpeedMin);
	DDX_Control(pDX, IDC_P_AccelSpeedMax, m_AccelSpeedMax);
	DDX_Control(pDX, IDC_P_MaxSpeedMin, m_MaxSpeedMin);
	DDX_Control(pDX, IDC_P_MaxSpeedMax, m_MaxSpeedMax);
	DDX_Control(pDX, IDC_P_RotateMinX, m_RotateDirMinX);
	DDX_Control(pDX, IDC_P_RotateMinY, m_RotateDirMinY);
	DDX_Control(pDX, IDC_P_RotateMinZ, m_RotateDirMinZ);
	DDX_Control(pDX, IDC_P_RotateMaxX, m_RotateDirMaxX);
	DDX_Control(pDX, IDC_P_RotateMaxY, m_RotateDirMaxY);
	DDX_Control(pDX, IDC_P_RotateMaxZ, m_RotateDirMaxZ);
	DDX_Control(pDX, IDC_P_RotateSpeedMax, m_RotateSpeedMin);
	DDX_Control(pDX, IDC_P_RotateSpeedMin, m_RotateSpeedMax);
	DDX_Control(pDX, IDC_P_ColorMinX, m_ColorMinX);
	DDX_Control(pDX, IDC_P_ColorMinY, m_ColorMinY);
	DDX_Control(pDX, IDC_P_ColorMinZ, m_ColorMinZ);
	DDX_Control(pDX, IDC_P_ColorMinW, m_ColorMinW);
	DDX_Control(pDX, IDC_P_ColorMaxX, m_ColorMaxX);
	DDX_Control(pDX, IDC_P_ColorMaxY, m_ColorMaxY);
	DDX_Control(pDX, IDC_P_ColorMaxZ, m_ColorMaxZ);
	DDX_Control(pDX, IDC_P_ColorMaxW, m_ColorMaxW);
	DDX_Control(pDX, IDC_P_isBillBoard, m_isBillBoard);
	DDX_Control(pDX, IDC_P_isRepeat, m_isRepeat);
	DDX_Control(pDX, IDC_P_isEnable, m_isEnable);
	DDX_Control(pDX, IDC_P_PositionMinX, m_PositionMinX);
	DDX_Control(pDX, IDC_P_PositionMinY, m_PositionMinY);
	DDX_Control(pDX, IDC_P_PositionMinZ, m_PositionMinZ);
	DDX_Control(pDX, IDC_P_PositionMaxX, m_PositionMaxX);
	DDX_Control(pDX, IDC_P_PositionMaxY, m_PositionMaxY);
	DDX_Control(pDX, IDC_P_PositionMaxZ, m_PositionMaxZ);
	DDX_Control(pDX, IDC_P_ScaleMinX, m_ScaleMinX);
	DDX_Control(pDX, IDC_P_ScaleMinY, m_ScaleMinY);
	DDX_Control(pDX, IDC_P_ScaleMinZ, m_ScaleMinZ);
	DDX_Control(pDX, IDC_P_ScaleMaxX, m_ScaleMaxX);
	DDX_Control(pDX, IDC_P_ScaleMaxY, m_ScaleMaxY);
	DDX_Control(pDX, IDC_P_ScaleMaxZ, m_ScaleMaxZ);
	DDX_Control(pDX, IDC_P_RotationMinX, m_RotationMinX);
	DDX_Control(pDX, IDC_P_RotationMinY, m_RotationMinY);
	DDX_Control(pDX, IDC_P_RotationMinZ, m_RotationMinZ);
	DDX_Control(pDX, IDC_P_RotationMaxX, m_RotationMaxX);
	DDX_Control(pDX, IDC_P_RotationMaxY, m_RotationMaxY);
	DDX_Control(pDX, IDC_P_RotationMaxZ, m_RotationMaxZ);
	DDX_Control(pDX, IDC_P_isFollow, m_isScreen);
	DDX_Control(pDX, IDC_P_GravityDirX, m_GravityDirX);
	DDX_Control(pDX, IDC_P_GravityDirY, m_GravityDirY);
	DDX_Control(pDX, IDC_P_GravityDirZ, m_GravityDirZ);
	DDX_Control(pDX, IDC_P_ScalePerLifeMinX, m_ScalePerLifeMinX);
	DDX_Control(pDX, IDC_P_ScalePerLifeMinY, m_ScalePerLifeMinY);
	DDX_Control(pDX, IDC_P_ScalePerLifeMinZ, m_ScalePerLifeMinZ);
	DDX_Control(pDX, IDC_P_ScalePerLifeMaxX, m_ScalePerLifeMaxX);
	DDX_Control(pDX, IDC_P_ScalePerLifeMaxY, m_ScalePerLifeMaxY);
	DDX_Control(pDX, IDC_P_ScalePerLifeMaxZ, m_ScalePerLifeMaxZ);
	DDX_Control(pDX, IDC_P_isScalarScale, m_isScalarScale);
	DDX_Control(pDX, IDC_P_InitCountMin, m_MinInitCount);
	DDX_Control(pDX, IDC_P_InitCountMax, m_MaxInitCount);
	DDX_Control(pDX, IDC_P_LifeTime, m_LifeTime);
	DDX_Control(pDX, IDC_P_isRealFollow, m_isFollow);
}

Dialog_CParticle* Dialog_CParticle::CreateOne(CWnd* pParent)
{
	Dialog_CParticle* pForm = new Dialog_CParticle();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}


BEGIN_MESSAGE_MAP(Dialog_CParticle, CFormView)

	ON_BN_CLICKED(IDC_P_Button_Reset, &Dialog_CParticle::OnBnClickedPButtonReset)
	ON_BN_CLICKED(IDC_P_Button_Update, &Dialog_CParticle::OnBnClickedPButtonUpdate)
	ON_BN_CLICKED(IDC_P_isFollow, &Dialog_CParticle::OnBnClickedPisfollow)
	ON_BN_CLICKED(IDC_P_isEnable, &Dialog_CParticle::OnBnClickedPisenable)
	ON_BN_CLICKED(IDC_P_isRepeat, &Dialog_CParticle::OnBnClickedPisrepeat)
	ON_BN_CLICKED(IDC_P_isBillBoard, &Dialog_CParticle::OnBnClickedPisbillboard)
	ON_BN_CLICKED(IDC_P_isScalarScale, &Dialog_CParticle::OnBnClickedPisscalarscale)
	ON_BN_CLICKED(IDC_P_isRealFollow, &Dialog_CParticle::OnBnClickedPisrealfollow)
END_MESSAGE_MAP()

// ============================ 정의 함수 =============================================
void Dialog_CParticle::UpdateEdit(const Component* pComp) noexcept
{
	if (pComp == nullptr) return;
	m_pParticle = (ParticleSystem*)pComp;
	
	m_isEnable.SetCheck(m_pParticle->isEnable());
	m_isScreen.SetCheck(m_pParticle->isScreen());
	m_isRepeat.SetCheck(m_pParticle->isRepeat());
	m_isBillBoard.SetCheck(m_pParticle->isBillBoard());
	m_isFollow.SetCheck(m_pParticle->isFollow());
	m_isScalarScale.SetCheck(m_pParticle->isScalarScale());
	// Scale이 단일값인지
	if (m_pParticle->isScalarScale())
	{
		m_ScaleMinY.EnableWindow(0);
		m_ScaleMinZ.EnableWindow(0);
		m_ScaleMaxY.EnableWindow(0);
		m_ScaleMaxZ.EnableWindow(0);
		m_ScalePerLifeMinY.EnableWindow(0);
		m_ScalePerLifeMinZ.EnableWindow(0);
		m_ScalePerLifeMaxY.EnableWindow(0);
		m_ScalePerLifeMaxZ.EnableWindow(0);
	}
	else
	{
		m_ScaleMinY.EnableWindow(1);
		m_ScaleMinZ.EnableWindow(1);
		m_ScaleMaxY.EnableWindow(1);
		m_ScaleMaxZ.EnableWindow(1);
		m_ScalePerLifeMinY.EnableWindow(1);
		m_ScalePerLifeMinZ.EnableWindow(1);
		m_ScalePerLifeMaxY.EnableWindow(1);
		m_ScalePerLifeMaxZ.EnableWindow(1);
	}
	///
	m_LifeTime.SetWindowTextW(std::to_wstring(m_pParticle->m_lifeTime).c_str());
	m_Interval.SetWindowTextW(std::to_wstring(m_pParticle->m_spawnInterval).c_str());
	m_MaxCount.SetWindowTextW(std::to_wstring(m_pParticle->m_maxParticleCount).c_str());
	m_MinInitCount.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitCount).c_str());
	m_MaxInitCount.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitCount).c_str());
	m_LifeMin.SetWindowTextW(std::to_wstring(m_pParticle->m_minLifeCycle).c_str());
	m_LifeMax.SetWindowTextW(std::to_wstring(m_pParticle->m_maxLifeCycle).c_str());
	m_GravityMin.SetWindowTextW(std::to_wstring(m_pParticle->m_minGravityPower).c_str());
	m_GravityMax.SetWindowTextW(std::to_wstring(m_pParticle->m_maxGravityPower).c_str());
	///
	m_GravityDirX.SetWindowTextW(std::to_wstring(m_pParticle->m_dirGravity.x).c_str());
	m_GravityDirY.SetWindowTextW(std::to_wstring(m_pParticle->m_dirGravity.y).c_str());
	m_GravityDirZ.SetWindowTextW(std::to_wstring(m_pParticle->m_dirGravity.z).c_str());
	///
	m_PositionMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitPosition.x).c_str());
	m_PositionMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitPosition.y).c_str());
	m_PositionMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitPosition.z).c_str());
	m_PositionMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitPosition.x).c_str());
	m_PositionMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitPosition.y).c_str());
	m_PositionMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitPosition.z).c_str());
	///
	m_ScaleMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitScale.x).c_str());
	m_ScaleMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitScale.y).c_str());
	m_ScaleMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitScale.z).c_str());
	m_ScaleMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitScale.x).c_str());
	m_ScaleMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitScale.y).c_str());
	m_ScaleMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitScale.z).c_str());
	///
	m_RotationMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitRotation.x).c_str());
	m_RotationMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitRotation.y).c_str());
	m_RotationMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minInitRotation.z).c_str());
	m_RotationMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitRotation.x).c_str());
	m_RotationMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitRotation.y).c_str());
	m_RotationMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxInitRotation.z).c_str());
	///
	m_ScalePerLifeMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minScalePerLife.x).c_str());
	m_ScalePerLifeMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minScalePerLife.y).c_str());
	m_ScalePerLifeMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minScalePerLife.z).c_str());
	m_ScalePerLifeMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxScalePerLife.x).c_str());
	m_ScalePerLifeMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxScalePerLife.y).c_str());
	m_ScalePerLifeMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxScalePerLife.z).c_str());
	///
	m_DirectionMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minDirection.x).c_str());
	m_DirectionMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minDirection.y).c_str());
	m_DirectionMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minDirection.z).c_str());
	m_DirectionMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxDirection.x).c_str());
	m_DirectionMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxDirection.y).c_str());
	m_DirectionMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxDirection.z).c_str());
	m_StartSpeedMin.SetWindowTextW(std::to_wstring(m_pParticle->m_minCurMoveSpeed).c_str());
	m_StartSpeedMax.SetWindowTextW(std::to_wstring(m_pParticle->m_maxCurMoveSpeed).c_str());
	m_AccelSpeedMin.SetWindowTextW(std::to_wstring(m_pParticle->m_minAccMoveSpeed).c_str());
	m_AccelSpeedMax.SetWindowTextW(std::to_wstring(m_pParticle->m_maxAccMoveSpeed).c_str());
	m_MaxSpeedMin.SetWindowTextW(std::to_wstring(m_pParticle->m_minMaxMoveSpeed).c_str());
	m_MaxSpeedMax.SetWindowTextW(std::to_wstring(m_pParticle->m_maxMaxMoveSpeed).c_str());
	///
	m_RotateDirMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minDirAngle.x).c_str());
	m_RotateDirMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minDirAngle.y).c_str());
	m_RotateDirMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minDirAngle.z).c_str());
	m_RotateDirMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxDirAngle.x).c_str());
	m_RotateDirMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxDirAngle.y).c_str());
	m_RotateDirMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxDirAngle.z).c_str());
	m_RotateSpeedMin.SetWindowTextW(std::to_wstring(m_pParticle->m_minRotateSpeed).c_str());
	m_RotateSpeedMax.SetWindowTextW(std::to_wstring(m_pParticle->m_maxRotateSpeed).c_str());
	///
	m_ColorMinX.SetWindowTextW(std::to_wstring(m_pParticle->m_minColor.x).c_str());
	m_ColorMinY.SetWindowTextW(std::to_wstring(m_pParticle->m_minColor.y).c_str());
	m_ColorMinZ.SetWindowTextW(std::to_wstring(m_pParticle->m_minColor.z).c_str());
	m_ColorMinW.SetWindowTextW(std::to_wstring(m_pParticle->m_minColor.w).c_str());
	m_ColorMaxX.SetWindowTextW(std::to_wstring(m_pParticle->m_maxColor.x).c_str());
	m_ColorMaxY.SetWindowTextW(std::to_wstring(m_pParticle->m_maxColor.y).c_str());
	m_ColorMaxZ.SetWindowTextW(std::to_wstring(m_pParticle->m_maxColor.z).c_str());
	m_ColorMaxW.SetWindowTextW(std::to_wstring(m_pParticle->m_maxColor.w).c_str());
}

// ============================ Dialog_CParticle 메시지 처리기 ===========================

void Dialog_CParticle::OnBnClickedPButtonUpdate()
{
	m_pParticle->isEnable(m_isEnable.GetCheck());
	m_pParticle->isScreen(m_isScreen.GetCheck());
	m_pParticle->isRepeat(m_isRepeat.GetCheck());
	m_pParticle->isBillBoard(m_isBillBoard.GetCheck());
	m_pParticle->isFollow(m_isFollow.GetCheck());
	try
	{
		m_LifeTime.GetWindowTextW(m_tempString);
		m_pParticle->m_lifeTime = std::stof(m_tempString.GetBuffer());
		// 간격, 최대값
		m_Interval.GetWindowTextW(m_tempString);
		m_pParticle->m_spawnInterval = std::stof(m_tempString.GetBuffer());
		m_MaxCount.GetWindowTextW(m_tempString);
		m_pParticle->m_maxParticleCount = std::stoi(m_tempString.GetBuffer());

		m_MinInitCount.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitCount = std::stoi(m_tempString.GetBuffer());

		m_MaxInitCount.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitCount = std::stoi(m_tempString.GetBuffer());

		m_LifeMin.GetWindowTextW(m_tempString);
		m_pParticle->m_minLifeCycle = std::stof(m_tempString.GetBuffer());
		m_LifeMax.GetWindowTextW(m_tempString);
		m_pParticle->m_maxLifeCycle = std::stof(m_tempString.GetBuffer());
		// 중력
		m_GravityMin.GetWindowTextW(m_tempString);
		m_pParticle->m_minGravityPower = std::stof(m_tempString.GetBuffer());
		m_GravityMax.GetWindowTextW(m_tempString);
		m_pParticle->m_maxGravityPower = std::stof(m_tempString.GetBuffer());

		m_GravityDirX.GetWindowTextW(m_tempString);
		m_pParticle->m_dirGravity.x = std::stof(m_tempString.GetBuffer());
		m_GravityDirY.GetWindowTextW(m_tempString);
		m_pParticle->m_dirGravity.y = std::stof(m_tempString.GetBuffer());
		m_GravityDirZ.GetWindowTextW(m_tempString);
		m_pParticle->m_dirGravity.z = std::stof(m_tempString.GetBuffer());
		// 초기 위치
		m_PositionMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitPosition.x = std::stof(m_tempString.GetBuffer());
		m_PositionMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitPosition.y = std::stof(m_tempString.GetBuffer());
		m_PositionMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitPosition.z = std::stof(m_tempString.GetBuffer());

		m_PositionMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitPosition.x = std::stof(m_tempString.GetBuffer());
		m_PositionMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitPosition.y = std::stof(m_tempString.GetBuffer());
		m_PositionMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitPosition.z = std::stof(m_tempString.GetBuffer());
		// 초기 크기
		m_ScaleMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitScale.x = std::stof(m_tempString.GetBuffer());
		m_ScaleMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitScale.y = std::stof(m_tempString.GetBuffer());
		m_ScaleMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitScale.z = std::stof(m_tempString.GetBuffer());

		m_ScaleMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitScale.x = std::stof(m_tempString.GetBuffer());
		m_ScaleMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitScale.y = std::stof(m_tempString.GetBuffer());
		m_ScaleMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitScale.z = std::stof(m_tempString.GetBuffer());
		// 초기 회전
		m_RotationMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitRotation.x = std::stof(m_tempString.GetBuffer());
		m_RotationMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitRotation.y = std::stof(m_tempString.GetBuffer());
		m_RotationMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minInitRotation.z = std::stof(m_tempString.GetBuffer());

		m_RotationMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitRotation.x = std::stof(m_tempString.GetBuffer());
		m_RotationMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitRotation.y = std::stof(m_tempString.GetBuffer());
		m_RotationMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxInitRotation.z = std::stof(m_tempString.GetBuffer());
		// 생명주기당 크기
		m_ScalePerLifeMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_minScalePerLife.x = std::stof(m_tempString.GetBuffer());
		m_ScalePerLifeMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_minScalePerLife.y = std::stof(m_tempString.GetBuffer());
		m_ScalePerLifeMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minScalePerLife.z = std::stof(m_tempString.GetBuffer());

		m_ScalePerLifeMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxScalePerLife.x = std::stof(m_tempString.GetBuffer());
		m_ScalePerLifeMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxScalePerLife.y = std::stof(m_tempString.GetBuffer());
		m_ScalePerLifeMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxScalePerLife.z = std::stof(m_tempString.GetBuffer());
		// 방향
		m_DirectionMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_minDirection.x = std::stof(m_tempString.GetBuffer());
		m_DirectionMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_minDirection.y = std::stof(m_tempString.GetBuffer());
		m_DirectionMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minDirection.z = std::stof(m_tempString.GetBuffer());

		m_DirectionMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxDirection.x = std::stof(m_tempString.GetBuffer());
		m_DirectionMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxDirection.y = std::stof(m_tempString.GetBuffer());
		m_DirectionMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxDirection.z = std::stof(m_tempString.GetBuffer());
		// 이속
		m_StartSpeedMin.GetWindowTextW(m_tempString);
		m_pParticle->m_minCurMoveSpeed = std::stof(m_tempString.GetBuffer());
		m_StartSpeedMax.GetWindowTextW(m_tempString);
		m_pParticle->m_maxCurMoveSpeed = std::stof(m_tempString.GetBuffer());

		m_AccelSpeedMin.GetWindowTextW(m_tempString);
		m_pParticle->m_minAccMoveSpeed = std::stof(m_tempString.GetBuffer());
		m_AccelSpeedMax.GetWindowTextW(m_tempString);
		m_pParticle->m_maxAccMoveSpeed = std::stof(m_tempString.GetBuffer());

		m_MaxSpeedMin.GetWindowTextW(m_tempString);
		m_pParticle->m_minMaxMoveSpeed = std::stof(m_tempString.GetBuffer());
		m_MaxSpeedMax.GetWindowTextW(m_tempString);
		m_pParticle->m_maxMaxMoveSpeed = std::stof(m_tempString.GetBuffer());
		// 회전
		m_RotateDirMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_minDirAngle.x = std::stof(m_tempString.GetBuffer());
		m_RotateDirMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_minDirAngle.y = std::stof(m_tempString.GetBuffer());
		m_RotateDirMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minDirAngle.z = std::stof(m_tempString.GetBuffer());

		m_RotateDirMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxDirAngle.x = std::stof(m_tempString.GetBuffer());
		m_RotateDirMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxDirAngle.y = std::stof(m_tempString.GetBuffer());
		m_RotateDirMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxDirAngle.z = std::stof(m_tempString.GetBuffer());

		m_RotateSpeedMin.GetWindowTextW(m_tempString);
		m_pParticle->m_minRotateSpeed = std::stof(m_tempString.GetBuffer());
		m_RotateSpeedMax.GetWindowTextW(m_tempString);
		m_pParticle->m_maxRotateSpeed = std::stof(m_tempString.GetBuffer());
		// 색상
		m_ColorMinX.GetWindowTextW(m_tempString);
		m_pParticle->m_minColor.x = std::stof(m_tempString.GetBuffer());
		m_ColorMinY.GetWindowTextW(m_tempString);
		m_pParticle->m_minColor.y = std::stof(m_tempString.GetBuffer());
		m_ColorMinZ.GetWindowTextW(m_tempString);
		m_pParticle->m_minColor.z = std::stof(m_tempString.GetBuffer());
		m_ColorMinW.GetWindowTextW(m_tempString);
		m_pParticle->m_minColor.w = std::stof(m_tempString.GetBuffer());

		m_ColorMaxX.GetWindowTextW(m_tempString);
		m_pParticle->m_maxColor.x = std::stof(m_tempString.GetBuffer());
		m_ColorMaxY.GetWindowTextW(m_tempString);
		m_pParticle->m_maxColor.y = std::stof(m_tempString.GetBuffer());
		m_ColorMaxZ.GetWindowTextW(m_tempString);
		m_pParticle->m_maxColor.z = std::stof(m_tempString.GetBuffer());
		m_ColorMaxW.GetWindowTextW(m_tempString);
		m_pParticle->m_maxColor.w = std::stof(m_tempString.GetBuffer());

		m_pParticle->Update();
	}
	catch (...)
	{
		MessageBox(L"잘못 입력된 값이 있습니다.", L"삐빅-");
	}
}


void Dialog_CParticle::OnBnClickedPButtonReset()
{
	if (Dialog_Inspector::Instance->SelectComponent != nullptr)
	{
		UpdateEdit(Dialog_Inspector::Instance->SelectComponent);
		m_pParticle->Update();
	}
}




void Dialog_CParticle::OnBnClickedPisenable()
{
	m_pParticle->isEnable(m_isEnable.GetCheck());
}

void Dialog_CParticle::OnBnClickedPisfollow()
{
	m_pParticle->isScreen(m_isScreen.GetCheck());
}

void Dialog_CParticle::OnBnClickedPisrepeat()
{
	m_pParticle->isRepeat(m_isRepeat.GetCheck());
}


void Dialog_CParticle::OnBnClickedPisbillboard()
{
	m_pParticle->isBillBoard(m_isBillBoard.GetCheck());
}

void Dialog_CParticle::OnBnClickedPisrealfollow()
{
	m_pParticle->isFollow(m_isFollow.GetCheck());
}


void Dialog_CParticle::OnBnClickedPisscalarscale()
{
	m_pParticle->isScalarScale(m_isScalarScale.GetCheck());
	if (m_pParticle->isScalarScale())
	{
		m_ScaleMinY.EnableWindow(0);
		m_ScaleMinZ.EnableWindow(0);
		m_ScaleMaxY.EnableWindow(0);
		m_ScaleMaxZ.EnableWindow(0);
		m_ScalePerLifeMinY.EnableWindow(0);
		m_ScalePerLifeMinZ.EnableWindow(0);
		m_ScalePerLifeMaxY.EnableWindow(0);
		m_ScalePerLifeMaxZ.EnableWindow(0);
	}
	else
	{
		m_ScaleMinY.EnableWindow(1);
		m_ScaleMinZ.EnableWindow(1);
		m_ScaleMaxY.EnableWindow(1);
		m_ScaleMaxZ.EnableWindow(1);
		m_ScalePerLifeMinY.EnableWindow(1);
		m_ScalePerLifeMinZ.EnableWindow(1);
		m_ScalePerLifeMaxY.EnableWindow(1);
		m_ScalePerLifeMaxZ.EnableWindow(1);
	}
}

