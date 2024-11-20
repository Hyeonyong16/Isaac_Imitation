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

#include "CAssetMgr.h"



CCharger::CCharger()
	: m_moveDir('D')
	, m_TurnDir('L')
	, m_prevDir('D')
	, m_isAttacking(false)
	, m_isTurn(false)
	, m_monsterFlipbook(nullptr)
	, m_isTouchRock(false)
{
	m_Collider = new CCollider;
	m_Collider->SetName(L"Charger_HitBox_01");
	m_Collider->SetScale(Vec2(80.f, 80.f));
	m_Collider->SetOffset(Vec2(0.f, 0.f));
	
	AddComponent(m_Collider);

	tMonInfo temp;
	temp.MaxHP = 3;
	temp.CurHP = 3;
	temp.Speed = 100.f;

	SetMonInfo(temp);

	// Flipbook 생성 및 등록
	CreateChargerFlipbook();
	m_monsterFlipbook->SetName(L"Charger_Flipbook");
	m_monsterFlipbook->SetRenderSize(Vec2(100.f, 100.f));
	m_monsterFlipbook->SetRenderOffset(Vec2(0.f, -25.f));

	m_FSM = (CFSM*)AddComponent(new CFSM);

	m_FSM->AddState(L"Idle", new CChargerIdleState);
	m_FSM->AddState(L"Attack", new CChargerAttackState);
	m_FSM->AddState(L"Death", new CChargerDeathState);

	// RigidBody 컴포넌트 추가
	m_RigidBody = (CRigidBody*)AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetInitialSpeed(250.f);
	m_RigidBody->SetMaxSpeed(250.f);
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
	if(GetIsActive())
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

			if (m_moveDir == 'L')
			{
				if (m_TurnDir == 'L') m_moveDir = 'D';
				else m_moveDir = 'U';
			}
			else if (m_moveDir == 'R')
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
}

void CCharger::Render()
{
	m_monsterFlipbook->Render();
	//DrawDebugRect(PEN_TYPE::GREEN, GetPos(), Vec2(30.f, 30.f), 0.f);

	wchar_t str2[255];
	swprintf_s(str2, 255, L"curHP: %d", (int)GetMonInfo().CurHP);
	//TextOut(CEngine::GetInst()->GetSecondDC(), GetPos().x, GetPos().y, str2, wcslen(str2));

}

void CCharger::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE || _OtherObject->GetLayerType() == LAYER_TYPE::ROCK)
	{
		SetGroundInRigidBody(m_Collider->GetScale()
			, _OtherCollider->GetScale()
			, GetPos()
			, _OtherObject->GetPos()
			, m_RigidBody);

		if (m_isAttacking && _OtherObject->GetLayerType() == LAYER_TYPE::ROCK) m_isTouchRock = true;
		m_isTurn = true;
	}

	if (_OtherObject->GetLayerType() == LAYER_TYPE::PLAYER_OBJECT)
	{
		m_monsterFlipbook->SetIsHitted(true);
	}
}

void CCharger::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CCharger::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE || _OtherObject->GetLayerType() == LAYER_TYPE::ROCK)
	{
		releaseGroundInRigidBody(m_Collider->GetScale()
			, _OtherCollider->GetScale()
			, GetPos()
			, _OtherObject->GetPos()
			, m_RigidBody);

		if (_OtherObject->GetLayerType() == LAYER_TYPE::ROCK) m_isTouchRock = false;
	}
}


void CCharger::CreateChargerFlipbook()
{
	m_monsterFlipbook = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);
	
	m_monsterFlipbook->AddFlipbook(CHARGER_MOVE_UP, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_MOVE_UP", L"Flipbook\\CHARGER_UP_MOVE.flip"));
	m_monsterFlipbook->AddFlipbook(CHARGER_MOVE_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_MOVE_DOWN", L"Flipbook\\CHARGER_DOWN_MOVE.flip"));
	m_monsterFlipbook->AddFlipbook(CHARGER_MOVE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_MOVE_RIGHT", L"Flipbook\\CHARGER_RIGHT_MOVE.flip"));
	m_monsterFlipbook->AddFlipbook(CHARGER_MOVE_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_MOVE_LEFT", L"Flipbook\\CHARGER_RIGHT_MOVE.flip"));

	m_monsterFlipbook->AddFlipbook(CHARGER_ATTACK_UP, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_ATTACK_UP", L"Flipbook\\CHARGER_UP_ATTACK.flip"));
	m_monsterFlipbook->AddFlipbook(CHARGER_ATTACK_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_ATTACK_DOWN", L"Flipbook\\CHARGER_DOWN_ATTACK.flip"));
	m_monsterFlipbook->AddFlipbook(CHARGER_ATTACK_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_ATTACK_RIGHT", L"Flipbook\\CHARGER_RIGHT_ATTACK.flip"));
	m_monsterFlipbook->AddFlipbook(CHARGER_ATTACK_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"CHARGER_ATTACK_LEFT", L"Flipbook\\CHARGER_RIGHT_ATTACK.flip"));
}


void CCharger::SetMaxSpeed(float _speed)
{
	m_RigidBody->SetMaxSpeed(_speed);
}