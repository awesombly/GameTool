#include "MySound.h"



MySound::MySound()
{}


MySound::~MySound()
{}

unsigned int MySound::GetTotalSecond() noexcept
{
	//m_Sound->getLength(&m_TotalLength, FMOD_TIMEUNIT_MS);
	return m_TotalLength / 1000;
}

unsigned int MySound::GetCurrentSecond() noexcept
{
	m_Channel->getPosition(&m_CurrentLength, FMOD_TIMEUNIT_MS);
	return m_CurrentLength / 1000;
}

float MySound::GetVolume() noexcept
{
	m_Channel->getVolume(&m_CurrentVolume);
	return m_CurrentVolume;
}

void  MySound::OperVolume(const float& value) noexcept
{
	m_CurrentVolume = clamp(GetVolume() + value, 0.0f, 1.0f);

	m_Channel->setVolume(m_CurrentVolume);
}

void  MySound::SetVolume(const float& value) noexcept
{
	m_CurrentVolume = clamp(value, 0.0f, 1.0f);

	m_Channel->setVolume(m_CurrentVolume);
}