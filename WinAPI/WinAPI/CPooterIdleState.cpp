#include "pch.h"
#include "CPooterIdleState.h"

#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"

#include "CCamera.h"
#include "CPooter.h"

#include "CFlipbookPlayer.h"

#include <time.h>

CPooterIdleState::CPooterIdleState()
	: m_TargetObject(nullptr)
	, m_curTime(0.f)
	, m_turnTime(0.f)
	, m_IsTurnRight(true)
{
}

CPooterIdleState::~CPooterIdleState()
{
}

void CPooterIdleState::Enter()
{
	// Level 에서 추적대상 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}

	srand(time(NULL));
	m_turnTime = (float)(rand() % 3) + 0.3f;
	m_curTime = 0.f;
}

void CPooterIdleState::FinalTick()
{
	CPooter* pPooter = dynamic_cast<CPooter*>(GetOwnerObj());
	assert(pPooter);

	if(pPooter->GetIsActive())
	{
		if (m_curTime >= m_turnTime)
		{
			m_IsTurnRight = !m_IsTurnRight;
			m_curTime -= m_turnTime;
			m_turnTime = rand() % 3 + 2;
		}

		CFlipbookPlayer* pFPPooter = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER));
		assert(pFPPooter);

		Vec2 curDir = pPooter->GetMoveDir();
		curDir.Normalize();

		if (m_TargetObject)
		{
			Vec2 vTargetPos = m_TargetObject->GetPos();
			Vec2 vPos = pPooter->GetPos();
			float fDist = (vTargetPos - vPos).Length();

			if (pPooter->GetMonInfo().CurHP <= 0)
			{
				GetFSM()->ChangeState(L"Death");
			}

			// 범위 안에 있으면 자신의 상태를 TraceState
			else if (fDist < pPooter->GetDetectRange())
			{
				GetFSM()->ChangeState(L"Trace");
			}

			// 범위 안이 아니면 이동
			else
			{
				float RotateDir = 0.f;
				if (m_IsTurnRight) RotateDir = 1.f;
				else RotateDir = -1.f;

				Vec2 vRotate = Rotate(curDir, RotateDir * PI * DT);
				pPooter->SetMoveDir(vRotate);

				if (pPooter->GetRigidBody()->GetGroundX() == -1 && vRotate.x > 0.f)
				{
					pPooter->GetRigidBody()->SetGroundX(0);
				}
				else if (pPooter->GetRigidBody()->GetGroundX() == 1 && vRotate.x < 0.f)
				{
					pPooter->GetRigidBody()->SetGroundX(0);
				}

				if (pPooter->GetRigidBody()->GetGroundY() == -1 && vRotate.y > 0.f)
				{
					pPooter->GetRigidBody()->SetGroundY(0);
				}
				else if (pPooter->GetRigidBody()->GetGroundY() == 1 && vRotate.y < 0.f)
				{
					pPooter->GetRigidBody()->SetGroundY(0);
				}

				pPooter->GetRigidBody()->SetVelocity(vRotate * 200.f);

				if (pPooter->GetRigidBody()->GetVelocity().x >= 0)
				{
					if (pFPPooter->GetPlayFlipbookIdx() != POOTER_IDLE_RIGHT)
						pFPPooter->Play(POOTER_IDLE_RIGHT, 5.f, true, false);
				}
				else
				{
					if (pFPPooter->GetPlayFlipbookIdx() != POOTER_IDLE_LEFT)
						pFPPooter->Play(POOTER_IDLE_LEFT, 5.f, true, true);
				}
			}
		}

		m_curTime += DT;

		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pPooter->GetPos());
		DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(pPooter->GetDetectRange() * 2.f, pPooter->GetDetectRange() * 2.f), 0.f);
	}
}

void CPooterIdleState::Exit()
{
}
