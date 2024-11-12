#include "pch.h"
#include "CChargerAttackState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"

#include "CObj.h"
#include "CCharger.h"

CChargerAttackState::CChargerAttackState()
{
}

CChargerAttackState::~CChargerAttackState()
{
}

void CChargerAttackState::Enter()
{
	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	pCharger->SetMaxSpeed(1000.f);
}

void CChargerAttackState::FinalTick()
{
	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	assert(pCharger);

	CFlipbookPlayer* pFPCharger = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER));
	assert(pFPCharger);

	if (pCharger->GetIsTurn())
	{
		pCharger->SetIsAttacking(false);
		pCharger->SetIsTurn(false);
	}

	if (pFPCharger)
	{
		switch (pCharger->GetMoveDir())
		{
		case'L':
			if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_ATTACK_LEFT)
				pFPCharger->Play(CHARGER_ATTACK_LEFT, 1.f, false, true);
			break;
		case'R':
			if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_ATTACK_RIGHT)
				pFPCharger->Play(CHARGER_ATTACK_RIGHT, 1.f, false, false);
			break;
		case'U':
			if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_ATTACK_UP)
				pFPCharger->Play(CHARGER_ATTACK_UP, 1.f, false, false);
			break;
		case'D':
			if (pFPCharger->GetPlayFlipbookIdx() != CHARGER_ATTACK_DOWN)
				pFPCharger->Play(CHARGER_ATTACK_DOWN, 1.f, false, false);
			break;
		}
	}

	if (pCharger->GetMonInfo().CurHP <= 0)
	{
		GetFSM()->ChangeState(L"Death");
	}

	else if (!pCharger->GetIsAttacking())
	{
		GetFSM()->ChangeState(L"Idle");
	}
}

void CChargerAttackState::Exit()
{
	
}
