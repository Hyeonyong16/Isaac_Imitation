#include "pch.h"
#include "CBtnUI.h"

#include "CEngine.h"
#include "CSelectGDI.h"

#include "CLevelMgr.h"
#include "CLevel_Editor.h"

CBtnUI::CBtnUI()
	: m_Func(nullptr)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::Tick_UI()
{
}

void CBtnUI::Render_UI()
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	SELECT_PEN(PEN_TYPE::GREEN);

	HBRUSH hBrush = CreateSolidBrush(RGB(143, 36, 32));
	HDC dc = CEngine::GetInst()->GetSecondDC();

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(dc, hBrush);

	Rectangle(dc
		, (int)vPos.x, (int)vPos.y
		, (int)(vPos.x + vScale.x)
		, (int)(vPos.y + vScale.y));

	SelectObject(dc, hPrevBrush);
	DeleteObject(hBrush);
}

void CBtnUI::MouseLBtnClikced()
{
	/*CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

	if (nullptr == pEditorLevel)
		return;

	pEditorLevel->SaveTileMap();*/

	if (nullptr != m_Func)
		m_Func();

	if (m_Inst && m_MemFunc)
	{
		(m_Inst->*m_MemFunc)();
	}
}
