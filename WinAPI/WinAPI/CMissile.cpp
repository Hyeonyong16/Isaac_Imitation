#include "pch.h"
#include "CMissile.h"

#include "CEngine.h"
#include "CTimeMgr.h"

#include "CCollider.h"

CMissile::CMissile()
	: m_Mass(1.f)
	, m_VelocityScale(1.f)
{
	m_Collider = (CCollider*)AddComponent(new CCollider);
	m_Collider->SetScale(Vec2(15.f, 15.f));

	m_Velocity = Vec2(1.f, 1.f);
	m_Velocity.Normalize();
	m_Velocity *= 500.f;
}

CMissile::~CMissile()
{
}

void CMissile::Tick()
{
	// 누적된 힘에 의해 생겨난 가속도 계산
	Vec2 vAccel = m_Force / m_Mass;

	// 가속도에 따른 속도의 변화 적용
	m_Velocity += vAccel * DT;

	// 속도에 따른 이동
	Vec2 vPos = GetPos();
	vPos += m_Velocity * m_VelocityScale * DT;
	SetPos(vPos);

	DrawDebugLine(PEN_TYPE::BLUE, GetRenderPos(), GetRenderPos() + m_Velocity * 0.1f, 0.f);

	// 현재 적용중인 힘을 리셋
	m_Force = Vec2(0.f, 0.f);
}

void CMissile::Render()
{
	HDC dc = CEngine::GetInst()->GetSecondDC();

	Vec2 vPos = GetRenderPos();
	Vec2 vScale = GetScale();

	Ellipse(dc, vPos.x - vScale.x / 2.f, vPos.y - vScale.y / 2
		, vPos.x + vScale.x / 2.f, vPos.y + vScale.y / 2.f);
}

