#include "pch.h"
#include "EndingObj.h"

#include "CEngine.h"

#include "CSound.h"

#include "CCamera.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTaskMgr.h"
#include "CDbgRender.h"
#include "CPathMgr.h"

#include "CFlipbook.h"
#include "CFlipbookPlayer.h"
#include "CTimeMgr.h"
#include "CComponent.h"

EndingObj::EndingObj()
	: m_flipbookPlayer(nullptr)
	, m_curTime(0.f)
	, m_isFlipbookEnd(false)
	, m_isFadeIn(false)
	, m_sceneChange(false)
{
	m_flipbookPlayer = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);
	m_flipbookPlayer->AddFlipbook(ENDING_POLAROID, CAssetMgr::GetInst()->LoadFlipbook(L"ENDING_POLAROID", L"Flipbook\\ENDING_POLAROID.flip"));
	m_flipbookPlayer->AddFlipbook(THE_END, CAssetMgr::GetInst()->LoadFlipbook(L"THE_END", L"Flipbook\\THE_END.flip"));

	m_flipbookPlayer->SetRenderSize(Vec2(800.f, 800.f));
}

EndingObj::~EndingObj()
{
}

void EndingObj::Begin()
{
	//m_flipbookPlayer->Play(ENDING_POLAROID, 600.f, false);
}

void EndingObj::Tick()
{
	if (!m_isFlipbookEnd)
	{
		if(m_flipbookPlayer->GetPlayFlipbookIdx() != ENDING_POLAROID)
			m_flipbookPlayer->Play(ENDING_POLAROID, 0.7f, false);

		else if (m_flipbookPlayer->GetPlayFlipbookIdx() == ENDING_POLAROID && m_flipbookPlayer->isFinish())
		{
			m_isFlipbookEnd = true;
			m_curTime = 0.f;
		}

		else
		{
			float offset = 800.f;
			offset -= (30.f * m_curTime);

			m_flipbookPlayer->SetRenderSize(Vec2(offset, offset));
		}
	}

	else
	{
		if (!m_isFadeIn)
		{
			if (m_flipbookPlayer->GetPlayFlipbookIdx() != THE_END)
			{
				if (m_curTime >= 1.f)
				{
					m_flipbookPlayer->SetRenderSize(Vec2(800.f, 800.f));
					m_flipbookPlayer->Play(THE_END, 1.f, false);

					m_isFadeIn = true;
					CCamera::GetInst()->PostProcessEffect(FADE_OUT, 5.f);
					m_curTime = 0;
				}
			}
		}

		if (m_curTime >= 5.f)
		{
			m_sceneChange = true;
		}
	}

	m_curTime += DT;
}

void EndingObj::Render()
{
	m_flipbookPlayer->Render();
}
