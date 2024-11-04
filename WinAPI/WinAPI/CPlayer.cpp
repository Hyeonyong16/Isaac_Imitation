#include "pch.h"
#include "CPlayer.h"

#include "CCamera.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTaskMgr.h"
#include "CDbgRender.h"

#include "CMissile.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CMonster.h"
#include "CCollider.h"
#include "CGuidedMissile.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CSprite.h"
#include "CFlipbook.h"
#include "CFlipbookPlayer.h"
#include "CRigidBody.h"

#include "CFSM.h"
#include "CPlayerIdleState.h"
#include "CPlayerMoveState.h"
#include "CPlayerAttackState.h"

enum PLAYER_ANIM_STATE
{
	IDLE_UP,
	IDLE_DOWN,
	IDLE_LEFT,
	IDLE_RIGHT,

	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
};

//enum PLAYER_HEAD_ANIM_STATE
//{
//	ISAAC_HEAD_UP,
//	ISAAC_HEAD_DOWN,
//	ISAAC_HEAD_LEFT,
//	ISAAC_HEAD_RIGHT,
//};

CPlayer::CPlayer()
	: m_Speed(200.f)
	, m_AttSpeed(2.f)
	, m_AccTime(0.f)
	, m_HitBox(nullptr)
	, m_FlipbookBody(nullptr)
	, m_FlipbookHead(nullptr)
	//, m_Texture(nullptr)
	, m_RigidBody(nullptr)
	, isAttack{false, false, false, false}
	, m_headDir('D')
	, m_bodyDir('D')
	, m_FSM(nullptr)
	, m_isDamaged(false)
	, m_isAttacking(false)
{
	m_HitBox = new CCollider;
	m_HitBox->SetName(L"HitBox_01");
	m_HitBox->SetScale(Vec2(60.f, 60.f));
	m_HitBox->SetOffset(Vec2(0.f, 0.f));

	AddComponent(m_HitBox);

	// Flipbook 생성 및 등록
	CreatePlayerFlipbook();
	m_FlipbookHead->SetName(L"Player_Flipbook_Head");
	m_FlipbookHead->SetRenderSize(Vec2(60.f, 60.f));
	m_FlipbookHead->SetRenderOffset(Vec2(0.f, -47.f));

	m_FlipbookBody->SetName(L"Player_Flipbook_Body");
	m_FlipbookBody->SetRenderSize(Vec2(30.f, 30.f));
	m_FlipbookBody->SetRenderOffset(Vec2(0.f, -15.f));

	// RigidBody 컴포넌트 추가
	m_RigidBody = (CRigidBody*)AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetInitialSpeed(200.f);
	m_RigidBody->SetMaxSpeed(400.f);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetFriction(1000.f);
	//m_RigidBody->SetJumpVelocity(Vec2(0.f, -500.f));

	m_FSM = (CFSM*)AddComponent(new CFSM);

	m_FSM->AddState(L"Idle", new CPlayerIdleState);
	m_FSM->AddState(L"Move", new CPlayerMoveState);
	m_FSM->AddState(L"Attack", new CPlayerAttackState);
}

CPlayer::~CPlayer()
{
	//DELETE(m_Texture);
}

void CPlayer::Begin()
{
	m_AccTime = 1.f / m_AttSpeed;

	// 공격 키입력 관련 초기화
	AttackQueue.clear();
	for (int i = 0; i < 4; ++i)
		isAttack[i] = false;

	m_FSM->ChangeState(L"Idle");
	//m_FlipbookHead->Play(ISAAC_HEAD_DOWN, 5.f, true);

	//CCamera::GetInst()->SetTarget(this);
}

void CPlayer::Tick()
{

	// 이동
	{
		//if (KEY_TAP(A))
		//{
		//	m_FlipbookPlayer->Play(MOVE_LEFT, 15.f, true);
		//}
		//if (KEY_TAP(D))
		//{
		//	m_FlipbookPlayer->Play(MOVE_RIGHT, 15.f, true);
		//}
		//if (KEY_TAP(W))
		//{
		//	m_FlipbookPlayer->Play(MOVE_UP, 15.f, true);
		//}
		//if (KEY_TAP(S))
		//{
		//	m_FlipbookPlayer->Play(MOVE_DOWN, 15.f, true);
		//}

		//if (KEY_RELEASED(A))
		//	m_FlipbookPlayer->Play(IDLE_LEFT, 5.f, true);
		//if (KEY_RELEASED(D))
		//	m_FlipbookPlayer->Play(IDLE_RIGHT, 5.f, true);
		//if (KEY_RELEASED(W))
		//	m_FlipbookPlayer->Play(IDLE_UP, 5.f, true);
		//if (KEY_RELEASED(S))
		//	m_FlipbookPlayer->Play(IDLE_DOWN, 5.f, true);


		// 키 두개를 동시 입력 시 속도가 더 빨라지는것을 막기 위한 용도
		if (!KEY_PRESSED(A) && !KEY_PRESSED(D) && !KEY_PRESSED(W) && !KEY_PRESSED(S))
		{
			//m_RigidBody->SetVelocity(Vec2(0, 0));
		}

		Vec2 tempForce(0, 0);

		if (KEY_PRESSED(A))
		{
			tempForce += Vec2(-1.f, 0.f);
			SetHeadBodyDir('L', 'L');
		}
		if (KEY_PRESSED(D))
		{
			tempForce += Vec2(1.f, 0.f);
			SetHeadBodyDir('R', 'R');
		}
		if (KEY_PRESSED(W))
		{
			tempForce += Vec2(0.f, -1.f);
			SetHeadBodyDir('U', 'U');
		}
		if (KEY_PRESSED(S))
		{
			tempForce += Vec2(0.f, 1.f);
			SetHeadBodyDir('D', 'D');
		}

		// 정규 화 후 해당 방향으로 1000 만큼 곱해서 addforce
		if (tempForce != Vec2(0.f, 0.f))
		{
			tempForce.Normalize();
			tempForce *= 2000;	// 가속도
			m_RigidBody->AddForce(tempForce, true);
		}
	}

	// 공격
	// 키 입력 중복 처리
	{
		if (KEY_TAP(LEFT) || KEY_PRESSED(LEFT))
		{
			if (!isAttack[0])
			{
				isAttack[0] = true;
				AttackQueue.push_back('L');
			}
		}
		if (KEY_TAP(RIGHT) || KEY_PRESSED(RIGHT))
		{
			if (!isAttack[1])
			{
				isAttack[1] = true;
				AttackQueue.push_back('R');
			}
		}
		if (KEY_TAP(UP) || KEY_PRESSED(UP))
		{
			if (!isAttack[2])
			{
				isAttack[2] = true;
				AttackQueue.push_back('U');
			}
		}
		if (KEY_TAP(DOWN) || KEY_PRESSED(DOWN))
		{
			if (!isAttack[3])
			{
				isAttack[3] = true;
				AttackQueue.push_back('D');
			}
		}

		if (KEY_RELEASED(LEFT))
		{
			isAttack[0] = false;
			AttackQueue.remove('L');
		}
		if (KEY_RELEASED(RIGHT))
		{
			isAttack[1] = false;
			AttackQueue.remove('R');
		}
		if (KEY_RELEASED(UP))
		{
			isAttack[2] = false;
			AttackQueue.remove('U');
		}
		if (KEY_RELEASED(DOWN))
		{
			isAttack[3] = false;
			AttackQueue.remove('D');
		}


		if (AttackQueue.size() > 4)
			int a = 0;
	}
	

	{
		if (AttackQueue.size() > 0)
		{
			m_isAttacking = false;
			if (!m_AccTime || (1.f / m_AttSpeed <= m_AccTime))
			{
				if (1.f / m_AttSpeed <= m_AccTime)
					m_AccTime -= 1.f / m_AttSpeed;

				Vec2 shotPos(0, 0);
				Vec2 shotVel(0, 0);
				switch (AttackQueue.front())
				{
				case 'L':
				{
					shotPos = GetPos() + Vec2(-GetScale().x / 2.f, 0.f);
					shotVel = Vec2(cosf(PI), sinf(PI)) * 200.f;
					m_headDir = 'L';
					//m_FlipbookHead->Play(ISAAC_HEAD_LEFT, 5.f, true);
				}
				break;
				case 'R':
				{
					shotPos = GetPos() + Vec2(GetScale().x / 2.f, 0.f);
					shotVel = Vec2(-cosf(PI), sinf(PI)) * 200.f;
					m_headDir = 'R';
					//m_FlipbookHead->Play(ISAAC_HEAD_RIGHT, 5.f, true);
				}
				break;
				case 'U':
				{
					shotPos = GetPos() + Vec2(0.f, -GetScale().y / 2.f);
					shotVel = Vec2(cosf(PI / 2.f), -sinf(PI / 2.f)) * 200.f;
					m_headDir = 'U';
					//m_FlipbookHead->Play(ISAAC_HEAD_UP, 5.f, true);
				}
				break;
				case 'D':
				{
					shotPos = GetPos() + Vec2(0.f, GetScale().y / 2.f);
					shotVel = Vec2(cosf(PI / 2.f), sinf(PI / 2.f)) * 200.f;
					m_headDir = 'D';
					//m_FlipbookHead->Play(ISAAC_HEAD_DOWN, 5.f, true);
				}
				break;
				}

				CMissile* pMissile = new CGuidedMissile;
				pMissile->SetPos(shotPos);
				pMissile->SetScale(20.f, 20.f);
				pMissile->SetVelocity(shotVel);
				CreateObject(pMissile, LAYER_TYPE::PLAYER_OBJECT);
				m_isAttacking = true;
			}

			m_AccTime += DT;
		}
		else
		{
			if (m_AccTime != 0.f)
			{
				m_AccTime += DT;
				if (1.f / m_AttSpeed <= m_AccTime)
					m_AccTime = 0.f;
			}
		}
	}



	//if (KEY_TAP(SPACE))
	//{
	//	CCamera::GetInst()->PostProcessEffect(HEART, 0.2f);
	//	//m_RigidBody->Jump();
	//	//DrawDebugRect(PEN_TYPE::GREEN, GetPos(), GetScale() * 2.f, 3.f);
	//	//DrawDebugCircle(PEN_TYPE::GREEN, GetPos(), GetScale() * 2.f, 3.f);
	//	//DrawDebugLine(PEN_TYPE::GREEN, GetPos(), GetPos() + GetScale(), 3.f);
	//}

}

void CPlayer::Render()
{
	//m_FlipbookPlayer->Render();
	m_FlipbookBody->Render();
	m_FlipbookHead->Render();


	DrawDebugRect(PEN_TYPE::GREEN, Vec2(0.f, -60.f), Vec2(60.f, 60.f), 0.f);
	DrawDebugRect(PEN_TYPE::GREEN, Vec2(0.f, 0.f), Vec2(60.f, 60.f), 0.f);


	wchar_t str1[255];
	wchar_t str2[255];
	wchar_t str3[255];
	swprintf_s(str1, 255, L"pos x: %d, y: %d", (int)GetPos().x, (int)GetPos().y);
	swprintf_s(str2, 255, L"AccTime: %f", m_AccTime);
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 30, str1, wcslen(str1));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 50, str2, wcslen(str2));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 70, m_FSM->GetCurState().c_str(), wcslen(m_FSM->GetCurState().c_str()));
}

void CPlayer::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Monster")
	{
		//DeleteObject(_OtherObject);
	}
}

void CPlayer::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CPlayer::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CPlayer::CreatePlayerFlipbook()
{
	//CTexture* pAtlas = CAssetMgr::GetInst()->LoadTexture(L"Link", L"Texture\\link_32.bmp");

	// Flipbook 생성
	/*CreateFlipbook(L"LINK_IDLEDOWN", pAtlas, Vec2(0.f, 0.f), Vec2(120.f, 130.f), 3);
	CreateFlipbook(L"LINK_IDLELEFT",  pAtlas, Vec2(0.f, 130.f), Vec2(120.f, 130.f), 3);
	CreateFlipbook(L"LINK_IDLEUP",	  pAtlas, Vec2(0.f, 260.f), Vec2(120.f, 130.f), 1);
	CreateFlipbook(L"LINK_IDLERIGHT", pAtlas, Vec2(0.f, 390.f), Vec2(120.f, 130.f), 3);

	CreateFlipbook(L"LINK_MOVEDOWN",  pAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), 10);
	CreateFlipbook(L"LINK_MOVELEFT",  pAtlas, Vec2(0.f, 650.f), Vec2(120.f, 130.f), 10);
	CreateFlipbook(L"LINK_MOVEUP",    pAtlas, Vec2(0.f, 780.f), Vec2(120.f, 130.f), 10);
	CreateFlipbook(L"LINK_MOVERIGHT", pAtlas, Vec2(0.f, 910.f), Vec2(120.f, 130.f), 10);*/

	// FlipbookPlayer 컴포넌트 추가하기
	//m_FlipbookPlayer = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);
	m_FlipbookHead = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);
	m_FlipbookBody = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);

	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_LEFT", L"Flipbook\\ISAAC_HEAD_LEFT.flip"));
	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_RIGHT", L"Flipbook\\ISAAC_HEAD_RIGHT.flip"));
	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_UP, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_BACK", L"Flipbook\\ISAAC_HEAD_BACK.flip"));
	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_FRONT", L"Flipbook\\ISAAC_HEAD_FRONT.flip"));

	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_IDLE_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_IDLE_LEFT", L"Flipbook\\ISAAC_HEAD_IDLE_LEFT.flip"));
	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_IDLE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_IDLE_RIGHT", L"Flipbook\\ISAAC_HEAD_IDLE_RIGHT.flip"));
	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_IDLE_UP, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_IDLE_BACK", L"Flipbook\\ISAAC_HEAD_IDLE_BACK.flip"));
	m_FlipbookHead->AddFlipbook(ISAAC_HEAD_IDLE_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_HEAD_IDLE_FRONT", L"Flipbook\\ISAAC_HEAD_IDLE_FRONT.flip"));

	m_FlipbookBody->AddFlipbook(ISAAC_BODY_MOVE_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_BODY_MOVE_FRONT", L"Flipbook\\ISAAC_BODY_MOVE_FRONT.flip"));
	m_FlipbookBody->AddFlipbook(ISAAC_BODY_MOVE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_BODY_MOVE_RIGHT", L"Flipbook\\ISAAC_BODY_MOVE_RIGHT.flip"));
	m_FlipbookBody->AddFlipbook(ISAAC_BODY_MOVE_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_BODY_MOVE_LEFT", L"Flipbook\\ISAAC_BODY_MOVE_RIGHT.flip"));

	m_FlipbookBody->AddFlipbook(ISAAC_BODY_IDLE_DOWN, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_BODY_IDLE_FRONT", L"Flipbook\\ISAAC_BODY_IDLE_FRONT.flip"));
	m_FlipbookBody->AddFlipbook(ISAAC_BODY_IDLE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_BODY_IDLE_RIGHT", L"Flipbook\\ISAAC_BODY_IDLE_RIGHT.flip"));
	m_FlipbookBody->AddFlipbook(ISAAC_BODY_IDLE_LEFT, CAssetMgr::GetInst()->LoadFlipbook(L"ISAAC_BODY_IDLE_LEFT", L"Flipbook\\ISAAC_BODY_IDLE_RIGHT.flip"));

	// FlipbookPlayer 컴포넌트에 Flipbook 추가
	//m_FlipbookPlayer->AddFlipbook(IDLE_DOWN,  CAssetMgr::GetInst()->LoadFlipbook(L"LINK_IDLEDOWN",  L"Flipbook\\LINK_IDLEDOWN.flip"));
	//m_FlipbookPlayer->AddFlipbook(IDLE_LEFT,  CAssetMgr::GetInst()->LoadFlipbook(L"LINK_IDLELEFT",  L"Flipbook\\LINK_IDLELEFT.flip"));
	//m_FlipbookPlayer->AddFlipbook(IDLE_UP,    CAssetMgr::GetInst()->LoadFlipbook(L"LINK_IDLEUP",    L"Flipbook\\LINK_IDLEUP.flip"));
	//m_FlipbookPlayer->AddFlipbook(IDLE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"LINK_IDLERIGHT", L"Flipbook\\LINK_IDLERIGHT.flip"));
																	
	//m_FlipbookPlayer->AddFlipbook(MOVE_DOWN,  CAssetMgr::GetInst()->LoadFlipbook(L"LINK_MOVEDOWN",  L"Flipbook\\LINK_MOVEDOWN.flip"));
	//m_FlipbookPlayer->AddFlipbook(MOVE_LEFT,  CAssetMgr::GetInst()->LoadFlipbook(L"LINK_MOVELEFT",  L"Flipbook\\LINK_MOVELEFT.flip"));
	//m_FlipbookPlayer->AddFlipbook(MOVE_UP,    CAssetMgr::GetInst()->LoadFlipbook(L"LINK_MOVEUP",    L"Flipbook\\LINK_MOVEUP.flip"));
	//m_FlipbookPlayer->AddFlipbook(MOVE_RIGHT, CAssetMgr::GetInst()->LoadFlipbook(L"LINK_MOVERIGHT", L"Flipbook\\LINK_MOVERIGHT.flip"));

	//CSprite* pSprite = CAssetMgr::GetInst()->FindSprite(L"LINK_IDLERIGHT_1");
	//pSprite->SetOffset(Vec2(1.f, 0.f));
}

void CPlayer::CreateFlipbook(const wstring& _FlipbookName, CTexture* _Atlas, Vec2 _LeftTop, Vec2 _Slice, int MaxFrame)
{
	// Sprite 생성하기
	/*for (int i = 0; i < MaxFrame; ++i)
	{
		CSprite* pSprite = new CSprite;
		pSprite->Create(_Atlas, Vec2(_LeftTop.x + (_Slice.x * i), _LeftTop.y), _Slice);

		wchar_t Key[50] = {};
		swprintf_s(Key, 50, (_FlipbookName + L"_%d").c_str(), i);
		CAssetMgr::GetInst()->AddSprite(Key, pSprite);

		wstring strSavePath = L"Sprite\\";
		strSavePath += pSprite->GetKey();
		pSprite->Save(strSavePath);
	}*/

	// Sprite 불러오기
	for (int i = 0; i < MaxFrame; ++i)
	{
		wchar_t Key[50] = {};
		swprintf_s(Key, 50, (_FlipbookName + L"_%d").c_str(), i);
		wstring Path = L"Sprite\\";
		Path += Key;
		CAssetMgr::GetInst()->LoadSprite(Key, Path);
	}

	// Flipbook 생성하기
	/*CFlipbook* pFlipbook = new CFlipbook;

	for (int i = 0; i < MaxFrame; ++i)
	{
		wchar_t Key[50] = {};
		swprintf_s(Key, 50, (_FlipbookName + L"_%d").c_str(), i);
		pFlipbook->AddSprite(CAssetMgr::GetInst()->FindSprite(Key));
	}

	CAssetMgr::GetInst()->AddFlipbook(_FlipbookName, pFlipbook);
	wstring Path = L"Flipbook\\";
	pFlipbook->Save(Path + _FlipbookName);*/
}

Vec2 CPlayer::GetVelocity()
{
	return m_RigidBody->GetVelocity();
}
