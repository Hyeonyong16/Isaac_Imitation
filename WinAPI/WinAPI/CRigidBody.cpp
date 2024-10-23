#include "pch.h"
#include "CRigidBody.h"

#include "CTimeMgr.h"

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)
	, m_InitialSpeed(0.f)
	, m_MaxSpeed(1000.f)
	, m_bGround(false)
	, m_Friction(100.f)
	, m_bPrevMove(false)
	, m_bMove(false)
	, m_GravityAccel(Vec2(0.f, 980.f))
	, m_Mode(RIGIDBODY_MODE::TOPVIEW)
	, m_JumpStack(1)
	, m_MaxJumpStack(3)
{

}

CRigidBody::~CRigidBody()
{

}

void CRigidBody::FinalTick()
{
	if (RIGIDBODY_MODE::TOPVIEW == m_Mode)
		FinalTick_TopView();
	else
		FinalTick_BeltScroll();

	// Velocity Debug Render
	DrawDebugLine(PEN_TYPE::BLUE, GetOwner()->GetRenderPos(), GetOwner()->GetRenderPos() + m_Velocity * 0.3f, 0.f);
}

void CRigidBody::FinalTick_TopView()
{
	CObj* pObject = GetOwner();

	// F / M == A
	Vec2 vAccel = m_Force / m_Mass;

	// 가속도에 따른 속도의 변화
	m_Velocity += vAccel * DT;

	// 상태 체크
	CheckState();

	// 초기속도 계산
	CalcInitSpeed_TopView();

	// 마찰력에 따른 속도 감소
	CalcFriction_TopView();

	// 최대속력 제한
	CalcMaxSpeed_TopView();

	// 속도에 따른 이동
	Vec2 vPos = pObject->GetPos();
	vPos += m_Velocity * DT;
	pObject->SetPos(vPos);

	// 힘 초기화
	m_Force = Vec2(0.f, 0.f);
}

void CRigidBody::FinalTick_BeltScroll()
{
	CObj* pObject = GetOwner();

	// F / M == A
	Vec2 vAccel = m_Force / m_Mass;

	// 가속도에 따른 속도의 변화
	m_VelocityX += Vec2(vAccel.x * DT, 0.f);
	m_VelocityY += Vec2(0.f, vAccel.y * DT);

	if (m_Velocity.Length() != 0.f)
		int a = 0;

	// 상태 체크
	CheckState();

	// 초기속도 계산
	CalcInitSpeed_BeltScroll();

	// 마찰력에 따른 속도 감소
	CalcFriction_BeltScroll();

	// 최대속력 제한
	CalcMaxSpeed_BeltScroll();

	// 중력 가속도 계산	
	if (false == m_bGround) // 공중에 있을 경우
		m_VelocityY += m_GravityAccel * DT;
	else if (0.f < m_VelocityY.y)
	{
		m_VelocityY.y = 0.f;
	}

	// 분해된 속도를 합쳐서 최종 속도를 계산한다.
	m_Velocity = m_VelocityX + m_VelocityY;

	// 속도에 따른 이동
	Vec2 vPos = pObject->GetPos();
	vPos += m_Velocity * DT;
	pObject->SetPos(vPos);

	// 초기화
	m_Force = Vec2(0.f, 0.f);
	m_Self = false;
}

void CRigidBody::CheckState()
{
	// 현재 상태를 이전상태로 저장
	m_bPrevMove = m_bMove;

	if (m_Velocity.Length() == 0.f && m_Force.Length() == 0.f)
		m_bMove = false;
	else
		m_bMove = true;
}

void CRigidBody::CalcInitSpeed_TopView()
{
	if (m_bPrevMove == false && m_bMove == true && m_Self)
	{
		Vec2 vDir = m_Velocity;
		vDir.Normalize();
		m_Velocity += vDir * m_InitialSpeed;
	}
}

void CRigidBody::CalcInitSpeed_BeltScroll()
{
	if (m_bPrevMove == false && m_bMove == true && m_Self)
	{
		Vec2 vDir = m_VelocityX;
		vDir.Normalize();
		m_VelocityX += vDir * m_InitialSpeed;
	}
}

void CRigidBody::CalcFriction_TopView()
{
	float Speed = m_Velocity.Length();

	if (0.f != Speed)
	{
		Speed -= m_Friction * m_Mass * DT;
		if (Speed < 0.f)
			Speed = 0.f;

		m_Velocity.Normalize();
		m_Velocity *= Speed;
	}
}

void CRigidBody::CalcFriction_BeltScroll()
{
	float Speed = m_VelocityX.Length();

	if (0.f != Speed)
	{
		Speed -= m_Friction * m_Mass * DT;
		if (Speed < 0.f)
			Speed = 0.f;

		m_VelocityX.Normalize();
		m_VelocityX *= Speed;
	}
}

void CRigidBody::CalcMaxSpeed_TopView()
{
	float Speed = m_Velocity.Length();

	if (m_MaxSpeed < Speed)
	{
		Speed = m_MaxSpeed;
		m_Velocity.Normalize();
		m_Velocity *= Speed;
	}
}

void CRigidBody::CalcMaxSpeed_BeltScroll()
{
	float Speed = m_VelocityX.Length();

	if (m_MaxSpeed < Speed)
	{
		Speed = m_MaxSpeed;
		m_VelocityX.Normalize();
		m_VelocityX *= Speed;
	}
}

void CRigidBody::Jump()
{
	if (RIGIDBODY_MODE::TOPVIEW == m_Mode || m_JumpStack <= 0)
		return;

	m_VelocityY = m_JumpVelocity;
	m_bGround = false;

	--m_JumpStack;
}