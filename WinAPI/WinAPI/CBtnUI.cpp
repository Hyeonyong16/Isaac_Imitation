#include "pch.h"
#include "CBtnUI.h"

#include "CEngine.h"
#include "CSelectGDI.h"

#include "CLevelMgr.h"
#include "CLevel_Editor.h"

#include "CSprite.h"

CBtnUI::CBtnUI()
	: m_Func(nullptr)
	, m_Func1(nullptr)
	, m_sprite(nullptr)
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

	if(!m_sprite)
	{
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
	else
	{
		HDC dc = CEngine::GetInst()->GetSecondDC();

		BLENDFUNCTION blend = {};

		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(dc
			, vPos.x
			, vPos.y
			, vScale.x
			, vScale.y
			, m_sprite->GetAtlas()->GetDC()
			, m_sprite->GetLeftTop().x, m_sprite->GetLeftTop().y
			, m_sprite->GetSlice().x
			, m_sprite->GetSlice().y
			, blend);
	}
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
