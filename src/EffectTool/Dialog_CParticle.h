#pragma once


class Dialog_CParticle : public CFormView
{
public:
	static Dialog_CParticle* Instance;
	ParticleSystem* m_pParticle;
	///	
	CButton m_isEnable;
	CButton m_isBillBoard;
	CButton m_isRepeat;
	CButton m_isScreen;
	CButton m_isFollow;
	CButton m_isScalarScale;
	///
	CEdit m_LifeTime;
	CEdit m_Interval;
	CEdit m_MaxCount;
	CEdit m_MinInitCount;
	CEdit m_MaxInitCount;
	///
	CEdit m_LifeMin;
	CEdit m_LifeMax;
	CEdit m_GravityMin;
	CEdit m_GravityMax;
	///
	CEdit m_GravityDirX;
	CEdit m_GravityDirY;
	CEdit m_GravityDirZ;
	///
	CEdit m_PositionMinX;
	CEdit m_PositionMinY;
	CEdit m_PositionMinZ;
	CEdit m_PositionMaxX;
	CEdit m_PositionMaxY;
	CEdit m_PositionMaxZ;
	///
	CEdit m_ScaleMinX;
	CEdit m_ScaleMinY;
	CEdit m_ScaleMinZ;
	CEdit m_ScaleMaxX;
	CEdit m_ScaleMaxY;
	CEdit m_ScaleMaxZ;
	///
	CEdit m_RotationMinX;
	CEdit m_RotationMinY;
	CEdit m_RotationMinZ;
	CEdit m_RotationMaxX;
	CEdit m_RotationMaxY;
	CEdit m_RotationMaxZ;
	///
	CEdit m_ScalePerLifeMinX;
	CEdit m_ScalePerLifeMinY;
	CEdit m_ScalePerLifeMinZ;
	CEdit m_ScalePerLifeMaxX;
	CEdit m_ScalePerLifeMaxY;
	CEdit m_ScalePerLifeMaxZ;
	///
	CEdit m_DirectionMinX;
	CEdit m_DirectionMinY;
	CEdit m_DirectionMinZ;
	CEdit m_DirectionMaxX;
	CEdit m_DirectionMaxY;
	CEdit m_DirectionMaxZ;
	///
	CEdit m_StartSpeedMin;
	CEdit m_StartSpeedMax;
	CEdit m_AccelSpeedMin;
	CEdit m_AccelSpeedMax;
	CEdit m_MaxSpeedMin;
	CEdit m_MaxSpeedMax;
	///
	CEdit m_RotateDirMinX;
	CEdit m_RotateDirMinY;
	CEdit m_RotateDirMinZ;
	CEdit m_RotateDirMaxX;
	CEdit m_RotateDirMaxY;
	CEdit m_RotateDirMaxZ;
	CEdit m_RotateSpeedMin;
	CEdit m_RotateSpeedMax;
	///
	CEdit m_ColorMinX;
	CEdit m_ColorMinY;
	CEdit m_ColorMinZ;
	CEdit m_ColorMinW;
	CEdit m_ColorMaxX;
	CEdit m_ColorMaxY;
	CEdit m_ColorMaxZ;
	CEdit m_ColorMaxW;

	CString m_tempString;
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CParticle };
#endif
	static Dialog_CParticle* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	void UpdateEdit(const Component* pComp)		noexcept;
public:
	DECLARE_DYNAMIC(Dialog_CParticle)
	Dialog_CParticle();   // 표준 생성자입니다.
	virtual ~Dialog_CParticle();
	afx_msg void OnBnClickedPButtonReset();
	afx_msg void OnBnClickedPButtonUpdate();
	afx_msg void OnBnClickedPisfollow();
	afx_msg void OnBnClickedPisenable();
	afx_msg void OnBnClickedPisrepeat();
	afx_msg void OnBnClickedPisbillboard();
	afx_msg void OnBnClickedPisscalarscale();
	afx_msg void OnBnClickedPisrealfollow();
};