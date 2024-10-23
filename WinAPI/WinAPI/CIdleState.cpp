#include "pch.h"
#include "CIdleState.h"


#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"

#include "CMonster.h"
#include "CCamera.h"


CIdleState::CIdleState()
	: m_TargetObject(nullptr)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
	// 소유자(몬스터) 의 FlipbookPlayer 를 이용해서 특정 플립북(애니메이션) 을 재생시킨다.
	CFlipbookPlayer* pFP = GetOwnerObj()->GetComponent<CFlipbookPlayer>();
	if (pFP)
	{
		//pFP->Play();
	}
}

void CIdleState::FinalTick()
{
	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}

	// 몬스터의 스탯을 가져온다.
	CMonster* pMon = dynamic_cast<CMonster*>(GetOwnerObj());
	assert(pMon);
	const tMonInfo& info = pMon->GetMonInfo();

	// 플레이어가 몬스터의 인지범위 내에 있는지 확인한다.
	if (m_TargetObject)
	{
		Vec2 vTargetPos = m_TargetObject->GetPos();
		Vec2 vPos = pMon->GetPos();
		float fDist = (vTargetPos - vPos).Length();

		// 범위 안에 있으면 자신의 상태를 TraceState
		if (fDist < info.DetectRange)
		{
			GetFSM()->ChangeState(L"Trace");
		}
	}

	// 인지범위 시각화

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pMon->GetPos());
	DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(info.DetectRange * 2.f, info.DetectRange * 2.f), 0.f);
}

void CIdleState::Exit()
{
}