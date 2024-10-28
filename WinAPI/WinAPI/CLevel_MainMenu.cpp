#include "pch.h"
#include "CLevel_MainMenu.h"

#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CSprite.h"

#include "CPanelUI.h"
#include "CBtnUI.h"

CLevel_MainMenu::CLevel_MainMenu()
{
	//m_MainMenuSprite = CAssetMgr::GetInst()->LoadSprite(L"MAIN_MENU", L"Sprite\\MAIN_MENU.sprite");
	//m_MainMenuTex = m_MainMenuSprite->GetAtlas();
}

CLevel_MainMenu::~CLevel_MainMenu()
{
}

void CLevel_MainMenu::Begin()
{
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	CPanelUI* pPanel = new CPanelUI;
	pPanel->SetName(L"Panel 1");
	Vec2 vScale = vResolution;

	pPanel->SetPos(0, 0);
	pPanel->SetScale(vScale);
	pPanel->SetSprite(CAssetMgr::GetInst()->LoadSprite(L"MAIN_MENU", L"Sprite\\MAIN_MENU.sprite"));

	// Panel 에 넣을 자식 UI
	CBtnUI* pBtn = new CBtnUI;
	pBtn->SetScale(Vec2(300.f, 100.f));
	pBtn->SetPos(Vec2(((vResolution.x / 2) - (pBtn->GetScale().x / 2)), 300.f));

	void ChangeStage1Level();
	pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_MainMenu::ChangeStage1Level);

	pPanel->AddChildUI(pBtn);

	pBtn = new CBtnUI;
	pBtn->SetScale(Vec2(300.f, 100.f));
	pBtn->SetPos(Vec2(((vResolution.x / 2) - (pBtn->GetScale().x / 2)), 450.f));

	void ChangeEditorLevel();
	pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_MainMenu::ChangeEditorLevel);

	pPanel->AddChildUI(pBtn);

	AddObject(pPanel, LAYER_TYPE::UI);

}

void CLevel_MainMenu::Tick()
{
	CLevel::Tick();

	if (KEY_TAP(KEY::M))
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		ChangeLevel(LEVEL_TYPE::EDITOR_TILE);
	}
}

void CLevel_MainMenu::Render()
{
	CLevel::Render();

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	HDC dc = CEngine::GetInst()->GetSecondDC();

	BLENDFUNCTION blend = {};

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	/*AlphaBlend(dc
		, 0
		, 0
		, vResolution.x
		, vResolution.y
		, m_MainMenuSprite->GetAtlas()->GetDC()
		, 0, 0
		, m_MainMenuSprite->GetSlice().x
		, m_MainMenuSprite->GetSlice().y
		, blend);*/



	/*TransparentBlt(dc
		, 0
		, 0
		, vResolution.x, vResolution.y
		, m_MainMenuTex->GetAtlas()->GetDC()
		, 0, 0, m_MainMenuTex->GetSlice().x, m_MainMenuTex->GetSlice().y
		, RGB(255, 0, 255));*/
}

void CLevel_MainMenu::End()
{
	DeleteAllObject();

}

void CLevel_MainMenu::ChangeStage1Level()
{
	ChangeLevel(LEVEL_TYPE::STAGE_0);
}

void CLevel_MainMenu::ChangeEditorLevel()
{
	ChangeLevel(LEVEL_TYPE::EDITOR_TILE);
}

//void CLevel_MainMenu::ChangeLevel(LEVEL_TYPE _level)
//{
//}

