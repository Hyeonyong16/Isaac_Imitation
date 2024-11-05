#include "pch.h"
#include "CCharger.h"

#include "CEngine.h"
#include "CFSM.h"

#include "CFlipbookPlayer.h"
#include "CRigidBody.h"
#include "CCollider.h"

#include "CDbgRender.h"
#include "CChargerIdleState.h"
#include "CChargerAttackState.h"
#include "CChargerDeathState.h"



CCharger::CCharger()
	: m_moveDir('D')
	, m_TurnDir('L')
	, m_prevDir('D')
	, m_isAttacking(false)
	, m_isTurn(false)
	, m_monsterFlipbook(nullptr)
{
	m_Collider = new CCollider;
	m_Collider->SetName(L"Charger_HitBox_01");
	m_Collider->SetScale(Vec2(50.f, 50.f));
	m_Collider->SetOffset(Vec2(0.f, 0.f));
	
	AddComponent(m_Collider);

	tMonInfo temp;
	temp.MaxHP = 6;
	temp.CurHP = 6;
	temp.Speed = 100.f;

	SetMonInfo(temp);

	m_FSM = (CFSM*)AddComponent(new CFSM);

	m_FSM->AddState(L"Idle", new CChargerIdleState);
	m_FSM->AddState(L"Attack", new CChargerAttackState);
	m_FSM->AddState(L"Death", new CChargerDeathState);

	// RigidBody 컴포넌트 추가
	m_RigidBody = (CRigidBody*)AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetInitialSpeed(400.f);
	m_RigidBody->SetMaxSpeed(400.f);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetFriction(1000.f);
}

CCharger::~CCharger()
{
}

void CCharger::Begin()
{
	CMonster::Begin();


	m_FSM->ChangeState(L"Idle");
}

void CCharger::Tick()
{
	if (GetMonInfo().CurHP == 0)
	{
		m_RigidBody->SetVelocity(Vec2(0.f, 0.f));
		return;
	}

	if (m_prevDir != m_moveDir)
	{
		m_RigidBody->SetVelocity(Vec2(0.f, 0.f));
	}

	if (m_isTurn)
	{

		if(m_moveDir == 'L')
		{
			if (m_TurnDir == 'L') m_moveDir = 'D';
			else m_moveDir = 'U';
		}
		else if(m_moveDir == 'R')
		{
			if (m_TurnDir == 'L') m_moveDir = 'U';
			else m_moveDir = 'D';
		}
		else if (m_moveDir == 'U')
		{
			if (m_TurnDir == 'L') m_moveDir = 'L';
			else m_moveDir = 'R';
		}
		else
		{
			if (m_TurnDir == 'L') m_moveDir = 'R';
			else m_moveDir = 'L';
		}

		if (!m_isAttacking)
			m_isTurn = false;
	}

	Vec2 tempForce(0, 0);

	switch (m_moveDir)
	{
	case 'L':
		tempForce += Vec2(-1.f, 0.f);
		break;
	case 'R':
		tempForce += Vec2(1.f, 0.f);
		break;
	case 'U':
		tempForce += Vec2(0.f, -1.f);
		break;
	case 'D':
		tempForce += Vec2(0.f, 1.f);
		break;
	}

	tempForce.Normalize();
	tempForce *= 2000;
	if (m_RigidBody->GetGroundX() == -1 && tempForce.x > 0.f)
	{
		m_RigidBody->SetGroundX(0);
	}
	else if (m_RigidBody->GetGroundX() == 1 && tempForce.x < 0.f)
	{
		m_RigidBody->SetGroundX(0);
	}

	if (m_RigidBody->GetGroundY() == -1 && tempForce.y > 0.f)
	{
		m_RigidBody->SetGroundY(0);
	}
	else if (m_RigidBody->GetGroundY() == 1 && tempForce.y < 0.f)
	{
		m_RigidBody->SetGroundY(0);
	}

	m_RigidBody->AddForce(m_RigidBody->GetForce() * -1, true);
	m_RigidBody->AddForce(tempForce, true);

	m_prevDir = m_moveDir;
}

void CCharger::Render()
{
	//m_monsterFlipbook->Render();
	//DrawDebugRect(PEN_TYPE::GREEN, GetPos(), Vec2(30.f, 30.f), 0.f);

	wchar_t str1[255];
	swprintf_s(str1, 255, L"MoveDir: %c", m_moveDir);
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 90, m_FSM->GetCurState().c_str(), wcslen(m_FSM->GetCurState().c_str()));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 110, str1, wcslen(str1));


	wchar_t str2[255];
	swprintf_s(str2, 255, L"curHP: %d", (int)GetMonInfo().CurHP);
	TextOut(CEngine::GetInst()->GetSecondDC(), GetPos().x, GetPos().y, str2, wcslen(str2));

}

void CCharger::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE)
	{
		SetGroundInRigidBody(m_Collider->GetScale()
			, _OtherCollider->GetScale()
			, GetPos()
			, _OtherObject->GetPos()
			, m_RigidBody);

		m_isTurn = true;
	}
}

void CCharger::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CCharger::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE)
	{
		releaseGroundInRigidBody(m_Collider->GetScale()
			, _OtherCollider->GetScale()
			, GetPos()
			, _OtherObject->GetPos()
			, m_RigidBody);
	}
}


void CCharger::CreateChargerFlipbook()
{

	//m_monsterFlipbook = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);
	
	//m_FlipbookHead->AddFlipbook(ISAAC_HEAD_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_LEFT", L"Flipbook\\ISAAC_HEAD_LEFT.flip"));
}


void CCharger::SetMaxSpeed(float _speed)
{
	m_RigidBody->SetMaxSpeed(_speed);
}