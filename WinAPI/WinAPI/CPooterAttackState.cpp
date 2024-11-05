#include "pch.h"
#include "CPooterAttackState.h"

#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"

#include "CMissile.h"

#include "CCamera.h"
#include "CPooter.h"

CPooterAttackState::CPooterAttackState()
	: m_TargetObject(nullptr)
{
}

CPooterAttackState::~CPooterAttackState()
{
}

void CPooterAttackState::Enter()
{
	// Level 에서 추적대상 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
	
}

void CPooterAttackState::FinalTick()
{
	CPooter* pPooter = dynamic_cast<CPooter*>(GetOwnerObj());
	assert(pPooter);

	if (m_TargetObject)
	{
		Vec2 vTargetPos = m_TargetObject->GetPos();
		Vec2 vPos = pPooter->GetPos();
		float fDist = (vTargetPos - vPos).Length();

		// 공격 범위 밖일 때
		if (fDist > pPooter->GetAttRange())
		{
			GetFSM()->ChangeState(L"Trace");
		}

		//상태 전환이 아니면 공격
		else
		{
			Vec2 vMoveDir = m_TargetObject->GetPos() - pPooter->GetPos();
			vMoveDir.Normalize();

			if (!pPooter->GetAccTime() || (1.f / pPooter->GetAttSpeed() <= pPooter->GetAccTime()))
			{
				if ((1.f / pPooter->GetAttSpeed() <= pPooter->GetAccTime()))
					pPooter->SetAccTime(pPooter->GetAccTime() - 1.f / pPooter->GetAttSpeed());

				CMissile* pMissile = new CMissile;
				pMissile->SetPos(pPooter->GetPos() + Vec2(0.f, 5.f));
				pMissile->SetScale(20.f, 20.f);
				pMissile->SetVelocity(vMoveDir * 200.f);
				CreateObject(pMissile, LAYER_TYPE::MONSTER_OBJECT);
			}

			pPooter->SetAccTime(pPooter->GetAccTime() + DT);
		}

	}

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pPooter->GetPos());
	DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(pPooter->GetDetectRange() * 2.f, pPooter->GetDetectRange() * 2.f), 0.f);
	DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(pPooter->GetAttRange() * 2.f, pPooter->GetAttRange() * 2.f), 0.f);
}

void CPooterAttackState::Exit()
{
}
