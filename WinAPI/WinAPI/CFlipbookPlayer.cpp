#include "pch.h"
#include "CFlipbookPlayer.h"

#include "CTimeMgr.h"
#include "CFlipbook.h"

#include "CEngine.h"
#include "CSprite.h"
#include "CTexture.h"

CFlipbookPlayer::CFlipbookPlayer()
	: CComponent(COMPONENT_TYPE::FLIPBOOKPLAYER)
	, m_CurFlipbook(nullptr)
	, m_SpriteIdx(0)
	, m_FPS(0.f)
	, m_Time(0.f)
	, m_Repeat(false)
	, m_Finish(false)
	, m_renderSize{0, 0}
	, m_renderOffset{0, 0}
{
}

CFlipbookPlayer::~CFlipbookPlayer()
{
}

void CFlipbookPlayer::FinalTick()
{
	// 현재 재생중인 Flipbook 이 없으면 끝
	if (nullptr == m_CurFlipbook)
	{
		return;
	}

	if (m_Finish)
	{
		// 반복 재생을 하기로 했으면 0번 스프라이트로 인덱스 변경
		if (m_Repeat)
			Reset();

		// 한번 재생이 끝난 상태 + 반복 재생상태가 아니면 마지막 프레임 유지
		else
			return;
	}

	// FPS 에 따른 시간체크
	m_Time += DT;
	if (1.f / m_FPS <= m_Time)
	{
		m_Time -= 1.f / m_FPS;
		++m_SpriteIdx;

		// 마지막 Sprite 에 도달했다면
		if (m_CurFlipbook->GetMaxSpriteCount() <= m_SpriteIdx)
		{
			m_Finish = true;
			--m_SpriteIdx;
		}
	}
}

void CFlipbookPlayer::Render()
{
	if (nullptr == m_CurFlipbook)
		return;

	CSprite* Sprite = m_CurFlipbook->GetSprite(m_SpriteIdx);

	// Sprite 를 화면에 그린다.
	HDC hBackDC = CEngine::GetInst()->GetSecondDC();
	Vec2 vPos = GetOwner()->GetRenderPos();

	BLENDFUNCTION blend = {};

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	if(m_renderSize == Vec2(0, 0))
	{
		AlphaBlend(hBackDC
			, vPos.x - (Sprite->GetSlice().x / 2) + Sprite->GetOffset().x + m_renderOffset.x
			, vPos.y - (Sprite->GetSlice().y / 2) + Sprite->GetOffset().y + m_renderOffset.y
			, Sprite->GetSlice().x
			, Sprite->GetSlice().y
			, Sprite->GetAtlas()->GetDC()
			, Sprite->GetLeftTop().x
			, Sprite->GetLeftTop().y
			, Sprite->GetSlice().x
			, Sprite->GetSlice().y
			, blend);
	}

	else
	{
		AlphaBlend(hBackDC
			, vPos.x - (m_renderSize.x / 2) + Sprite->GetOffset().x + m_renderOffset.x
			, vPos.y - (m_renderSize.y / 2) + Sprite->GetOffset().y + m_renderOffset.y
			, m_renderSize.x
			, m_renderSize.y
			, Sprite->GetAtlas()->GetDC()
			, Sprite->GetLeftTop().x
			, Sprite->GetLeftTop().y
			, Sprite->GetSlice().x
			, Sprite->GetSlice().y
			, blend);
	}
}


