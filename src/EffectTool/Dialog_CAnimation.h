#pragma once



class Dialog_CAnimation : public CFormView
{
public:
	static Dialog_CAnimation* Instance;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAnimation };
#endif
	static Dialog_CAnimation* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	
protected:
	DECLARE_DYNAMIC(Dialog_CAnimation)
	Dialog_CAnimation();   // 표준 생성자입니다.
	virtual ~Dialog_CAnimation();
};