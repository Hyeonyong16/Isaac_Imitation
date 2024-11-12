#include "pch.h"
#include "CMonstroAttackState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CCamera.h"
#include "CMissile.h"

#include "CObj.h"
#include "CMonstro.h"

#include "CTimeMgr.h"
#include "ctime"

CMonstroAttackState::CMonstroAttackState()
	: m_TargetObject(nullptr)
	, m_isPlay(false)
	, m_isAttack(false)
	, m_curTime(0.f)
{
}

CMonstroAttackState::~CMonstroAttackState()
{
}

void CMonstroAttackState::Enter()
{
	m_isPlay = false;
	m_isAttack = false;
	m_curTime = 0.f;

	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CMonstroAttackState::FinalTick()
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
		srand(time(NULL));

		if (!m_isPlay)
		{
			pMonstroFlipbook->Play(MONSTRO_ATTACK, 10.f, false, false);
			m_isPlay = true;
		}
		
		else
		{
			if (pMonstroFlipbook->isFinish() && !m_isAttack)
			{
				int bulletNum = rand() % 10 + 10;
				Vec2 vMoveDir = m_TargetObject->GetPos() - pMonstro->GetPos();
				vMoveDir.Normalize();

				float degree = (acosf(vMoveDir.x)) * (180 / PI);

				for (int i = 0; i < bulletNum; ++i)
				{
					float randDegree = (float)((rand() % 70) - 35);
					float randVelocityy = (float)((rand() % 300) + 300);
					float resultDegree = (degree + randDegree) * PI / 180;

					Vec2 shotDegree;

					if(vMoveDir.y <= 0)
						shotDegree = Vec2(cosf(resultDegree), -sinf(resultDegree));
					else
						shotDegree = Vec2(cosf(resultDegree), sinf(resultDegree));

					shotDegree.Normalize();

					CMissile* pMissile = new CMissile;
					pMissile->SetPos(pMonstro->GetPos() + Vec2(0.f, -75.f));
					pMissile->SetScale(20.f, 20.f);
					pMissile->SetVelocity(shotDegree * randVelocityy);
					CreateObject(pMissile, LAYER_TYPE::MONSTER_OBJECT);
				}

				m_isAttack = true;
			}

			if(m_curTime >= 1.f)
				GetFSM()->ChangeState(L"Idle");

			m_curTime += DT;
		}
	}
}

void CMonstroAttackState::Exit()
{
}
