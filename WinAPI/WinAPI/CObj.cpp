#include "pch.h"
#include "CObj.h"

#include "CEngine.h"
#include "CComponent.h"
#include "CCamera.h"

CObj::CObj()
	: m_Pos{}
	, m_Scale{}
	, m_LayerType(LAYER_TYPE::END)
	, m_Dead(false)
{
}

CObj::~CObj()
{
	Delete_Vector(m_Component);
}

void CObj::Begin()
{
}

// PC 사양에 따라 Tick 함수 호출하는 횟수가 달라질 수 있음.
// ㄴ 움직임이나 속도가 PC 성능에 영향을 받기 때문에 맞춰줘야 함 (틱당 X > 초당 O)
	// GetAsyncKeyState 의 반환값이 0x0001 : 함수 호출 시점에는 안눌려있고, 그전까지는 눌린적이 있다.
	//								0x8000 : 함수 호출 시점에 눌려있고, 이전에는 안눌렸었다.
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8001)	// 함수 호출되기 전에 눌려있던것도 포함 ( 키 씹힘 방지 )
	//{
	//	float Speed = 200.f;
	//	m_Pos.x += Speed * DT;
	//}

void CObj::FinalTick()
{
	for (size_t i = 0; i < m_Component.size(); ++i)
	{
		m_Component[i]->FinalTick();
	}
}

void CObj::Render()
{
	HDC dc = CEngine::GetInst()->GetSecondDC();

	Vec2 vPos = GetRenderPos();

	Rectangle(dc
		, vPos.x - m_Scale.x / 2
		, vPos.y - m_Scale.y / 2
		, vPos.x + m_Scale.x / 2
		, vPos.y + m_Scale.y / 2);
}

CComponent* CObj::AddComponent(CComponent* _Component)
{
	m_Component.push_back(_Component);

	_Component->m_Owner = this;

	return _Component;
}

CComponent* CObj::GetComponent(const wstring& _Name)
{
	for (size_t i = 0; i < m_Component.size(); ++i)
	{
		if (m_Component[i]->GetName() == _Name)
			return m_Component[i];
	}

	return nullptr;
}

CComponent* CObj::GetComponent(COMPONENT_TYPE _Type)
{
	for (size_t i = 0; i < m_Component.size(); ++i)
	{
		if (m_Component[i]->GetType() == _Type)
			return m_Component[i];
	}

	return nullptr;
}

Vec2 CObj::GetRenderPos()
{
	return CCamera::GetInst()->GetRenderPos(m_Pos);
}