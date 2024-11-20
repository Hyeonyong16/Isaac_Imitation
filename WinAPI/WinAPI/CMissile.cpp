#include "pch.h"
#include "CMissile.h"

#include "CEngine.h"
#include "CTimeMgr.h"

#include "CCollider.h"
#include "CMonster.h"

#include "CAssetMgr.h"
#include "CFlipbookPlayer.h"

CMissile::CMissile()
	: m_Mass(1.f)
	, m_VelocityScale(1.f)
	, m_curTime(0.f)
	, m_existTime(2.5f)
	, m_isDamaged(false)
	, m_isDeletePlay(false)
	, m_BulletFlipbook(nullptr)
{
	m_Collider = (CCollider*)AddComponent(new CCollider);
	m_Collider->SetScale(Vec2(15.f, 15.f));

	m_Velocity = Vec2(1.f, 1.f);
	m_Velocity.Normalize();
	m_Velocity *= 500.f;

	m_BulletFlipbook = (CFlipbookPlayer*)AddComponent(new CFlipbookPlayer);
	m_BulletFlipbook->AddFlipbook(BULLET_IDLE, CAssetMgr::GetInst()->LoadFlipbook(L"BULLET_IDLE", L"Flipbook\\BULLET_IDLE.flip"));
	m_BulletFlipbook->AddFlipbook(BULLET_DESTROY, CAssetMgr::GetInst()->LoadFlipbook(L"BULLET_DESTROY", L"Flipbook\\BULLET_DESTROY.flip"));
	m_BulletFlipbook->SetRenderSize(Vec2(25.f, 25.f));
}

CMissile::~CMissile()
{
}

void CMissile::Tick()
{
	// 총알이 존재한 시간이 유효시간보다 길면 삭제
	m_curTime += DT;
	if (m_curTime >= m_existTime)
		m_isDeleted = true;
		if((m_BulletFlipbook->GetPlayFlipbookIdx() == BULLET_DESTROY) && m_BulletFlipbook->isFinish())
			DeleteObject(this);

	if(!m_isDeleted)
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

	if (m_isDeleted)
	{
		m_BulletFlipbook->SetRenderSize(Vec2(64.f, 64.f));
		if(!m_isDeletePlay)
		{
			if (m_BulletFlipbook->GetPlayFlipbookIdx() != BULLET_DESTROY)
				m_BulletFlipbook->Play(BULLET_DESTROY, 15.f, false);
			m_isDeletePlay = true;
		}
		else
		{
			if (m_BulletFlipbook->isFinish())
				DeleteObject(this);
		}
	}

	else
	{
		if (m_BulletFlipbook->GetPlayFlipbookIdx() != BULLET_IDLE)
			m_BulletFlipbook->Play(BULLET_IDLE, 1.f, false);
	}

}

void CMissile::Render()
{
	HDC dc = CEngine::GetInst()->GetSecondDC();

	Vec2 vPos = GetRenderPos();
	Vec2 vScale = GetScale();

	float offsetY = 47.f;
	if (m_curTime >= m_existTime - 0.2f)
	{
		if (!m_isDeleted)
		{
			if(m_Velocity.y <= 0.f)
				offsetY -= (47.f - ((m_existTime - m_curTime) / 0.2f) * 47.f);
		}

		else
			offsetY = 0.f;
	}
		
	if (m_Velocity.y <= 0.f)
		m_BulletFlipbook->SetRenderOffset(Vec2(0.f, -offsetY));
	else
		m_BulletFlipbook->SetRenderOffset(Vec2(0.f, 0.f));
	
	m_BulletFlipbook->Render();

	/*Ellipse(dc, vPos.x - vScale.x / 2.f, vPos.y - vScale.y / 2 - offsetY
		, vPos.x + vScale.x / 2.f, vPos.y + vScale.y / 2.f - offsetY);*/
}

void CMissile::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE)
	{
		if(!m_isDeleted)
			m_isDeleted = true;
		//DeleteObject(this);
	}

	if(GetLayerType() == LAYER_TYPE::PLAYER_OBJECT)
	{
		if (_OtherObject->GetLayerType() == LAYER_TYPE::MONSTER)
		{
			if (!m_isDamaged)
			{
				CMonster* pMonster = dynamic_cast<CMonster*>(_OtherObject);
				assert(pMonster);
				if(pMonster->GetCanHit() && !m_isDamaged)
				{
					pMonster->DecreaseHP(1.f);
					m_isDamaged = true;
					m_isDeleted = true;
					//DeleteObject(this);
				}
			}

		}
	}

	else if (GetLayerType() == LAYER_TYPE::MONSTER_OBJECT)
	{
		if (_OtherObject->GetLayerType() == LAYER_TYPE::PLAYER)
		{
			if(!m_isDeleted)
				m_isDeleted = true;
			//DeleteObject(this);
		}
	}
}

void CMissile::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CMissile::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

