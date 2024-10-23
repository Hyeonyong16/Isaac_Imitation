#include "pch.h"
#include "CGuidedMissile.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CCollider.h"

#include "CTimeMgr.h"

CGuidedMissile::CGuidedMissile()
	: m_Target(nullptr)
	, m_DetectRange(300.f)
{
	GetCollider()->SetScale(Vec2(50.f, 50.f));
}

CGuidedMissile::~CGuidedMissile()
{
}

void CGuidedMissile::Tick()
{
	// 타겟이 유효하면 ( nullptr X, Dead 상태 X)
	if (IsValid(m_Target))
	{
		//TraceTarget_0();
		//TraceTarget_1();
		TraceTarget_2();
	}

	// 타겟이 유효하지 않으면
	else
	{
		FindTarget();
	}

	DrawDebugCircle(PEN_TYPE::BLUE, GetRenderPos(), Vec2(m_DetectRange * 2.f, m_DetectRange * 2.f), 0.f);

	CMissile::Tick();
}

void CGuidedMissile::FindTarget()
{
	m_Target = nullptr;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	const vector<CObj*>& vecMon = pCurLevel->GetObjects(LAYER_TYPE::MONSTER);

	// 미사일 본인과 가장 가까운 대상을 타겟으로 지정
	Vec2 vPos = GetPos();
	float MinDistance = m_DetectRange;

	for (size_t i = 0; i < vecMon.size(); ++i)
	{
		if (vecMon[i]->IsDead())
			continue;

		Vec2 vMonPos = vecMon[i]->GetPos();
		float Dist = (vPos - vMonPos).Length();

		// 기존 최소 거리보다 짧은 경우
		if (Dist < MinDistance)
		{
			MinDistance = Dist;
			m_Target = vecMon[i];
		}
	}
}

void CGuidedMissile::TraceTarget_0()
{
	// 타겟을 향해 이동방향 설정
	Vec2 vDir = (m_Target->GetPos() - GetPos());
	vDir.Normalize();
	vDir *= GetVelocity().Length();
	SetVelocity(vDir);
}

void CGuidedMissile::TraceTarget_1()
{
	// 타겟방향을 향해서 힘을 가하는 방법
	Vec2 vDir = (m_Target->GetPos() - GetPos());
	vDir.Normalize();
	AddForce(vDir * 500.f);
}

void CGuidedMissile::TraceTarget_2()
{
	// 타겟을 향해 방향을 회전시키는 방법
	// 현재 미사일의 속력 벡터 정규화
	Vec2 V = GetVelocity();
	V.Normalize();

	// 타겟을 향한 방향 벡터
	Vec2 vDir = m_Target->GetPos() - GetPos();
	vDir.Normalize();

	// 진행 방향 벡터와 타겟 방향 벡터의 사이 각 구하기
	float Dot = V.Dot(vDir);
	float fTheta = acosf(Dot);

	// 라디안 > 60분법으로 변경
	float Degree = fabs((fTheta * 180.f) / PI);

	// 진행 방향, 목적지와의 방향, 각도에 따라서 속도 배율 적용
	float Scale = Degree / 90.f;
	if (1.f < Scale)
		Scale = 1.f;
	Scale = 1.f - Scale;
	Scale = 0.3f + Scale * 0.7f;	// 최소 속도 수치 * 속도 * 배율
	SetVelocityScale(Scale);

	// 두 벡터 사이의 각도가 0 (일정 범위로 지정) 보다 크면 회전
	if (!(0.f <= Degree && Degree <= 2.f))
	{
		float RotateDir = 0.f;
		if (GetClockWise(V, vDir))
			RotateDir = 1.f;
		else
			RotateDir = -1.f;

		Vec2 vRotate = Rotate(V, RotateDir * PI * DT);	// 각도가 작을수록 회전속도가 느림
		vRotate *= GetVelocity().Length();
		SetVelocity(vRotate);
	}
}

void CGuidedMissile::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Monster")
	{
		DeleteObject(this);
		//DeleteObject(_OtherObject);
	}
}
