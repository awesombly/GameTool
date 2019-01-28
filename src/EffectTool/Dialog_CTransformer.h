#pragma once



class Dialog_CTransformer : public CFormView
{
public:
	static Dialog_CTransformer* Instance;
	CTransformer* m_pTrans = nullptr;

	CButton m_isEnable;
	CEdit m_PosX;
	CEdit m_PosY;
	CEdit m_PosZ;
	CEdit m_RotX;
	CEdit m_RotY;
	CEdit m_RotZ;
	CEdit m_ScaleX;
	CEdit m_ScaleY;
	CEdit m_ScaleZ;

	CString m_tempString;
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CTransformer };
#endif
	static Dialog_CTransformer* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	void UpdateEdit(const Component* pComp) noexcept;
protected:
	DECLARE_DYNAMIC(Dialog_CTransformer)
	Dialog_CTransformer();   // 표준 생성자입니다.
	virtual ~Dialog_CTransformer();
public:
	afx_msg void OnBnClickedisenable();
	afx_msg void OnBnClickedTApply();
	afx_msg void OnBnClickedTReset();
};