#include "pch.h"
#include "CPlatform.h"

#include "CCollider.h"
#include "CPlayer.h"
#include "CRigidBody.h"

#include "CCamera.h"

CPlatform::CPlatform()
	: m_Collider(nullptr)
{
	m_Collider = (CCollider*)AddComponent(new CCollider);
	m_Collider->SetScale(Vec2(700.f, 150.f));
}

CPlatform::~CPlatform()
{
}

void CPlatform::Tick()
{
}

void CPlatform::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CRigidBody* pBody = _OtherObject->GetComponent<CRigidBody>();
		pBody->SetGround(true);
		CCamera::GetInst()->Oscillation(0.15f, 5.f, 10.f);
	}
}

void CPlatform::Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

void CPlatform::EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CRigidBody* pBody = _OtherObject->GetComponent<CRigidBody>();
		pBody->SetGround(false);
	}
}