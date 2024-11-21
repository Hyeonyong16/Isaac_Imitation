#include "pch.h"
#include "CMonstroJumpState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CCamera.h"
#include "CMissile.h"

#include "CObj.h"
#include "CMonstro.h"

#include "CTimeMgr.h"
#include "ctime"

CMonstroJumpState::CMonstroJumpState()
	: m_TargetObject(nullptr)
	, m_curTime(0)
	, m_isPlay(false)
	, tempPlayerPos{}
	, m_isAttack(false)
{
}

CMonstroJumpState::~CMonstroJumpState()
{
}

void CMonstroJumpState::Enter()
{
	m_curTime = 0;
	m_isPlay = false;
	tempPlayerPos = Vec2(0.f, 0.f);
	m_isAttack = false;

	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CMonstroJumpState::FinalTick()
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
		if(!pMonstro->GetIsJumping())
		{
			if (pMonstro->GetIsGround())
			{
				pMonstro->SetIsGround(false);
				pMonstroFlipbook->Play(MONSTRO_JUMP, 10.f, false, false);	//점프 애님

				pMonstro->SetCanHit(false);
			}
			else if (!pMonstro->GetIsGround() && !pMonstroFlipbook->isFinish())
			{
				pMonstro->SetPos(pMonstro->GetPos() + Vec2(0.f, -2.f));
			}

			else if(!pMonstro->GetIsGround() && pMonstroFlipbook->isFinish())
			{
				pMonstro->SetIsJumping(true);
				m_curTime = 0;

				tempPlayerPos = m_TargetObject->GetPos();
			}
		}

		else
		{
			if (m_curTime >= .3f && !m_isPlay)
			{
				pMonstro->SetPos(tempPlayerPos + Vec2(0.f, -500.f));
				pMonstroFlipbook->Play(MONSTRO_DOWN, 10.f, false, false);	//착지 애님
				m_isPlay = true;
			}

			else if (m_isPlay && !pMonstroFlipbook->isFinish())
			{
				if (pMonstro->GetPos().y < tempPlayerPos.y)
				{
					pMonstro->SetPos(pMonstro->GetPos() + Vec2(0.f, 6.f));
				}
				else
				{
					pMonstro->SetPos(tempPlayerPos);
				}
			}

			// 착지 끝난후 콜라이더 충돌 설정
			else if (m_isPlay && pMonstroFlipbook->isFinish())
			{
				if (!m_isAttack)
				{
					srand(time(NULL));
					int bulletNum = rand() % 10 + 10;

					for (int i = 0; i < bulletNum; ++i)
					{
						int degree = rand() % 360;
						Vec2 vMoveDir = Vec2(cosf(degree), sinf(degree));

						CMissile* pMissile = new CMissile;
						pMissile->SetPos(pMonstro->GetPos() + Vec2(0.f, -75.f));
						pMissile->SetScale(20.f, 20.f);
						pMissile->SetVelocity(vMoveDir * 400.f);
						CreateObject(pMissile, LAYER_TYPE::MONSTER_OBJECT);
					}

					pMonstro->SetCanHit(true);
					m_isAttack = true;
				}

				if(m_curTime >= 1.0f)
				{
					pMonstro->SetIsGround(true);
					pMonstro->SetIsJumping(false);

					GetFSM()->ChangeState(L"Idle");
				}
			}
		}


		pMonstro->GetFlipbook()->isFinish();
	}

	m_curTime += DT;

}

void CMonstroJumpState::Exit()
{
}
