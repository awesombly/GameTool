#pragma once




class Dialog_CCollider : public CFormView
{
public:
	static Dialog_CCollider* Instance;

	Collider* m_pCollider = nullptr;

	CEdit m_Radius;
	CEdit m_MinX;
	CEdit m_MinY;
	CEdit m_MinZ;
	CEdit m_MaxX;
	CEdit m_MaxY;
	CEdit m_MaxZ;
	CEdit m_DirectionX;
	CEdit m_DirectionY;
	CEdit m_DirectionZ;
	CEdit m_Damping;
	CEdit m_Repulsion;
	CEdit m_GravityScale;

	CButton m_isEnable;
	CButton m_usePhysics;

	CString m_tempString;
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Collider };
#endif
	static Dialog_CCollider* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	void UpdateEdit(const Component* pComp)		noexcept;
protected:
	DECLARE_DYNAMIC(Dialog_CCollider)
	Dialog_CCollider();   // 표준 생성자입니다.
	virtual ~Dialog_CCollider();
public:
	afx_msg void OnBnClickedCApply();
	afx_msg void OnBnClickedCReset();
};