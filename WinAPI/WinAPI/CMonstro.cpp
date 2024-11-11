#include "pch.h"
#include "CMonstro.h"

#include "CEngine.h"
#include "CFSM.h"
#include "CAssetMgr.h"

#include "CFlipbookPlayer.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CTimeMgr.h"

#include "CDbgRender.h"
#include "CMonstroIdleState.h"
#include "CMonstroAttackState.h"
#include "CMonstroChaseState.h"
#include "CMonstroJumpState.h"
#include "CMonstroDeathState.h"

CMonstro::CMonstro()
	: m_isJumping(false)
	, m_isGround(true)
	, m_isChaseToJump(false)
	, m_JumpCount(0)
	, m_curPattern(0)
	, m_monsterFlipbook(nullptr)
{
	m_Collider = new CCollider;
	m_Collider->SetName(L"Monstro_HitBox_01");
	m_Collider->SetScale(Vec2(150.f, 150.f));
	m_Collider->SetOffset(Vec2(0.f, 0.f));

	AddComponent(m_Collider);

	tMonInfo temp;
	temp.MaxHP = 60.f;
	temp.CurHP = 60.f;
	temp.Speed = 200.f;

	SetMonInfo(temp);

	CreateMonstroFlipbook();
	m_monsterFlipbook->SetName(L"Monstro_Flipbook");
	m_monsterFlipbook->SetRenderSize(Vec2(150.f, 150.f));
	m_monsterFlipbook->SetRenderOffset(Vec2(0.f, -75.f));


	m_FSM = (CFSM*)AddComponent(new CFSM);

	m_FSM->AddState(L"Idle", new CMonstroIdleState);
	m_FSM->AddState(L"Jump", new CMonstroJumpState);
	m_FSM->AddState(L"Chase", new CMonstroChaseState);

	// RigidBody 컴포넌트 추가
	m_RigidBody = (CRigidBody*)AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetInitialSpeed(200.f);
	m_RigidBody->SetMaxSpeed(200.f);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetFriction(1000.f);
}

CMonstro::~CMonstro()
{
}

void CMonstro::Begin()
{
	CMonster::Begin();

	m_FSM->ChangeState(L"Idle");
}

void CMonstro::Tick()
{
}

void CMonstro::Render()
{
	m_monsterFlipbook->Render();

	wchar_t str[255];
	swprintf_s(str, 255, L"HP: %d", (int)GetMonInfo().CurHP);
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 170, str, wcslen(str));

	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 70, m_FSM->GetCurState().c_str(), wcslen(m_FSM->GetCurState().c_str()));
}

void CMonstro::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{

}

void CMonstro::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CMonstro::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CMonstro::CreateMonstroFlipbook()
{
	m_monsterFlipbook = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);

	m_monsterFlipbook->AddFlipbook(MONSTRO_IDLE, CAssetMgr::GetInst()->LoadFlipbook(L"MONSTRO_IDLE", L"Flipbook\\MONSTRO_IDLE.flip"));
	m_monsterFlipbook->AddFlipbook(MONSTRO_JUMP, CAssetMgr::GetInst()->LoadFlipbook(L"MONSTRO_JUMP", L"Flipbook\\MONSTRO_JUMP.flip"));
	m_monsterFlipbook->AddFlipbook(MONSTRO_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"MONSTRO_DOWN", L"Flipbook\\MONSTRO_DOWN.flip"));
	m_monsterFlipbook->AddFlipbook(MONSTRO_CHASE, CAssetMgr::GetInst()->LoadFlipbook(L"MONSTRO_CHASE", L"Flipbook\\MONSTRO_CHASE.flip"));
}