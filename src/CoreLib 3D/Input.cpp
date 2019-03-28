#include "Input.h"
#include "CoreDX.h"

bool Input::isDebug = false;
//bool Input::isChatting = false;
short	  Input::m_WheelScroll = 0;
short	  Input::m_tempWheelScroll	= 0;
POINT	  Input::m_Cursor			= { 0, };
POINT	  Input::m_prevCursor		= { 0, };
D3DXVECTOR2 Input::m_moveCursorPos = { 0.0f, 0.0f };
EKeyState Input::m_KeyState[256]	 = { EKeyState::FREE, };
EKeyState Input::m_MousePrevState[3] = { EKeyState::FREE, };

bool Input::Init() noexcept
{
	return true;
}

bool Input::Frame() noexcept
{
	// 스크린 좌표 얻고, 스크린->클라이언트 좌표 전환
	m_prevCursor = m_Cursor;
	GetCursorPos(&m_Cursor);
	ScreenToClient(Window::m_hWnd, &m_Cursor);
	m_moveCursorPos.x = float(m_Cursor.x - m_prevCursor.x);
	m_moveCursorPos.y = float(m_Cursor.y - m_prevCursor.y);

	// 마우스 휠 값
	m_WheelScroll = m_tempWheelScroll;
	m_tempWheelScroll = 0;

	KeyCheck(VK_F1);		KeyCheck(VK_F2);		KeyCheck(VK_F3);		KeyCheck(VK_F4);
	KeyCheck(VK_F5);		KeyCheck(VK_F6);		KeyCheck(VK_F7);		KeyCheck(VK_F8);
	KeyCheck(VK_F9);		KeyCheck(VK_F10);		KeyCheck(VK_F11);		KeyCheck(VK_F12);
	KeyCheck(VK_LEFT);		KeyCheck(VK_RIGHT);		KeyCheck(VK_UP);		KeyCheck(VK_DOWN);
	KeyCheck(VK_SPACE);		KeyCheck(VK_SHIFT);		KeyCheck(VK_ESCAPE);	KeyCheck(VK_TAB);
	KeyCheck(VK_INSERT);	KeyCheck(VK_DELETE);	KeyCheck(VK_HOME);		KeyCheck(VK_END);
	KeyCheck(VK_NUMPAD0);	KeyCheck(VK_NUMPAD1);	KeyCheck(VK_NUMPAD2);	KeyCheck(VK_NUMPAD3);
	KeyCheck(VK_NUMPAD4);	KeyCheck(VK_NUMPAD5);	KeyCheck(VK_NUMPAD6);	KeyCheck(VK_NUMPAD7);
	KeyCheck(VK_NUMPAD8);	KeyCheck(VK_NUMPAD9);	KeyCheck(VK_NUMLOCK);
	KeyCheck(VK_ADD);		KeyCheck(VK_SUBTRACT);	KeyCheck(VK_DIVIDE);
	KeyCheck(VK_RETURN);	KeyCheck(VK_CONTROL);	KeyCheck(VK_BACK);
	
	KeyCheck('1');		KeyCheck('2');		KeyCheck('3');		KeyCheck('4');
	KeyCheck('5');		KeyCheck('6');		KeyCheck('7');		KeyCheck('8');
	KeyCheck('9');		KeyCheck('0');		
	KeyCheck('Q');		KeyCheck('W');		KeyCheck('E');		KeyCheck('R');
	KeyCheck('T');		KeyCheck('Y');		KeyCheck('U');		KeyCheck('I');
	KeyCheck('A');		KeyCheck('S');		KeyCheck('D');		KeyCheck('F');
	KeyCheck('G');		KeyCheck('H');		KeyCheck('J');		KeyCheck('K');
	KeyCheck('Z');		KeyCheck('X');		KeyCheck('C');		KeyCheck('V');
	KeyCheck('B');		KeyCheck('N');		KeyCheck('M');
	KeyCheck('O');		KeyCheck('P');		KeyCheck('L');


	if (GetKeyState(VK_TAB) == EKeyState::DOWN)
	{
		isDebug = !isDebug;
	}
	//if (GetKeyState(VK_CONTROL) == EKeyState::DOWN)
	//{
	//	Core::isPlaying = false;
	//	//this_thread::yield();
	//}
	return true;
}

bool Input::Render() noexcept
{
	return true;
}

bool  Input::Release() noexcept
{
	return true;
}

POINT Input::GetCursor()
{
	return m_Cursor;
}

//EKeyState Input::GetMouseState(const EMouseButton& mouseButton)
//{
//	return m_KeyState[mouseButton];
//}

short Input::GetWheelScroll()
{
	return m_WheelScroll;
}

D3DXVECTOR2 Input::GetMouseMovePos()
{
	return m_moveCursorPos;
}

void Input::OperMoveMousePos(const D3DXVECTOR2& vector2)
{
	m_moveCursorPos += vector2;
}


EKeyState Input::GetKeyState(const WORD& keyValue)
{
	return m_KeyState[keyValue];
}

void Input::KeyCheck(const WORD& keyValue)
{
	// GetKeyState()		// 동기식
	// GetAsyncKeyState()	// 비동기식

	short sKey = GetAsyncKeyState(keyValue);
	// 클릭 상태일시 최상위 비트를 1로 바꿔줌
	if (sKey & 0x8000)		// 눌림			
	{
		if (m_KeyState[keyValue] == EKeyState::FREE)
			m_KeyState[keyValue] = EKeyState::DOWN;
		else
			m_KeyState[keyValue] = EKeyState::HOLD;
	}
	else                    // 안눌림
	{
		if (m_KeyState[keyValue] == EKeyState::DOWN ||
			m_KeyState[keyValue] == EKeyState::HOLD)
			m_KeyState[keyValue] = EKeyState::UP;
		else
			m_KeyState[keyValue] = EKeyState::FREE;
	}
}

// 이벤트 핸들러, 윈도우에서 인자를 받아 대신 처리를 수행
void Input::MsgEvent(const MSG& message) noexcept
{
	switch (message.message)
	{
	 case WM_LBUTTONDOWN:
	 {
	 	m_MousePrevState[EMouseButton::Left] = EKeyState::FREE;
		m_KeyState[EMouseButton::Left] = EKeyState::DOWN;
	 } break;
	 case WM_LBUTTONUP:
	 {
	 	m_MousePrevState[EMouseButton::Left] = EKeyState::HOLD;
		m_KeyState[EMouseButton::Left] = EKeyState::UP;
	 } break;
	 case WM_RBUTTONDOWN:
	 {
	 	m_MousePrevState[EMouseButton::Right] = EKeyState::FREE;
		m_KeyState[EMouseButton::Right] = EKeyState::DOWN;
	 } break;
	 case WM_RBUTTONUP:
	 {
	 	m_MousePrevState[EMouseButton::Right] = EKeyState::HOLD;
		m_KeyState[EMouseButton::Right] = EKeyState::UP;
	 } break;
	 case WM_MBUTTONDOWN:
	 {
	 	m_MousePrevState[EMouseButton::Middle] = EKeyState::FREE;
		m_KeyState[EMouseButton::Middle] = EKeyState::DOWN;
	 } break;
	 case WM_MBUTTONUP:
	 {
	 	m_MousePrevState[EMouseButton::Middle] = EKeyState::HOLD;
		m_KeyState[EMouseButton::Middle] = EKeyState::UP;
	 } break;
	 case WM_MOUSEWHEEL: 
	 {
	 	m_tempWheelScroll = (short)HIWORD(message.wParam);
	 }
	}
}

void Input::MouseUpdate()
{
	// 마우스 상태 갱신
	for (int i = 0; i < 3; i++)
	{
		if (m_MousePrevState[i] == EKeyState::FREE)
		{
			if (m_KeyState[i] == EKeyState::DOWN)
			{
				m_MousePrevState[i] = EKeyState::DOWN;
				m_KeyState[i] = EKeyState::HOLD;
			}
		}
		else if (m_MousePrevState[i] == EKeyState::HOLD)
		{
			if (m_KeyState[i] == EKeyState::UP)
			{
				m_MousePrevState[i] = EKeyState::FREE;
				m_KeyState[i] = EKeyState::FREE;
			}
		}
	}
}