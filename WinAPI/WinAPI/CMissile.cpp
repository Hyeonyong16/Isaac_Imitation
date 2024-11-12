#include "pch.h"
#include "CMissile.h"

#include "CEngine.h"
#include "CTimeMgr.h"

#include "CCollider.h"
#include "CMonster.h"

CMissile::CMissile()
	: m_Mass(1.f)
	, m_VelocityScale(1.f)
	, m_curTime(0.f)
	, m_existTime(2.5f)
	, m_isDamaged(false)
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
	// �Ѿ��� ������ �ð��� ��ȿ�ð����� ��� ����
	m_curTime += DT;
	if (m_curTime >= m_existTime)
		DeleteObject(this);

	// ������ ���� ���� ���ܳ� ���ӵ� ���
	Vec2 vAccel = m_Force / m_Mass;

	// ���ӵ��� ���� �ӵ��� ��ȭ ����
	m_Velocity += vAccel * DT;

	// �ӵ��� ���� �̵�
	Vec2 vPos = GetPos();
	vPos += m_Velocity * m_VelocityScale * DT;
	SetPos(vPos);

	DrawDebugLine(PEN_TYPE::BLUE, GetRenderPos(), GetRenderPos() + m_Velocity * 0.1f, 0.f);

	// ���� �������� ���� ����
	m_Force = Vec2(0.f, 0.f);
}

void CMissile::Render()
{
	HDC dc = CEngine::GetInst()->GetSecondDC();

	Vec2 vPos = GetRenderPos();
	Vec2 vScale = GetScale();

	float offsetY = 47.f;
	if (m_curTime >= m_existTime - 0.2f)
	{
		offsetY -= (47.f - ((m_existTime - m_curTime) / 0.2f) * 47.f);
	}

	Ellipse(dc, vPos.x - vScale.x / 2.f, vPos.y - vScale.y / 2 - offsetY
		, vPos.x + vScale.x / 2.f, vPos.y + vScale.y / 2.f - offsetY);
}

void CMissile::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetLayerType() == LAYER_TYPE::TILE)
	{
		DeleteObject(this);
	}

	if(GetLayerType() == LAYER_TYPE::PLAYER_OBJECT)
	{
		if (_OtherObject->GetLayerType() == LAYER_TYPE::MONSTER)
		{
			if (!m_isDamaged)
			{
				CMonster* pMonster = dynamic_cast<CMonster*>(_OtherObject);
				assert(pMonster);
				if(pMonster->GetCanHit())
				{
					pMonster->DecreaseHP(1.f);
					m_isDamaged = true;
					DeleteObject(this);
				}
			}

		}
	}

	else if (GetLayerType() == LAYER_TYPE::MONSTER_OBJECT)
	{
		if (_OtherObject->GetLayerType() == LAYER_TYPE::PLAYER)
		{
			DeleteObject(this);
		}
	}
}

void CMissile::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CMissile::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

