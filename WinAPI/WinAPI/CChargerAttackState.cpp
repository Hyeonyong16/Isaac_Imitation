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

	if (pCharger->GetIsTurn())
	{
		pCharger->SetIsAttacking(false);
		pCharger->SetIsTurn(false);
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
