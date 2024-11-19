#include "pch.h"
#include "CHPUI.h"

#include "CEngine.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"

#include "CPlayer.h"
#include "CSprite.h"


CHPUI::CHPUI()
	: m_maxHP(0)
	, m_curHP(0)
	, m_TargetPlayer(nullptr)
	, m_FullHPSprite(nullptr)
	, m_HalfHPSprite(nullptr)
	, m_EmptyHPSprite(nullptr)
{
	m_FullHPSprite = CAssetMgr::GetInst()->LoadSprite(L"UI_HEART_FULL", L"Sprite\\UI_HEART_FULL.sprite");
	m_HalfHPSprite = CAssetMgr::GetInst()->LoadSprite(L"UI_HEART_HALF", L"Sprite\\UI_HEART_HALF.sprite");
	m_EmptyHPSprite = CAssetMgr::GetInst()->LoadSprite(L"UI_HEART_EMPTY", L"Sprite\\UI_HEART_EMPTY.sprite");
}

CHPUI::~CHPUI()
{
}

void CHPUI::Tick_UI()
{
	if (!m_TargetPlayer)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player"));
		
		if (pPlayer)
		{
			m_TargetPlayer = pPlayer;
			m_maxHP = m_TargetPlayer->GetCurHP();
			m_curHP = m_maxHP;
		}
	}

	m_curHP = m_TargetPlayer->GetCurHP();
}

void CHPUI::Render_UI()
{
	int MaxHeartNum = m_maxHP / 2 + (m_maxHP % 2);	// 최대 하트 개수
	int FullHeartNum = m_curHP / 2;					// 현재 꽉찬 하트 수
	bool isHalfHeart = m_curHP % 2;					// 반쪽 하트 존재 여부

	HDC hBackDC = CEngine::GetInst()->GetSecondDC();

	BLENDFUNCTION blend = {};

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	for (int i = 0; i < MaxHeartNum; ++i)
	{
		if (i < FullHeartNum)
		{
			AlphaBlend(hBackDC
				, 20 + i * 50
				, 20
				, 50
				, 50
				, m_FullHPSprite->GetAtlas()->GetDC()
				, m_FullHPSprite->GetLeftTop().x
				, m_FullHPSprite->GetLeftTop().y
				, m_FullHPSprite->GetSlice().x
				, m_FullHPSprite->GetSlice().y
				, blend);
		}

		else if (isHalfHeart)
		{
			if(i == FullHeartNum)
			{
				AlphaBlend(hBackDC
					, 20 + i * 50
					, 20
					, 50
					, 50
					, m_HalfHPSprite->GetAtlas()->GetDC()
					, m_HalfHPSprite->GetLeftTop().x
					, m_HalfHPSprite->GetLeftTop().y
					, m_HalfHPSprite->GetSlice().x
					, m_HalfHPSprite->GetSlice().y
					, blend);
			}
			else
			{
				AlphaBlend(hBackDC
					, 20 + i * 50
					, 20
					, 50
					, 50
					, m_EmptyHPSprite->GetAtlas()->GetDC()
					, m_EmptyHPSprite->GetLeftTop().x
					, m_EmptyHPSprite->GetLeftTop().y
					, m_EmptyHPSprite->GetSlice().x
					, m_EmptyHPSprite->GetSlice().y
					, blend);
			}
		}

		else
		{
			AlphaBlend(hBackDC
				, 20 + i * 50
				, 20
				, 50
				, 50
				, m_EmptyHPSprite->GetAtlas()->GetDC()
				, m_EmptyHPSprite->GetLeftTop().x
				, m_EmptyHPSprite->GetLeftTop().y
				, m_EmptyHPSprite->GetSlice().x
				, m_EmptyHPSprite->GetSlice().y
				, blend);
		}
	}

}
