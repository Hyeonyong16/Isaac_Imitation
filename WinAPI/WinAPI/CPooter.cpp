#include "pch.h"
#include "CPooter.h"

#include "CEngine.h"
#include "CFSM.h"

#include "CFlipbookPlayer.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CTimeMgr.h"

#include "CDbgRender.h"
#include "CPooterIdleState.h"
#include "CPooterTraceState.h"
#include "CPooterAttackState.h"
#include "CPooterDeathState.h"

#include "CAssetMgr.h"



CPooter::CPooter()
	: m_LookDir('L')
	, m_DetectRange(400.f)
	, m_AttRange(250.f)
	, m_AttSpeed(0.5f)
	, m_AccTime(0.f)
	, m_monsterFlipbook(nullptr)
	, m_moveDir{1.f, 0.f}
	, m_isAttack(false)
{
	m_Collider = new CCollider;
	m_Collider->SetName(L"Pooter_HitBox_01");
	m_Collider->SetScale(Vec2(50.f, 50.f));
	m_Collider->SetOffset(Vec2(0.f, 0.f));

	AddComponent(m_Collider);

	tMonInfo temp;
	temp.MaxHP = 3;
	temp.CurHP = 3;
	temp.Speed = 100.f;

	SetMonInfo(temp);

	// Flipbook 생성 및 등록
	CreatePooterFlipbook();
	m_monsterFlipbook->SetName(L"Pooter_Flipbook");
	m_monsterFlipbook->SetRenderSize(Vec2(50.f, 50.f));
	m_monsterFlipbook->SetRenderOffset(Vec2(0.f, -25.f));

	m_FSM = (CFSM*)AddComponent(new CFSM);

	m_FSM->AddState(L"Idle", new CPooterIdleState);
	m_FSM->AddState(L"Trace", new CPooterTraceState);
	m_FSM->AddState(L"Attack", new CPooterAttackState);
	m_FSM->AddState(L"Death", new CPooterDeathState);

	// RigidBody 컴포넌트 추가
	m_RigidBody = (CRigidBody*)AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetInitialSpeed(200.f);
	m_RigidBody->SetMaxSpeed(200.f);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetFriction(1000.f);
}

CPooter::~CPooter()
{
}


void CPooter::Begin()
{
	CMonster::Begin();

	m_FSM->ChangeState(L"Idle");
}

void CPooter::Tick()
{
	wstring curStateName = m_FSM->GetCurState();
	if (curStateName != L"Attack")
	{
		if (m_AccTime != 0.f)
		{
			if (m_AccTime >= 1.f / m_AttSpeed)
				m_AccTime = 0.f;
			else
				m_AccTime += DT;
		}
	}
}

void CPooter::Render()
{
	m_monsterFlipbook->Render();

	//TextOut(CEngine::GetInst()->GetSecondDC(), 10, 130, m_FSM->GetCurState().c_str(), wcslen(m_FSM->GetCurState().c_str()));

	wchar_t str[255];
	swprintf_s(str, 255, L"Pooter Pos x: %d, y: %d", (int)GetPos().x, (int)GetPos().y);
	//TextOut(CEngine::GetInst()->GetSecondDC(), 10, 150, str, wcslen(str));
}


void CPooter::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE)
	{
		SetGroundInRigidBody(m_Collider->GetScale()
			, _OtherCollider->GetScale()
			, GetPos()
			, _OtherObject->GetPos()
			, m_RigidBody);
	}
}

void CPooter::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CPooter::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE)
	{
		releaseGroundInRigidBody(m_Collider->GetScale()
			, _OtherCollider->GetScale()
			, GetPos()
			, _OtherObject->GetPos()
			, m_RigidBody);
	}

	if(_OtherObject->GetLayerType() == LAYER_TYPE::PLAYER_OBJECT)
	{
		m_monsterFlipbook->SetIsHitted(true);
	}
}


void CPooter::CreatePooterFlipbook()
{
	m_monsterFlipbook = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);

	m_monsterFlipbook->AddFlipbook(POOTER_IDLE_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"POOTER_IDLE_LEFT", L"Flipbook\\POOTER_IDLE_RIGHT.flip"));
	m_monsterFlipbook->AddFlipbook(POOTER_IDLE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"POOTER_IDLE_RIGHT", L"Flipbook\\POOTER_IDLE_RIGHT.flip"));
	
	m_monsterFlipbook->AddFlipbook(POOTER_ATTACK_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"POOTER_ATTACK_LEFT", L"Flipbook\\POOTER_ATTACK_RIGHT.flip"));
	m_monsterFlipbook->AddFlipbook(POOTER_ATTACK_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"POOTER_ATTACK_RIGHT", L"Flipbook\\POOTER_ATTACK_RIGHT.flip"));
}

void CPooter::SetMaxSpeed(float _speed)
{
	m_RigidBody->SetMaxSpeed(_speed);
}