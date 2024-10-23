#include "pch.h"
#include "CPanelUI.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CSelectGDI.h"

CPanelUI::CPanelUI()
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


	// 임의로 확인을 위해서 넣은 부분
	wchar_t str[255];
	wchar_t str1[255];
	wchar_t str2[255];
	wchar_t str3[255];
	swprintf_s(str, 255, L"vMousePos x: %d, y: %d"
		, (int)m_vMousePosTemp.x, (int)m_vMousePosTemp.y);

	swprintf_s(str1, 255, L"vDiff x: %d, y: %d"
		, (int)m_vDiffTemp.x, (int)m_vDiffTemp.y);

	swprintf_s(str2, 255, L"vPos x: %d, y: %d"
		, (int)m_vPosTemp.x, (int)m_vPosTemp.y);

	swprintf_s(str3, 255, L"m_DownPos x: %d, y: %d"
		, (int)m_DownPos.x, (int)m_DownPos.y);

	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 30, str, wcslen(str));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 50, str1, wcslen(str1));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 70, str2, wcslen(str2));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 90, str3, wcslen(str3));
}

void CPanelUI::MouseLBtnDown()
{
	m_DownPos = CKeyMgr::GetInst()->GetMousePos();
}

void CPanelUI::MouseLBtnClikced()
{
}

