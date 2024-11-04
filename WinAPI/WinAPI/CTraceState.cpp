#include "pch.h"
#include "CTraceState.h"

#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"

#include "CCamera.h"

CTraceState::CTraceState()
	: m_TargetObject(nullptr)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::Enter()
{
	// Level 에서 추적대상 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}
}

void CTraceState::FinalTick()
{
	// 몬스터의 스탯정보를 가져온다.
	CMonster* pMon = dynamic_cast<CMonster*>(GetOwnerObj());
	assert(pMon);
	const tMonInfo& info = pMon->GetMonInfo();

	// 몬스터의 이동속에 맞게 플레이어를 향해서 이동한다.
	// 이동 방향 계산
	Vec2 vMoveDir = m_TargetObject->GetPos() - pMon->GetPos();
	vMoveDir.Normalize();

	// 해당 방향으로, 속력에 맞게 매프레임마다 이동
	Vec2 vPos = pMon->GetPos() + vMoveDir * info.Speed * DT;
	pMon->SetPos(vPos);

	// 플레이어가 공격범위 안으로 들어오면 Attack 상태로 변경한다.

	// 만약에 추적중에 피격을 당하면, 피격상태로 전환다.


	// 인지범위 시각화

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(pMon->GetPos());
	//DrawDebugCircle(PEN_TYPE::BLUE, vRenderPos, Vec2(info.DetectRange * 2.f, info.DetectRange * 2.f), 0.f);
}

void CTraceState::Exit()
{
}