#pragma once
#include "header.h"

class Window
{
private:
	static MSG		message;					// 메세지 컨테이너
	static RECT		m_winRect;					// 윈도우 크기 속성
	static RECT		m_realWinRect;				// 진짜 윈도우 영역
	static RECT	    m_clientRect;				// 클라이언트 영역
public:
	static HWND				m_hWnd;				// 윈도우 핸들
	static HINSTANCE		m_hInstance;		// 실행 객체 핸들
	static Window*			Instance;			// 접근용
public:
	static void			ResizeWindow(const RECT& rect)	noexcept;
	static RECT&		getClientRect();
	static RECT&		GetWinRect();
	static D3DXVECTOR2	GetWinCenter();
	static D3DXVECTOR2	GetClientCenter();
	static void			UpdateWindowRect()				noexcept;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);							// 메세지 처리 콜백함수
	bool SetWindow(const HINSTANCE& hInstance, const int& nCmdShow, const TCHAR* title = L"SampleWin") noexcept;	// 윈도우 활성화
	void SetClientRect(const int& x, const int& y, const int& width, const int& height) noexcept;						// 화면 크기 설정
	bool MessageProcess();					// 윈도우 메세지 해석, 처리
	virtual void MsgEvent(const MSG& message) = 0;	// 마우스 이벤트 핸들링용
public:
	Window()
	{ Instance = this; }
	virtual ~Window() = default;
};