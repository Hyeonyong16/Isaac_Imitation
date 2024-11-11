#include "pch.h"
#include "CMonstroChaseState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CFlipbook.h"
#include "CCamera.h"

#include "CObj.h"
#include "CMonstro.h"

#include "CTimeMgr.h"
#include "ctime"

CMonstroChaseState::CMonstroChaseState()
	: m_TargetObject(nullptr)
	, m_isPlay(false)
	, tempPlayerPos{}
	, m_jumpCount(0)
	, m_curTime(0)
	, m_RenderOffset{}
{
}

CMonstroChaseState::~CMonstroChaseState()
{
}

void CMonstroChaseState::Enter()
{
	m_curTime = 0;
	m_isPlay = false;
	tempPlayerPos = Vec2(0.f, 0.f);
	m_jumpCount = 0;
	m_RenderOffset = Vec2(0.f, 0.f);

	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CMonstroChaseState::FinalTick()
{
	CMonstro* pMonstro = dynamic_cast<CMonstro*>(GetOwnerObj());
	assert(pMonstro);

	CFlipbookPlayer* pMonstroFlipbook = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER));
	assert(pMonstroFlipbook);

	if (pMonstro->GetMonInfo().CurHP <= 0)
	{
		GetFSM()->ChangeState(L"Death");
	}

	else
	{
		if (m_jumpCount < 3)
		{
			if (m_isPlay && !pMonstroFlipbook->isFinish())
			{
				Vec2 vMoveDir = tempPlayerPos - pMonstro->GetPos();
				vMoveDir.Normalize();

				Vec2 m_Force = Vec2(0.f, 0.f);
				Vec2 m_Velocity = vMoveDir * 500.f;

				if (pMonstroFlipbook->GetCurSpriteIdx() < (pMonstroFlipbook->GetCurFlipbook()->GetMaxSpriteCount() / 2))
				{
					m_RenderOffset -= Vec2(0.f, 1.f);
				}
				else
				{
					m_RenderOffset += Vec2(0.f, 1.f);
					if (m_RenderOffset.y >= -75.f)
						m_RenderOffset.y = -75.f;
				}

				// 누적된 힘에 의해 생겨난 가속도 계산
				Vec2 vAccel = m_Force;

				// 가속도에 따른 속도의 변화 적용
				m_Velocity += vAccel * DT;

				// 속도에 따른 이동
				Vec2 vPos = pMonstro->GetPos();
				vPos += m_Velocity * DT;
				pMonstro->SetPos(vPos);
				pMonstroFlipbook->SetRenderOffset(m_RenderOffset);
			}

			else if (m_isPlay && pMonstroFlipbook->isFinish())
			{
				m_isPlay = false;
				++m_jumpCount;

				pMonstroFlipbook->Play(MONSTRO_IDLE, 1.f, false, false);
			}

			else
			{
				if (m_curTime >= 0.8f)
				{
					m_isPlay = true;
					pMonstroFlipbook->Play(MONSTRO_CHASE, 10.f, false, false);
					m_curTime = 0.f;
					tempPlayerPos = m_TargetObject->GetPos();
				}

				else
					m_curTime += DT;
			}
		}

		else
		{
			pMonstroFlipbook->Play(MONSTRO_IDLE, 1.f, false, false);
			GetFSM()->ChangeState(L"Idle");
		}
	}
}

void CMonstroChaseState::Exit()
{
}
