#include "pch.h"
#include "CPlayerMoveState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"

#include "CPlayer.h"

CPlayerMoveState::CPlayerMoveState()
{
}

CPlayerMoveState::~CPlayerMoveState()
{
}

void CPlayerMoveState::Enter()
{
	// �Ӹ�, ���� Flipbook ��������
	CFlipbookPlayer* pFPHead = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Player_Flipbook_Head"));
	assert(pFPHead);
	//CFlipbookPlayer* pFPHead = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Player_Flipbook_Head"));
	//assert(pFPHead);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetOwnerObj());
	assert(pPlayer);

	if (pFPHead)
	{
		switch (pPlayer->GetHeadDir())
		{
		case'L':
			pFPHead->Play(ISAAC_HEAD_IDLE_LEFT, 1.f, false);
			break;
		case'R':
			pFPHead->Play(ISAAC_HEAD_IDLE_RIGHT, 1.f, false);
			break;
		case'U':
			pFPHead->Play(ISAAC_HEAD_IDLE_UP, 1.f, false);
			break;
		case'D':
			pFPHead->Play(ISAAC_HEAD_IDLE_DOWN, 1.f, false);
			break;
		}
	}
}

void CPlayerMoveState::FinalTick()
{
	// �Ӹ�, ���� Flipbook ��������
	CFlipbookPlayer* pFPHead = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Player_Flipbook_Head"));
	assert(pFPHead);
	//CFlipbookPlayer* pFPHead = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Player_Flipbook_Head"));
	//assert(pFPHead);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetOwnerObj());
	assert(pPlayer);

	if (pPlayer->GetIsDamaged())
	{
		// Damaged State �� �̵�
		// �̵� �� ü�� ���, �ִϸ��̼� ���
		// SetIsDamaged �� false �����
	}

	else if (pPlayer->GetAttackQueueSize() > 0)
	{
		// Attack State �� �̵�
		GetFSM()->ChangeState(L"Attack");
	}

	else if (Vec2(0.f, 0.f) == pPlayer->GetVelocity())
	{
		GetFSM()->ChangeState(L"Idle");
	}

	else 
	{
		if (pFPHead)
		{
			switch (pPlayer->GetHeadDir())
			{
			case'L':
				pFPHead->Play(ISAAC_HEAD_IDLE_LEFT, 1.f, false);
				break;
			case'R':
				pFPHead->Play(ISAAC_HEAD_IDLE_RIGHT, 1.f, false);
				break;
			case'U':
				pFPHead->Play(ISAAC_HEAD_IDLE_UP, 1.f, false);
				break;
			case'D':
				pFPHead->Play(ISAAC_HEAD_IDLE_DOWN, 1.f, false);
				break;
			}
		}
	}
}

void CPlayerMoveState::Exit()
{
}
