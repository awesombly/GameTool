#pragma once
#include "ISingleton.h"
#include "IBasis.h"
#include "Window.h"

enum class EKeyState : char {
	FREE = 0,
	DOWN,
	UP,
	HOLD
};

enum EMouseButton : char {
	Left = VK_LBUTTON, Right= VK_RBUTTON, Middle = VK_MBUTTON,
};

class Input : public ISingleton<Input>, public IBasis
{
public:
	static bool			isDebug;
private:
	static short		m_WheelScroll;
	static short		m_tempWheelScroll;
	static POINT		m_Cursor;
	static POINT		m_prevCursor;
	static D3DXVECTOR2	m_moveCursorPos;
	static EKeyState	m_KeyState[256];
	static EKeyState	m_MousePrevState[3];
private:
	void			   KeyCheck(const WORD& key);			// 키 검사(1회만)
public:
	static POINT	   GetCursor();
	///
	//static EKeyState   GetMouseState(const EMouseButton& mouseButton);
	static short	   GetWheelScroll();
	static EKeyState   GetKeyState(const WORD& keyValue);
	static D3DXVECTOR2 GetMouseMovePos();
	static void		   OperMoveMousePos(const D3DXVECTOR2& vector2);

	void		 MsgEvent(const MSG& message) noexcept;		// 마우스 클릭 처리
	void		 MouseUpdate();								// 마우스 상태 갱신
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
private:
	Input() = default;
public:
	friend class ISingleton<Input>;
	friend class Timer;
	virtual ~Input() = default;
};