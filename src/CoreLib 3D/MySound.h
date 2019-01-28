#pragma once
#include "header.h"
//#include "fmod.h"
#include "fmod.hpp"			// fmod 헤더파일
#include "fmod_errors.h"	// "

#ifdef _WIN64 
#pragma comment( lib, "fmod64_vc.lib")
#else
#pragma comment( lib, "fmod_vc.lib")
#endif

class MySound
{
public:
	FMOD::Sound*	m_Sound;
	FMOD::Channel*	m_Channel;
	string			m_SoundName;

	unsigned int	m_TotalLength;
	unsigned int	m_CurrentLength;
	float			m_CurrentVolume;
public:
	unsigned int GetTotalSecond()		 noexcept;
	unsigned int GetCurrentSecond()		 noexcept;
	float GetVolume()					 noexcept;
	void  OperVolume(const float& value) noexcept;
	void  SetVolume(const float& value)  noexcept;
public:
	MySound();
	virtual ~MySound();
};

