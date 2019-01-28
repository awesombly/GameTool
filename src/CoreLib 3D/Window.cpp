#include "Window.h"
#include "ObjectManager.h"
#include "CoreDX.h"


// static 초기화
MSG			Window::message;					
RECT		Window::m_winRect;					
RECT		Window::m_realWinRect;				
RECT        Window::m_clientRect{0,0,0,0};
HWND		Window::m_hWnd = 0;			
HINSTANCE	Window::m_hInstance = 0;	
//MyVector2	Window::DrawPoint{ 0, 0 };
Window*		Window::Instance = nullptr;

bool Window::MessageProcess()
{
	while (Core::isPlaying)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// 메세지 해석
			TranslateMessage(&message);
			// 메세지->프로시져 전달
			DispatchMessage(&message);

			//if (message.message == WM_QUIT)
			//	break;
			MsgEvent(message);
		}
		else
		{
			return true;
		}
	}
	return false;
}

// 운영체제로부터 메세지를 받아 처리하는 함수.
// 메세지는 메세지 큐에 저장되고, 필요시 해당 윈도우가 꺼내온다.
// CALLBACK : 사용자가 호출하지 않고 운영체제에서 호출하는 함수
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, 
								 WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		// 최소화 메세지가 아닐시
		if (wParam != SIZE_MINIMIZED)
		{
			// 하위, 상위 바이트에 가로 세로 크기 넘어옴
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			DxManager::Get().ResizeDevice(width, height);

			UpdateWindowRect();
		}
	}	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	} break;
	case WM_DESTROY:
	{
		// 종료 메세지를 윈도우 프로시져에 등록
		Core::isPlaying = false;
		PostQuitMessage(0);
	} break;
	default:
		// 안받은 메세지들은 운영체제로 넘겨 알아서 처리하게 한다
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

// 윈도우 초기 설정, 생성
// CHAR : 멀티바이트, WCHAR : 유니코드, TCHAR : 알아서해줌
bool Window::SetWindow(const HINSTANCE& hInstance, const int& nCmdShow, const TCHAR* title) noexcept
{
	m_hInstance = hInstance;

	// 1. 윈도우 클래스 객체 등록(운영체제)
	WNDCLASSEX winClass;
	// 윈도우 클래스 이름, 크기, 스타일
	winClass.lpszClassName = L"MyWindow";
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	// 윈도우 프로시져, 인스턴스
	winClass.lpfnWndProc = WndProc;
	winClass.hInstance = m_hInstance;
	// 메뉴, 
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	// 아이콘, 커서, 배경 로드
	winClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	winClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	winClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassEx(&winClass))		// 등록 처리
		return false;
	
	AdjustWindowRectEx(&m_winRect, WS_POPUP, false, WS_EX_APPWINDOW);
	m_winRect.right -= m_winRect.left;
	m_winRect.bottom -= m_winRect.top;
	m_winRect.left += m_clientRect.left;
	m_winRect.top += m_clientRect.top;
	

	// 2. 등록된 클래스 객체 사용해 윈도우 생성하고 핸들에 저장
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"MyWindow", title,
							WS_OVERLAPPED,
							m_winRect.left, m_winRect.top,
							m_winRect.right, m_winRect.bottom,
							NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL)
		return false;

	// 윈도우 출력
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);			// 화면 갱신->WM_PAINT 호출

	GetClientRect(m_hWnd, &m_clientRect);

	return true;
}

void Window::SetClientRect(const int& x, const int& y, const int& width, const int& height) noexcept
{
	m_winRect = { 0, 0, width, height };
	m_clientRect = { x, y, width, height };
};

void Window::ResizeWindow(const RECT& rect) noexcept
{
	Instance->m_winRect = rect;
	Instance->m_clientRect = rect;
	AdjustWindowRectEx(&Instance->m_winRect, WS_OVERLAPPED, false, WS_EX_APPWINDOW);
	Instance->m_winRect.right -= Instance->m_winRect.left;
	Instance->m_winRect.bottom -= Instance->m_winRect.top;

	SetWindowPos(m_hWnd, NULL, 0, 0, Instance->m_winRect.right, Instance->m_winRect.bottom, SWP_SHOWWINDOW);
	GetClientRect(m_hWnd, &m_clientRect);
};

RECT& Window::getClientRect()
{
	return m_clientRect;
}

RECT& Window::GetWinRect()
{
	return Instance->m_realWinRect;
}

D3DXVECTOR2 Window::GetWinCenter()
{
	return { (Instance->m_realWinRect.left + Instance->m_realWinRect.right) * 0.5f, (Instance->m_realWinRect.top + Instance->m_realWinRect.bottom) * 0.5f };
}

D3DXVECTOR2 Window::GetClientCenter()
{
	return { (m_clientRect.left + m_clientRect.right) * 0.5f, (m_clientRect.top + m_clientRect.bottom) * 0.5f };
}

void Window::UpdateWindowRect() noexcept
{
	RECT tempRect;
	GetClientRect(Window::m_hWnd, &Instance->m_realWinRect);
	//ScreenToClient(Window::m_hWnd, &Instance->m_realWinRect);
	GetWindowRect(Window::m_hWnd, &tempRect);
	Instance->m_realWinRect.left += tempRect.left;
	Instance->m_realWinRect.right += tempRect.left;
	Instance->m_realWinRect.top += tempRect.top;
	Instance->m_realWinRect.bottom += tempRect.top;
}