#include "pch.h"
#include "CPooterAttackState.h"

#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"

#include "CMissile.h"

#include "CCamera.h"
#include "CPooter.h"

#include "CFlipbookPlayer.h"

CPooterAttackState::CPooterAttackState()
	: m_TargetObject(nullptr)
	, m_isPlay(false)
{
}

CPooterAttackState::~CPooterAttackState()
{
}

void CPooterAttackState::Enter()
{
	m_isPlay = false;
	// Level ���� ������� �÷��̾ ã�´�.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CPooterAttackState::FinalTick()
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

		// ���� ���� ���� ��
		if (fDist > pPooter->GetAttRange())
		{
			GetFSM()->ChangeState(L"Trace");
		}

		//���� ��ȯ�� �ƴϸ� ����
		else
		{
			Vec2 vMoveDir = m_TargetObject->GetPos() - pPooter->GetPos();
			vMoveDir.Normalize();

			if (!pPooter->GetAccTime() || (1.f / pPooter->GetAttSpeed() <= pPooter->GetAccTime()))
			{
				if (!m_isPlay)
				{
					if (vMoveDir.x >= 0)
						pFPPooter->Play(POOTER_ATTACK_RIGHT, 10.f, false, false);
					else
						pFPPooter->Play(POOTER_ATTACK_LEFT, 10.f, false, true);

					m_isPlay = true;
				}
			}

			if (m_isPlay && pFPPooter->isFinish())
			{
				if ((1.f / pPooter->GetAttSpeed() <= pPooter->GetAccTime()))
					pPooter->SetAccTime(pPooter->GetAccTime() - 1.f / pPooter->GetAttSpeed());

				CMissile* pMissile = new CMissile;
				pMissile->SetPos(pPooter->GetPos() + Vec2(0.f, 5.f));
				pMissile->SetScale(20.f, 20.f);
				pMissile->SetVelocity(vMoveDir * 200.f);
				CreateObject(pMissile, LAYER_TYPE::MONSTER_OBJECT);

				m_isPlay = false;
				if (vMoveDir.x >= 0)
					pFPPooter->Play(POOTER_IDLE_RIGHT, 5.f, true, false);
				else
					pFPPooter->Play(POOTER_IDLE_LEFT, 5.f, true, true);
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
