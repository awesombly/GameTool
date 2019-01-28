#pragma once
#include "Texture.h"

// 연속된 스프라이트 중 한장의 이미지에 대한 정보
class Sprite
{
public:
	Texture*	m_pTexture;		// 텍스처 포인터
	D3DXVECTOR4	m_textureUV;	// 텍스처 투영좌표
	float		m_frame;		// 해당 스프라이트 간격(sec)
public:
	Sprite(Texture* pTexture, const float& frame, const D3DXVECTOR4& textureUV)
	{
		m_pTexture = pTexture;
		m_frame = frame;
		m_textureUV = textureUV;
	};
	Sprite(Texture* pTexture, const float& frame)
	{
		m_pTexture = pTexture;
		m_frame = frame;
		m_textureUV = { 0.0f, 0.0f, 1.0f, 1.0f };
	};
	virtual ~Sprite() = default;
};