#include "pch.h"
#include "CPanelUI.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CSelectGDI.h"

#include "CSprite.h"

CPanelUI::CPanelUI()
	: m_sprite(nullptr)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::Tick_UI()
{;
	if(IsLBtnDown())
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vDiff = vMousePos - m_DownPos;

		Vec2 vPos = GetPos();
		vPos += vDiff;
		SetPos(vPos);

		m_DownPos = vMousePos;

		// 임의로 확인을 위해서 넣은 부분
		m_vMousePosTemp = vMousePos;
		m_vDiffTemp = vDiff;
		m_vPosTemp = vPos;
	}

}

void CPanelUI::Render_UI()
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if(!m_sprite)
	{
		SELECT_PEN(PEN_TYPE::GREEN);

		HBRUSH hBrush = CreateSolidBrush(RGB(143, 90, 145));
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
			, 0, 0
			, m_sprite->GetSlice().x
			, m_sprite->GetSlice().y
			, blend);
	}

}

void CPanelUI::MouseLBtnDown()
{
	m_DownPos = CKeyMgr::GetInst()->GetMousePos();
}

void CPanelUI::MouseLBtnClikced()
{
}

