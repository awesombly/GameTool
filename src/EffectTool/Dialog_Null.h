#pragma once



class Dialog_Null : public CFormView
{
public:
	static Dialog_Null* Instance;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Null };
#endif
	static Dialog_Null* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:

protected:
	DECLARE_DYNAMIC(Dialog_Null)
	Dialog_Null();   // 표준 생성자입니다.
	virtual ~Dialog_Null();
}; 