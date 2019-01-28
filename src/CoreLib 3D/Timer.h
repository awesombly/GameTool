#pragma once
#include "IBasis.h"
#include "Window.h"
#include <mutex>

class Timer : public IBasis
{
private:
	static int	FPS;				// 초당 프레임

	int		m_FrameCnt;				// 프레임 세는놈
	int		m_DirFrame;				// 목표 프레임
	float	m_kDirTick;				// 목표 프레임 간격
	float	m_ElapseTime;			// 이전 프레임부터 경과 시간
	float	m_GameSpeed;			// 게임 속도
	
	LARGE_INTEGER	m_Frequency;	// 초당 주파수
	LARGE_INTEGER	m_CurrentTick;	// 현재 틱
	LARGE_INTEGER   m_BeforeTick;	// 이전 프레임 틱

	//wostringstream  m_InfoStream;
public:
	static Timer*	Instance;
	static float	SPF;			// 현재 프레임당 시간
	static float	AccumulateTime;	// 총 누적 시간

	std::mutex		m_mutex;
	std::condition_variable m_FrameEvent;		// 쓰레드 이벤트 대기, 알림 발생시 대기중인 쓰레드 가동
	std::condition_variable m_RenderEvent;

	// 초당 발생 이벤트(함수, 인자, 인자)
	static queue<tuple<void(*)(void*, void*), void*, void*> > SecondFrameEvent;
public:
	void ResetSPF()			noexcept;
	void setDirFrame(const int& dirFrame);
	void setGameSpeed(const float& speedRate);

	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	Timer();
	virtual ~Timer();
};