#include "pch.h"
#include "CUI.h"

#include "CEngine.h"
#include "CSelectGDI.h"

#include "CKeyMgr.h"

CUI::CUI()
	: m_ParentUI(nullptr)
	, m_MouseHover(false)
	, m_LBtnDown(false)
{
}

CUI::~CUI()
{
	Delete_Vector(m_vecChildUI);
}

void CUI::Tick()
{
	// 최종 좌표 계산하기
	m_FinalPos = GetPos();

	if (m_ParentUI)
	{
		m_FinalPos += m_ParentUI->GetFinalPos();
	}

	// 마우스가 UI 위에 올라왔는지 체크하기
	MouseHoverCheck();

	// 실제 UI 가 추가적으로 할일
	Tick_UI();

	// 자식 UI Tick 호출
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Tick();
	}
}

void CUI::Render()
{
	// UI 본인 렌더링
	Render_UI();

	// 자식 UI Render 호출
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render();
	}
}

void CUI::Render_UI()
{
	// GetPos() : 부모 UI 로부터 상대 좌표, 부모가 없는 UI 경우 최종 좌표
	// GetFinalPos() : 최종 좌표, 부모의 위치가 누적되고 거기에 자신의 상대좌표를 더한 값
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	SELECT_PEN(PEN_TYPE::GREEN);
	SELECT_BRUSH(BRUSH_TYPE::HOLLOW);

	Rectangle(CEngine::GetInst()->GetSecondDC()
		, (int)vPos.x, (int)vPos.y
		, (int)(vPos.x + vScale.x)
		, (int)(vPos.y + vScale.y));
}

void CUI::MouseHoverCheck()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (vPos.x <= vMousePos.x && vMousePos.x <= vPos.x + vScale.x
		&& vPos.y <= vMousePos.y && vMousePos.y <= vPos.y + vScale.y)
	{
		m_MouseHover = true;
	}
	else
	{
		m_MouseHover = false;
	}
}