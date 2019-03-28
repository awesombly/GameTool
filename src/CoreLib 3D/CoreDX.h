#pragma once
#include "Timer.h"
#include "ObjectManager.h"
#include "WriteManager.h"
#include "SoundManager.h"
//#include "../../include/server/ClientServer.h"


class Core : public Window, public IBasis
{
private:
	static Timer   m_Timer;
	Input&		   m_Input = Input::GetInstance();
	SoundManager&  m_SoundManager  = SoundManager::GetInstance();	// 사운드 관리
	DxManager&	   m_DxManager	   = DxManager::GetInstance();		// DirectX 관리
	ObjectManager& m_ObjectManager = ObjectManager::GetInstance();	// 오브젝트 관리
	WriteManager&  m_WriteManager  = WriteManager::GetInstance();	// 문자 출력 관리
	//ClientServer&  m_ClientServer = ClientServer::GetInstance();	// 클라-서버
public:
	static bool isPlaying;		// 프로그램 종료 플래그
public:
	virtual bool GameRun()		noexcept final;
private:
	virtual bool GameInit()		noexcept final;
	virtual bool GameFrame()	noexcept final;
	virtual bool GameRender()	noexcept final;
	virtual bool GameRelease()	noexcept final;
protected:
	virtual void MsgEvent(const MSG& message) noexcept override;
	virtual bool Init()			noexcept = 0;
	virtual bool Frame()		noexcept = 0;
	virtual bool Render()		noexcept = 0;
	virtual bool Release()		noexcept = 0;

	Core() = default;
	virtual ~Core() = default;

	friend class Timer;
};