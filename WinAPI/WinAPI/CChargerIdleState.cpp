#include "pch.h"
#include "CChargerIdleState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CCamera.h"

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
	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	pCharger->SetMaxSpeed(400.f);

	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CChargerIdleState::FinalTick()
{
	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	assert(pCharger);

	if(pCharger->GetIsActive())
	{
		CFlipbookPlayer* pFPCharger = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER));
		assert(pFPCharger);

		// 플레이어가 x, y 축 범위 내에 있는지 확인
		if (m_TargetObject)
		{
			Vec2 vTargetPos = m_TargetObject->GetPos();
			Vec2 vPos = pCharger->GetPos();
			float fDist = (vTargetPos - vPos).Length();

			if(fDist <= 350.f)
			{
				// X축 라인선상
				if (vTargetPos.x > vPos.x - (pCharger->GetScale().x / 2) && vTargetPos.x < vPos.x + (pCharger->GetScale().x / 2))
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
		}

		if (pFPCharger)
		{
			switch (pCharger->GetMoveDir())
			{
			case'L':
				if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_MOVE_LEFT)
					pFPCharger->Play(CHARGER_MOVE_LEFT, 15.f, true, true);
				break;
			case'R':
				if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_MOVE_RIGHT)
					pFPCharger->Play(CHARGER_MOVE_RIGHT, 15.f, true, false);
				break;
			case'U':
				if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_MOVE_UP)
					pFPCharger->Play(CHARGER_MOVE_UP, 15.f, true, false);
				break;
			case'D':
				if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_MOVE_DOWN)
					pFPCharger->Play(CHARGER_MOVE_DOWN, 15.f, true, false);
				break;
			}
		}

		if (pCharger->GetMonInfo().CurHP <= 0)
		{
			GetFSM()->ChangeState(L"Death");
		}

		else if (pCharger->GetIsAttacking() && !pCharger->GetIsTouchRock())
		{
			GetFSM()->ChangeState(L"Attack");
		}

		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pCharger->GetPos());
		DrawDebugRect(PEN_TYPE::BLUE, vRenderPos, Vec2(700.f, pCharger->GetScale().y), 0);
		DrawDebugRect(PEN_TYPE::BLUE, vRenderPos, Vec2(pCharger->GetScale().x, 700.f), 0);
	}
}

void CChargerIdleState::Exit()
{
}
