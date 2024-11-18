#include "pch.h"
#include "CPooterTraceState.h"

#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"

#include "CCamera.h"
#include "CPooter.h"

#include "CFlipbookPlayer.h"

CPooterTraceState::CPooterTraceState()
	: m_TargetObject(nullptr)
{
}

CPooterTraceState::~CPooterTraceState()
{
}

void CPooterTraceState::Enter()
{
	// Level 에서 추적대상 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CPooterTraceState::FinalTick()
{
	CPooter* pPooter = dynamic_cast<CPooter*>(GetOwnerObj());
	assert(pPooter);

	CFlipbookPlayer* pFPPooter = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER));
	assert(pFPPooter);

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
		else if (fDist < pPooter->GetAttRange())
		{
			GetFSM()->ChangeState(L"Attack");
		}

		else if (fDist > pPooter->GetDetectRange())
		{
			GetFSM()->ChangeState(L"Idle");
		}

		else
		{
			Vec2 vMoveDir = m_TargetObject->GetPos() - pPooter->GetPos();
			vMoveDir.Normalize();

			pPooter->GetRigidBody()->SetVelocity(vMoveDir * 200.f);
		}

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

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pPooter->GetPos());
	DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(pPooter->GetDetectRange() * 2.f, pPooter->GetDetectRange() * 2.f), 0.f);
	DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(pPooter->GetAttRange() * 2.f, pPooter->GetAttRange() * 2.f), 0.f);
}

void CPooterTraceState::Exit()
{
}
