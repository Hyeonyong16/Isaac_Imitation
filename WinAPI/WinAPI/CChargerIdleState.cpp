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
	// Level �ȿ� �ִ� �÷��̾ ã�´�.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}

	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	assert(pCharger);

	// �÷��̾ x, y �� ���� ���� �ִ��� Ȯ��
	if (m_TargetObject)
	{
		Vec2 vTargetPos = m_TargetObject->GetPos();
		Vec2 vPos = pCharger->GetPos();

		// X�� ���μ���
		if (vTargetPos.x > vPos.x - (pCharger->GetScale().x/2) && vTargetPos.x < vPos.x + (pCharger->GetScale().x / 2))
		{
			// ���� True
			pCharger->SetIsAttacking(true);

			// ��ǥ �������� �̵����� ����
			if (vTargetPos.y > vPos.y)
				pCharger->SetMoveDir('D');
			else pCharger->SetMoveDir('U');
		}

		// Y�� ���μ���
		else if (vTargetPos.y > vPos.y - (pCharger->GetScale().y / 2) && vTargetPos.y < vPos.y + (pCharger->GetScale().y / 2))
		{
			// ���� True
			pCharger->SetIsAttacking(true);

			// ��ǥ �������� �̵����� ����
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
