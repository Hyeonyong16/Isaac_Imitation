#include "pch.h"
#include "CRock.h"

#include "CCollider.h"
#include "CPlayer.h"
#include "CRigidBody.h"

#include "CAssetMgr.h"
#include "CSprite.h"
#include "CEngine.h"

#include "CCamera.h"

CRock::CRock()
	: m_Collider(nullptr)
	, m_Sprite(nullptr)
{
	m_Collider = (CCollider*)AddComponent(new CCollider);
	m_Collider->SetScale(Vec2(103.f, 103.f));

	m_Sprite = CAssetMgr::GetInst()->LoadSprite(L"ROCK", L"Sprite\\ROCK.sprite");
}

CRock::~CRock()
{
}

void CRock::Begin()
{
}

void CRock::Tick()
{
}

void CRock::Render()
{
	HDC hBackDC = CEngine::GetInst()->GetSecondDC();

	BLENDFUNCTION blend = {};

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	Vec2 renderPos = CCamera::GetInst()->GetRenderPos(GetPos());

	AlphaBlend(hBackDC
		, renderPos.x - (GetScale().x / 2)
		, renderPos.y - (GetScale().y / 2)
		, GetScale().x
		, GetScale().y
		, m_Sprite->GetAtlas()->GetDC()
		, m_Sprite->GetLeftTop().x
		, m_Sprite->GetLeftTop().y
		, m_Sprite->GetSlice().x
		, m_Sprite->GetSlice().y
		, blend);
}

void CRock::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CRock::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CRock::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CRock::SetColliderScale(Vec2 _scale)
{
	m_Collider->SetScale(_scale);
}
