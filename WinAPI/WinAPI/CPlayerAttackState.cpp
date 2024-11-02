#include "pch.h"
#include "CPlayerAttackState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"

#include "CPlayer.h"
#include "CPlayerMoveState.h"

CPlayerAttackState::CPlayerAttackState()
{
}

CPlayerAttackState::~CPlayerAttackState()
{
}

void CPlayerAttackState::Enter()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetOwnerObj());
	assert(pPlayer);

	// 머리, 몸통 Flipbook 가져오기
	CFlipbookPlayer* pFPHead = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Player_Flipbook_Head"));
	assert(pFPHead);

	if (pPlayer->GetIsAttacking())
	{
		switch (pPlayer->GetHeadDir())
		{
		case'L':
			pFPHead->Play(ISAAC_HEAD_LEFT, 10.f, false);
			break;
		case'R':
			pFPHead->Play(ISAAC_HEAD_RIGHT, 10.f, false);
			break;
		case'U':
			pFPHead->Play(ISAAC_HEAD_UP, 10.f, false);
			break;
		case'D':
			pFPHead->Play(ISAAC_HEAD_DOWN, 10.f, false);
			break;
		}
	}
}

void CPlayerAttackState::FinalTick()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetOwnerObj());
	assert(pPlayer);

	// 머리, 몸통 Flipbook 가져오기
	CFlipbookPlayer* pFPHead = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Player_Flipbook_Head"));
	assert(pFPHead);

	if (pPlayer->GetIsAttacking())
	{
		switch (pPlayer->GetHeadDir())
		{
		case'L':
			pFPHead->Play(ISAAC_HEAD_LEFT, 10.f, false);
			break;
		case'R':
			pFPHead->Play(ISAAC_HEAD_RIGHT, 10.f, false);
			break;
		case'U':
			pFPHead->Play(ISAAC_HEAD_UP, 10.f, false);
			break;
		case'D':
			pFPHead->Play(ISAAC_HEAD_DOWN, 10.f, false);
			break;
		}
	}

	if (pPlayer->GetIsDamaged())
	{
		// Damaged State 로 이동
		// 이동 후 체력 계산, 애니메이션 재생
		// SetIsDamaged 로 false 만들기
	}

	else if (pPlayer->GetAttackQueueSize() == 0)
	{
		if (Vec2(0.f, 0.f) == pPlayer->GetVelocity())
		{
			GetFSM()->ChangeState(L"Idle");
		}

		else
		{
			GetFSM()->ChangeState(L"Move");
		}
	}

	
}

void CPlayerAttackState::Exit()
{
}
