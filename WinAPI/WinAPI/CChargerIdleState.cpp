#include "pch.h"
#include "CChargerIdleState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"

#include "CObj.h"
#include "CCharger.h"

CChargerIdleState::CChargerIdleState()
	: m_TargetObject(nullptr)
{
}

CChargerIdleState::~CChargerIdleState()
{
}

void CChargerIdleState::Enter()
{

}

void CChargerIdleState::FinalTick()
{
	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}

	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	assert(pCharger);

	// 플레이어가 x, y 축 범위 내에 있는지 확인
	if (m_TargetObject)
	{
		Vec2 vTargetPos = m_TargetObject->GetPos();
		Vec2 vPos = pCharger->GetPos();

		// X축 라인선상
		if (vTargetPos.x > vPos.x - (pCharger->GetScale().x/2) && vTargetPos.x < vPos.x + (pCharger->GetScale().x / 2))
		{
			// 공격 True
			pCharger->SetIsAttacking(true);

			// 목표 방향으로 이동방향 설정
			if (vTargetPos.y > vPos.y)
				pCharger->SetMoveDir('D');
			else pCharger->SetMoveDir('U');
		}

		// Y축 라인선상
		else if (vTargetPos.y > vPos.y - (pCharger->GetScale().y / 2) && vTargetPos.y < vPos.y + (pCharger->GetScale().y / 2))
		{
			// 공격 True
			pCharger->SetIsAttacking(true);

			// 목표 방향으로 이동방향 설정
			if (vTargetPos.x > vPos.x) pCharger->SetMoveDir('R');
			else pCharger->SetMoveDir('L');
		}
	}

	if (pCharger->GetIsAttacking())
	{
		GetFSM()->ChangeState(L"Attack");
	}
}

void CChargerIdleState::Exit()
{
}
