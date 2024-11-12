#include "pch.h"
#include "CFlipbookPlayer.h"

#include "CTimeMgr.h"
#include "CFlipbook.h"

#include "CEngine.h"
#include "CSprite.h"
#include "CTexture.h"

#include "CAssetMgr.h"

CFlipbookPlayer::CFlipbookPlayer()
	: CComponent(COMPONENT_TYPE::FLIPBOOKPLAYER)
	, m_CurFlipbook(nullptr)
	, m_SpriteIdx(0)
	, m_FPS(0.f)
	, m_Time(0.f)
	, m_Repeat(false)
	, m_Finish(false)
	, m_Inversion(false)
	, m_renderSize{0, 0}
	, m_renderOffset{0, 0}
	, m_isHitted(false)
{
	m_heartEffect.Time = 0.f;
	m_heartEffect.Duration = .3f;
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
	HDC hInverseDC = CEngine::GetInst()->GetInverseDC();
	Vec2 vPos = GetOwner()->GetRenderPos();

	float NormalizedAge = Saturate(m_heartEffect.Time / m_heartEffect.Duration);
	int Alpha = 255;
	if (m_isHitted)
	{
		NormalizedAge *= 2.f;

		if (NormalizedAge <= 1.f)
		{
			Alpha = (int)(255.f * NormalizedAge);
		}
		else
		{
			NormalizedAge -= 1.f;
			Alpha = (int)(255.f * (1.f - NormalizedAge));
		}
	}

	BLENDFUNCTION blend = {};

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = Alpha;
	blend.AlphaFormat = AC_SRC_ALPHA;

	if(m_renderSize == Vec2(0, 0))
	{
		if(m_Inversion)
		{
			StretchBlt(hInverseDC
				, Sprite->GetSlice().x - 1
				, 0
				, -1 * Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, Sprite->GetAtlas()->GetDC()
				, Sprite->GetLeftTop().x
				, Sprite->GetLeftTop().y
				, Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, SRCCOPY);

			AlphaBlend(hBackDC
				, vPos.x - (Sprite->GetSlice().x / 2) + Sprite->GetOffset().x + m_renderOffset.x
				, vPos.y - (Sprite->GetSlice().y / 2) + Sprite->GetOffset().y + m_renderOffset.y
				, Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, hInverseDC
				, 0
				, 0
				, Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, blend);

			CEngine::GetInst()->ClearInverseBuffer();
		}
		else
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
	}

	else
	{
		if (m_Inversion)
		{
			StretchBlt(hInverseDC
				, Sprite->GetSlice().x - 1
				, 0
				, -1 * Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, Sprite->GetAtlas()->GetDC()
				, Sprite->GetLeftTop().x
				, Sprite->GetLeftTop().y
				, Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, SRCCOPY);

			AlphaBlend(hBackDC
				, vPos.x - (m_renderSize.x / 2) + Sprite->GetOffset().x + m_renderOffset.x
				, vPos.y - (m_renderSize.y / 2) + Sprite->GetOffset().y + m_renderOffset.y
				, m_renderSize.x
				, m_renderSize.y
				, hInverseDC
				, 0
				, 0
				, Sprite->GetSlice().x
				, Sprite->GetSlice().y
				, blend);
			CEngine::GetInst()->ClearInverseBuffer();
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

	// 후처리 효과시간이 만료되면 기능을 Off 한다.
	m_heartEffect.Time += DT;
	if (m_heartEffect.Duration <= m_heartEffect.Time)
	{
		m_isHitted = false;
	}
}


