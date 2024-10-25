#include "pch.h"
#include "CLevel_Editor_Sprite.h"

#include "resource.h"

#include "CEngine.h"
#include "CKeyMgr.h"
#include "CMap.h"
#include "CTileMap.h"

#include "CLevelMgr.h"
#include "CCamera.h"

#include "CPathMgr.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CAssetMgr.h"
#include "CSound.h"

#include "CSelectGDI.h"
#include "CSprite.h"


CLevel_Editor_Sprite::CLevel_Editor_Sprite()
	: m_AtlasTexture(nullptr)
	, m_hMenu(nullptr)
	, m_bSpriteMenu(false)
	, m_SpritePos{}
	, m_SpriteScale{}
	, m_bDrawSprite(false)
{
	
}

CLevel_Editor_Sprite::~CLevel_Editor_Sprite()
{
}

void CLevel_Editor_Sprite::Begin()
{
	// ����� ����
	CSound* pSound = CAssetMgr::GetInst()->LoadSound(L"DM_Opening", L"Sound\\DM.wav");
	if (nullptr != pSound)
	{
		pSound->SetVolume(70.f);
		pSound->PlayToBGM(true);
	}

	// �޴��� �ε� �� ���� �����쿡 ����
	if (nullptr == m_hMenu)
	{
		m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPI_SPRITE));
	}
	SetMenu(CEngine::GetInst()->GetMainWndHwnd(), m_hMenu);

	// �޴��ٰ� �߰��Ǿ��� ������, ������ ũ�⸦ �������Ѵ�.
	CEngine::GetInst()->ChangeWindowSize(CEngine::GetInst()->GetResolution());

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	// atlas �ʱ�ȭ
	m_AtlasTexture = nullptr;


	// PanelUI ����
	/*
	CPanelUI* pPanel = new CPanelUI;
	pPanel->SetName(L"Panel 1");
	Vec2 vScale = Vec2(380.f, 500.f);

	pPanel->SetPos(Vec2(vResolution.x - vScale.x - 10, 10.f));
	pPanel->SetScale(vScale);

	// Panel �� ���� �ڽ� UI
	CBtnUI* pBtn = new CBtnUI;
	pBtn->SetScale(Vec2(150.f, 100.f));
	pBtn->SetPos(Vec2(10.f, 10.f));

	void SaveTileMap();
	//pBtn->AddCallBack(&SaveTileMap);
	pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_Editor::SaveTileMap);

	pPanel->AddChildUI(pBtn);
	AddObject(pPanel, LAYER_TYPE::UI);

	// PanelUI ����
	pPanel = new CPanelUI;
	pPanel->SetName(L"Panel 2");

	pPanel->SetPos(Vec2(vResolution.x - vScale.x - 10.f - 500.f, 10.f));
	pPanel->SetScale(vScale);

	// Panel �� ���� �ڽ� UI
	pBtn = new CBtnUI;
	pBtn->SetScale(Vec2(150.f, 100.f));
	pBtn->SetPos(Vec2(10.f, 10.f));

	void LoadTileMap();
	//pBtn->AddCallBack(&LoadTileMap);
	pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_Editor::LoadTileMap);

	pPanel->AddChildUI(pBtn);
	AddObject(pPanel, LAYER_TYPE::UI);
	*/

	// ���� �Ҽ� ��� ������Ʈ�� Begin �� ȣ����� �� �ֵ��� �Ѵ�
	CLevel::Begin();
}

void CLevel_Editor_Sprite::End()
{
	//DELETE(m_AtlasTexture);
	DeleteAllObject();

	// �޴��� ����
	SetMenu(CEngine::GetInst()->GetMainWndHwnd(), nullptr);

	// �޴��� ���ŵǾ����Ƿ�, �ٽ� ������ ũ�� ����
	CEngine::GetInst()->ChangeWindowSize(CEngine::GetInst()->GetResolution());
}

void CLevel_Editor_Sprite::Tick()
{
	CLevel::Tick();

	// Ư�� Ű�� ������ Start ������ ����
	if (KEY_TAP(KEY::M))
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		ChangeLevel(LEVEL_TYPE::START);
	}

	// ���콺 Ŭ������ CMap ������Ʈ�� Ÿ�� �̹��� �ε��� ����
	// �Ϲ����� ������ : ���� ��ǥ -> Render ��ǥ ����
	// ���콺 ��ǥ : Render ��ǥ(���콺��ǥ) -> ���� ��ǥ�� ����
	if (m_bSpriteMenu && KEY_PRESSED(KEY::T))
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_SpritePos = CCamera::GetInst()->GetRealPos(CKeyMgr::GetInst()->GetMousePos());
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_X, m_SpritePos.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_Y, m_SpritePos.y, FALSE);
		}

		if (KEY_RELEASED(KEY::LBTN))
		{
			m_SpriteScale = CCamera::GetInst()->GetRealPos(CKeyMgr::GetInst()->GetMousePos()) - m_SpritePos;
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_X, m_SpriteScale.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_Y, m_SpriteScale.y, FALSE);
			m_bDrawSprite = true;
		}
	}
	
}

void CLevel_Editor_Sprite::Render()
{
	CLevel::Render();

	if(nullptr != m_AtlasTexture)
	{
		UINT Width = m_AtlasTexture->GetWidth();
		UINT Height = m_AtlasTexture->GetHeight();
		HDC hBackDC = CEngine::GetInst()->GetSecondDC();

		TransparentBlt(hBackDC
			, 0
			, 0
			, Width, Height
			, m_AtlasTexture->GetDC()
			, 0, 0, Width, Height, RGB(255, 0, 255));

		TextOut(CEngine::GetInst()->GetSecondDC(), 10, 10, L"Editor Level", wcslen(L"Editor Level"));

		if (m_bSpriteMenu && m_bDrawSprite)
		{
			HWND findhandle = GetDlgItem(m_hDlgHandle, IDC_SPRITE_EXAMPLE);
			HDC m_hDC = GetDC(findhandle);
			RECT rect;
			GetClientRect(findhandle, &rect); 
			 
			SELECT_BRUSH(BRUSH_TYPE::GRAY);
			Rectangle(m_hDC, -1, -1, (int)(rect.right - rect.left) + 1, (int)(rect.bottom - rect.top) + 1);
			BitBlt(m_hDC
				, (int)(((rect.right - rect.left)/2) - (m_SpriteScale.x / 2))
				, (int)(((rect.bottom - rect.top) / 2) - (m_SpriteScale.y / 2))
				, m_SpriteScale.x, m_SpriteScale.y
				, m_AtlasTexture->GetDC()
				, m_SpritePos.x, m_SpritePos.y, SRCCOPY);

			m_bDrawSprite = false;
		}
	}


	wchar_t str1[255];
	wchar_t str2[255];
	swprintf_s(str1, 255, L"SpritePos x: %d, y: %d", (int)m_SpritePos.x, (int)m_SpritePos.y);
	swprintf_s(str2, 255, L"SpriteScale x: %d, y: %d", (int)m_SpriteScale.x, (int)m_SpriteScale.y);
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 30, str1, wcslen(str1));
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 50, str2, wcslen(str2));
}

void CLevel_Editor_Sprite::LoadAtlasTexture()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Texture";

	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"\0PNG\0.png\0BMP\0*.bmp\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		wstring tempString = szFilePath;
		tempString = tempString.substr(strContentPath.length());
		wstring tempKey = tempString.substr(1, tempString.find(L".")-1);

		m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempKey, (L"Texture" + tempString));
	}
}

void CLevel_Editor_Sprite::SaveSprite()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Sprite";

	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"\0SPRITE\0.sprite\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		wstring tempString = szFilePath;
		tempString = tempString.substr(strContentPath.length());
		//m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempString, (L"Texture\\" + tempString));

		// sprite 
		CSprite* pSprite = new CSprite;
		pSprite->Create(m_AtlasTexture, m_SpritePos, m_SpriteScale);

		wchar_t Key[50] = {};
		swprintf_s(Key, 50, (tempString).c_str());
		CAssetMgr::GetInst()->AddSprite(Key, pSprite);

		pSprite->Save(L"Sprite\\" + tempString);
	}
}

void CLevel_Editor_Sprite::LoadSprite()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Sprite";

	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"\0SPRITE\0.sprite\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		wstring tempString = szFilePath;
		tempString = tempString.substr(strContentPath.length());
		//m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempString, (L"Texture\\" + tempString));
		
		wchar_t Key[50] = {};
		swprintf_s(Key, 50, (tempString.substr(0, tempString.length() - 7)).c_str());
		CSprite* pSprite = CAssetMgr::GetInst()->LoadSprite(Key, L"Sprite" + tempString);

		// ��������Ʈ Pos �� Slice ����
		m_SpritePos = pSprite->GetLeftTop();
		m_SpriteScale = pSprite->GetSlice();

		// ������ ������ Edit Control �� �� ����
		SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_X, m_SpritePos.x, FALSE);
		SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_Y, m_SpritePos.y, FALSE);

		SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_X, m_SpriteScale.x, FALSE);
		SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_Y, m_SpriteScale.y, FALSE);

		// picture control �� �ٽ� render �ϵ��� true �� ����
		m_bDrawSprite = true;
		
	}
}




/*
void CLevel_Editor_Sprite::SaveTileMap()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"TileMap";


	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;	// ���������� ���� ��θ� �޾Ƴ� ������
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		// �� ������Ʈ�� TileMap ������Ʈ ������ �����Ѵ�.
		m_MapObj->GetTileMap()->SaveTileMap(szFilePath);
	}
}

void CLevel_Editor_Sprite::LoadTileMap()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"TileMap";

	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		m_MapObj->GetTileMap()->LoadTileMap(szFilePath);
	}
}
*/





// =======
// �����Լ�
// =======
INT_PTR CALLBACK    SpriteInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool SpriteEditorMenu(HINSTANCE _inst, HWND _wnd, int wParam)
{
	switch (wParam)
	{
	//case ID_TILEMAP_INFO:
	//{
	//	DialogBox(_inst, MAKEINTRESOURCE(DLG_TILEMAP_INFO), _wnd, &TileMapInfoProc);

	//	/* if(nullptr == g_hDlg)
	//		g_hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(DLG_TILEMAP_INFO), hWnd, &TileMapInfoProc);

	//	ShowWindow(g_hDlg, true);*/
	//	return true;
	//}
	//case ID_TILEMAP_SAVE:
	//{
	//	// CLevel_Editor �� �ִ� MapObject �� Ÿ�ϸ� ������Ʈ�� �� ���� �������־�� ��
	//	// ���� ������ �˾Ƴ���. ��Ȳ�� ���� ������ �ݵ�� CLevel_Editor ���� �Ѵ�.
	//	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	//	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
	//	assert(pEditorLevel);

	//	pEditorLevel->SaveTileMap();
	//}
	//return true;
	//case ID_TILEMAP_LOAD:
	//{
	//	// CLevel_Editor �� �ִ� MapObject �� Ÿ�ϸ� ������Ʈ�� �� ���� �������־�� ��
	//	// ���� ������ �˾Ƴ���. ��Ȳ�� ���� ������ �ݵ�� CLevel_Editor ���� �Ѵ�.
	//	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	//	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
	//	assert(pEditorLevel);

	//	pEditorLevel->LoadTileMap();
	//}
	//return true;
	case ID_ATLAS_LOAD:
	{
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
		assert(pEditorLevel);

		pEditorLevel->LoadAtlasTexture();
	}
	return true;
	case ID_SPRITE_SET:
	{
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
		assert(pEditorLevel);

		pEditorLevel->SetSpriteMenu(true);

		// ��޸��� dialog box �� ������ �޴� ����
		HWND g_hDlg = nullptr;
		if (nullptr == g_hDlg)
			g_hDlg = CreateDialog(_inst, MAKEINTRESOURCE(DLG_SPRITE_INFO), _wnd, &SpriteInfoProc);

		// editor level ���� dialog box �� ��� ������ �����ϰ� hwnd �Ѱ���
		pEditorLevel->SetDlgHandle(g_hDlg);
		ShowWindow(g_hDlg, true);
	}
	return true;
	case ID_EDIT_CHANGE_TILEMAP:
		// ���� ����
		ChangeLevel(LEVEL_TYPE::EDITOR_TILE);
	};

	return false;
}

// ============================
// SPRITE_INFO Dialog ���ν���
// ============================
INT_PTR CALLBACK SpriteInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			pEditorLevel->SaveSprite();

			return (INT_PTR)TRUE;
		}

		else if (LOWORD(wParam) == IDCANCEL)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			pEditorLevel->SetSpriteMenu(false);

			EndDialog(hDlg, LOWORD(wParam));
		}

		else if (LOWORD(wParam) == ID_APPLY_DATA)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			int posX = GetDlgItemInt(hDlg, IDC_SPRITE_POS_X, nullptr, true);
			int posY = GetDlgItemInt(hDlg, IDC_SPRITE_POS_Y, nullptr, true);
			int scaleX = GetDlgItemInt(hDlg, IDC_SPRITE_SCALE_X, nullptr, true);
			int scaleY = GetDlgItemInt(hDlg, IDC_SPRITE_SCALE_Y, nullptr, true);

			pEditorLevel->SetSpritePos({ posX, posY });
			pEditorLevel->SetSpriteScale({ scaleX, scaleY });
			pEditorLevel->SetDrawSprite(true);
		}

		else if (LOWORD(wParam) == ID_LOAD_SPRITE)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			pEditorLevel->LoadSprite();
		}


		break;
	}

	return (INT_PTR)FALSE;
}