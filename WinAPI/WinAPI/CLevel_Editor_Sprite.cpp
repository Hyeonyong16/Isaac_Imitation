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
#include "CFlipbook.h"


CLevel_Editor_Sprite::CLevel_Editor_Sprite()
	: m_AtlasTexture(nullptr)
	, m_hMenu(nullptr)
	, m_bSpriteMenu(false)
	, m_SpritePos{}
	, m_SpriteScale{}
	, m_SpriteOffset{}
	, m_bDrawSprite(false)
	, m_bMouseLBtnPressed(false)
	, m_dbgRectPos{}
	, m_bFlipbookMenu(false)
	, m_curSprite(nullptr)
{
	
}

CLevel_Editor_Sprite::~CLevel_Editor_Sprite()
{
}

void CLevel_Editor_Sprite::Begin()
{
	// 배경음 지정
	CSound* pSound = CAssetMgr::GetInst()->LoadSound(L"DM_Opening", L"Sound\\DM.wav");
	if (nullptr != pSound)
	{
		pSound->SetVolume(0.f);
		pSound->PlayToBGM(true);
	}

	// 메뉴바 로드 및 메인 윈도우에 부착
	if (nullptr == m_hMenu)
	{
		m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPI_SPRITE));
	}
	SetMenu(CEngine::GetInst()->GetMainWndHwnd(), m_hMenu);

	// 메뉴바가 추가되었기 때문에, 윈도우 크기를 재조정한다.
	CEngine::GetInst()->ChangeWindowSize(CEngine::GetInst()->GetResolution());

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	// atlas 초기화
	m_AtlasTexture = nullptr;


	// PanelUI 생성
	/*
	CPanelUI* pPanel = new CPanelUI;
	pPanel->SetName(L"Panel 1");
	Vec2 vScale = Vec2(380.f, 500.f);

	pPanel->SetPos(Vec2(vResolution.x - vScale.x - 10, 10.f));
	pPanel->SetScale(vScale);

	// Panel 에 넣을 자식 UI
	CBtnUI* pBtn = new CBtnUI;
	pBtn->SetScale(Vec2(150.f, 100.f));
	pBtn->SetPos(Vec2(10.f, 10.f));

	void SaveTileMap();
	//pBtn->AddCallBack(&SaveTileMap);
	pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_Editor::SaveTileMap);

	pPanel->AddChildUI(pBtn);
	AddObject(pPanel, LAYER_TYPE::UI);

	// PanelUI 생성
	pPanel = new CPanelUI;
	pPanel->SetName(L"Panel 2");

	pPanel->SetPos(Vec2(vResolution.x - vScale.x - 10.f - 500.f, 10.f));
	pPanel->SetScale(vScale);

	// Panel 에 넣을 자식 UI
	pBtn = new CBtnUI;
	pBtn->SetScale(Vec2(150.f, 100.f));
	pBtn->SetPos(Vec2(10.f, 10.f));

	void LoadTileMap();
	//pBtn->AddCallBack(&LoadTileMap);
	pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_Editor::LoadTileMap);

	pPanel->AddChildUI(pBtn);
	AddObject(pPanel, LAYER_TYPE::UI);
	*/

	// 레벨 소속 모든 오브젝트가 Begin 을 호출받을 수 있도록 한다
	CLevel::Begin();
}

void CLevel_Editor_Sprite::End()
{
	//DELETE(m_AtlasTexture);
	DeleteAllObject();

	// 메뉴바 제거
	SetMenu(CEngine::GetInst()->GetMainWndHwnd(), nullptr);

	// 메뉴가 제거되었으므로, 다시 윈도우 크기 조정
	CEngine::GetInst()->ChangeWindowSize(CEngine::GetInst()->GetResolution());
}

void CLevel_Editor_Sprite::Tick()
{
	CLevel::Tick();

	// 특정 키가 눌리면 Start 레벨로 변경
	if (KEY_TAP(KEY::M))
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		ChangeLevel(LEVEL_TYPE::START);
	}

	// 마우스 클릭으로 CMap 오브젝트의 타일 이미지 인덱스 변경
	// 일반적인 렌더링 : 실제 좌표 -> Render 좌표 변경
	// 마우스 좌표 : Render 좌표(마우스좌표) -> 실제 좌표로 변경
	if (m_bSpriteMenu && KEY_PRESSED(KEY::T))
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_SpritePos = CCamera::GetInst()->GetRealPos(CKeyMgr::GetInst()->GetMousePos());
			m_dbgRectPos = m_SpritePos;
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_X, m_SpritePos.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_Y, m_SpritePos.y, FALSE);
			m_bMouseLBtnPressed = true;
		}

		if (KEY_PRESSED(KEY::LBTN))
		{
			m_dbgRectPos = CCamera::GetInst()->GetRealPos(CKeyMgr::GetInst()->GetMousePos());
		}

		if (KEY_RELEASED(KEY::LBTN))
		{
			m_SpriteScale = CCamera::GetInst()->GetRealPos(CKeyMgr::GetInst()->GetMousePos()) - m_SpritePos;
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_X, m_SpriteScale.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_Y, m_SpriteScale.y, FALSE);
			m_bDrawSprite = true;
			m_bMouseLBtnPressed = false;
		}
	}
	
}

void CLevel_Editor_Sprite::Render()
{
	CLevel::Render();

	if (nullptr != m_AtlasTexture)
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

		if (m_bMouseLBtnPressed)
		{
			SELECT_PEN(PEN_TYPE::GREEN);
			SELECT_BRUSH(BRUSH_TYPE::HOLLOW);
			Rectangle(hBackDC
				, m_SpritePos.x
				, m_SpritePos.y
				, m_dbgRectPos.x
				, m_dbgRectPos.y);
		}

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
				, (int)(((rect.right - rect.left) / 2) - (m_SpriteScale.x / 2) + m_SpriteOffset.x)
				, (int)(((rect.bottom - rect.top) / 2) - (m_SpriteScale.y / 2) + m_SpriteOffset.y)
				, m_SpriteScale.x, m_SpriteScale.y
				, m_AtlasTexture->GetDC()
				, m_SpritePos.x, m_SpritePos.y, SRCCOPY);

			SELECT_PEN(PEN_TYPE::BLUE);
			MoveToEx(m_hDC, rect.right / 2, 0.f, nullptr);
			LineTo(m_hDC, rect.right / 2, rect.bottom);
			MoveToEx(m_hDC, 0.f, rect.bottom / 2, nullptr);
			LineTo(m_hDC, rect.right, rect.bottom / 2);


			m_bDrawSprite = false;
		}

		else if (m_bFlipbookMenu && m_bDrawSprite)
		{
			if(0 != m_SpriteList.size())
			{
				HWND findhandle = GetDlgItem(m_hDlgHandle, IDC_FLIPBOOK_EXAMPLE);
				HDC m_hDC = GetDC(findhandle);
				RECT rect;
				GetClientRect(findhandle, &rect);
				Rectangle(m_hDC, -1, -1, (int)(rect.right - rect.left) + 1, (int)(rect.bottom - rect.top) + 1);
				BitBlt(m_hDC
					, (int)(((rect.right - rect.left) / 2) - (m_curSprite->GetSlice().x / 2) + m_curSprite->GetOffset().x)
					, (int)(((rect.bottom - rect.top) / 2) - (m_curSprite->GetSlice().y / 2) + m_curSprite->GetOffset().y)
					, m_curSprite->GetSlice().x, m_curSprite->GetSlice().y
					, m_curSprite->GetAtlas()->GetDC()
					, m_curSprite->GetLeftTop().x, m_curSprite->GetLeftTop().y, SRCCOPY);

				SELECT_PEN(PEN_TYPE::BLUE);
				MoveToEx(m_hDC, rect.right / 2, 0.f, nullptr);
				LineTo(m_hDC, rect.right / 2, rect.bottom);
				MoveToEx(m_hDC, 0.f, rect.bottom / 2, nullptr);
				LineTo(m_hDC, rect.right , rect.bottom / 2);

				m_bDrawSprite = false;
			}
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

	// 파일 경로 문자열
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

	// 파일 경로 문자열
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
		wstring tempKey = tempString.substr(1, tempString.find(L".") - 1);
		//m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempString, (L"Texture\\" + tempString));



		// sprite 
		CSprite* pSprite = new CSprite;
		pSprite->Create(m_AtlasTexture, m_SpritePos, m_SpriteScale);
		pSprite->SetOffset(m_SpriteOffset);

		//wchar_t Key[50] = {};
		//swprintf_s(Key, 50, (tempString).c_str());
		if(!CAssetMgr::GetInst()->FindSprite(tempKey))
			CAssetMgr::GetInst()->AddSprite(tempKey, pSprite);
		
		else
		{
			CAssetMgr::GetInst()->FindSprite(tempKey)->SetOffset(m_SpriteOffset);
		}
		
		//=======================================================
		// 어째서 기존 애를 덮어쓰기 하면 메모리 누수가 생기는가?
		//=======================================================
		pSprite->Save(L"Sprite" + tempString);
	}
}

void CLevel_Editor_Sprite::LoadSprite()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Sprite";

	// 파일 경로 문자열
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
		wstring tempKey = tempString.substr(1, tempString.find(L".") - 1);
		//m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempString, (L"Texture\\" + tempString));

		//wchar_t Key[50] = {};
		//swprintf_s(Key, 50, (tempString).c_str());
		CSprite* pSprite = CAssetMgr::GetInst()->LoadSprite(tempKey, L"Sprite" + tempString);

		if(m_bSpriteMenu)
		{
			// 스프라이트 Pos 와 Slice 설정
			m_SpritePos = pSprite->GetLeftTop();
			m_SpriteScale = pSprite->GetSlice();
			m_SpriteOffset = pSprite->GetOffset();

			// 설정한 값으로 Edit Control 에 값 설정
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_X, m_SpritePos.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_POS_Y, m_SpritePos.y, FALSE);

			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_X, m_SpriteScale.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_SCALE_Y, m_SpriteScale.y, FALSE);

			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_OFFSET_X, m_SpriteOffset.x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_SPRITE_OFFSET_Y, m_SpriteOffset.y, FALSE);

			// picture control 에 다시 render 하도록 true 로 변경
			m_bDrawSprite = true;
		}
		
		else if (m_bFlipbookMenu)
		{
			pSprite->GetOffset();
			SetDlgItemInt(m_hDlgHandle, IDC_FLIPBOOK_OFFSET_X, pSprite->GetOffset().x, FALSE);
			SetDlgItemInt(m_hDlgHandle, IDC_FLIPBOOK_OFFSET_Y, pSprite->GetOffset().y, FALSE);

			m_SpriteList.push_back(pSprite);
			m_curSprite = pSprite;

			m_bDrawSprite = true;
		}
	}
}

void CLevel_Editor_Sprite::SaveFlipbook()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Flipbook";

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"\FLIP\0.flip\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		wstring tempString = szFilePath;
		tempString = tempString.substr(strContentPath.length());
		wstring tempKey = tempString.substr(1, tempString.find(L".") - 1);
		//m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempString, (L"Texture\\" + tempString));

		CFlipbook* pFlipbook = new CFlipbook;

		list<CSprite*>::iterator iter = m_SpriteList.begin();
		for (; iter != m_SpriteList.end(); ++iter)
		{
			pFlipbook->AddSprite(*iter);
		}

		if (!CAssetMgr::GetInst()->FindFlipbook(tempKey))
			CAssetMgr::GetInst()->AddFlipbook(tempKey, pFlipbook);

		//=======================================================
		// 어째서 기존 애를 덮어쓰기 하면 메모리 누수가 생기는가?
		//=======================================================
		pFlipbook->Save(L"Flipbook" + tempString);
	}
}

void CLevel_Editor_Sprite::LoadFlipbook()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Flipbook";

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"\FLIP\0.flip\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		wstring tempString = szFilePath;
		tempString = tempString.substr(strContentPath.length());
		wstring tempKey = tempString.substr(1, tempString.find(L".") - 1);

		CFlipbook* pFlipbook = CAssetMgr::GetInst()->LoadFlipbook(tempKey, L"Flipbook" + tempString);

		HWND hwndList = GetDlgItem(m_hDlgHandle, IDC_SPRITE_LIST);
		
		m_SpriteList.clear();
		SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

		for (int i = 0; i < pFlipbook->GetMaxSpriteCount(); ++i)
		{
			m_SpriteList.push_back(pFlipbook->GetSprite(i));
			int pos = SendMessage(hwndList, LB_GETCURSEL, 0, 0);

			wstring spriteKey = pFlipbook->GetSprite(i)->GetKey();
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)spriteKey.c_str());
		}

		SendMessage(hwndList, LB_SETCURSEL, 0, 0);
	}
}



// =======
// 전역함수
// =======
INT_PTR CALLBACK    SpriteInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	FlipbookInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool SpriteEditorMenu(HINSTANCE _inst, HWND _wnd, int wParam)
{
	switch (wParam)
	{
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

		// 모달리스 dialog box 로 에디터 메뉴 생성
		HWND g_hDlg = nullptr;
		if (nullptr == g_hDlg)
			g_hDlg = CreateDialog(_inst, MAKEINTRESOURCE(DLG_SPRITE_INFO), _wnd, &SpriteInfoProc);

		// editor level 에서 dialog box 내 요소 접근이 가능하게 hwnd 넘겨줌
		if (nullptr != pEditorLevel->GetDlgHandle())
			EndDialog(pEditorLevel->GetDlgHandle(), IDCANCEL);
		pEditorLevel->SetDlgHandle(g_hDlg);
		ShowWindow(g_hDlg, true);
	}
	return true;


	case ID_FLIPBOOK_SET:
	{
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
		assert(pEditorLevel);

		pEditorLevel->SetFlipbookMenu(true);

		// 모달리스 dialog box 로 에디터 메뉴 생성
		HWND g_hDlg = nullptr;
		if (nullptr == g_hDlg)
			g_hDlg = CreateDialog(_inst, MAKEINTRESOURCE(DLG_FLIPBOOK_INFO), _wnd, &FlipbookInfoProc);

		// editor level 에서 dialog box 내 요소 접근이 가능하게 hwnd 넘겨줌
		if (nullptr != pEditorLevel->GetDlgHandle())
			EndDialog(pEditorLevel->GetDlgHandle(), IDCANCEL);
		pEditorLevel->SetDlgHandle(g_hDlg);
		ShowWindow(g_hDlg, true);
	}
	return true;


	case ID_EDIT_CHANGE_TILEMAP:
		// 레벨 변경
		ChangeLevel(LEVEL_TYPE::EDITOR_TILE);
	};

	return false;
}

// ============================
// SPRITE_INFO Dialog 프로시저
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
			int offsetX = GetDlgItemInt(hDlg, IDC_SPRITE_OFFSET_X, nullptr, true);
			int offsetY = GetDlgItemInt(hDlg, IDC_SPRITE_OFFSET_Y, nullptr, true);

			pEditorLevel->SetSpritePos({ posX, posY });
			pEditorLevel->SetSpriteScale({ scaleX, scaleY });
			pEditorLevel->SetSpriteOffset({ offsetX, offsetY });

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


// ============================
// FLIPBOOK_INFO Dialog 프로시저
// ============================
INT_PTR CALLBACK	FlipbookInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

			pEditorLevel->SaveFlipbook();

			return (INT_PTR)TRUE;
		}

		else if (LOWORD(wParam) == IDCANCEL)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			//pEditorLevel->SetSpriteMenu(false);

			EndDialog(hDlg, LOWORD(wParam));
		}

		else if (LOWORD(wParam) == ID_LOAD_FLIPBOOK)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			pEditorLevel->LoadFlipbook();
		}

		else if (LOWORD(wParam) == ID_APPLY_OFFSET)
		{
			float xOffset = GetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_X, nullptr, true);
			float yOffset = GetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_Y, nullptr, true);

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			CSprite* pSprite = pEditorLevel->GetCurSprite();
			pSprite->SetOffset(Vec2(xOffset, yOffset));
			pEditorLevel->SetDrawSprite(true);
		}

		else if (LOWORD(wParam) == ID_DELETE_SPRITE)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			HWND hwndList = GetDlgItem(hDlg, IDC_SPRITE_LIST);
			int pos = SendMessage(hwndList, LB_GETCURSEL, 0, 0);

			if (-1 != pos)
			{
				SendMessage(hwndList, LB_DELETESTRING, pos, 0);
				pEditorLevel->DeleteIdxSpriteList(pos);

				int count = SendMessage(hwndList, LB_GETCOUNT, 0, 0);
				if (count > 0)
				{
					SendMessage(hwndList, LB_SETCURSEL, 0, 0);

					int index = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
					wchar_t buffer[50] = {};
					SendMessage(hwndList, LB_GETTEXT, index, (LPARAM)buffer);

					pEditorLevel->SetCurSprite(CAssetMgr::GetInst()->FindSprite(buffer));
					pEditorLevel->SetDrawSprite(true);
				}
			}
		}

		else if (LOWORD(wParam) == ID_ADD_SPRITE)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			pEditorLevel->LoadSprite();

			HWND hwndList = GetDlgItem(hDlg, IDC_SPRITE_LIST);
			wstring spriteKey = pEditorLevel->GetSpriteList().back()->GetKey();
			int pos = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			pos = (int)SendMessage(hwndList, LB_INSERTSTRING, pos + 1, (LPARAM)spriteKey.c_str());
			SendMessage(hwndList, LB_SETCURSEL, pos, 0);

		}

		else if (LOWORD(wParam) == IDC_PREV_BTN)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			HWND hwndList = GetDlgItem(hDlg, IDC_SPRITE_LIST);

			int pos = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			int count = SendMessage(hwndList, LB_GETCOUNT, 0, 0);

			if (pos > 0)
			{
				SendMessage(hwndList, LB_SETCURSEL, pos - 1, 0);

				int index = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				wchar_t buffer[50] = {};
				SendMessage(hwndList, LB_GETTEXT, index, (LPARAM)buffer);

				pEditorLevel->SetCurSprite(CAssetMgr::GetInst()->FindSprite(buffer));
				SetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_X, pEditorLevel->GetCurSprite()->GetOffset().x, FALSE);
				SetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_Y, pEditorLevel->GetCurSprite()->GetOffset().y, FALSE);
				pEditorLevel->SetDrawSprite(true);
			}
		}

		else if (LOWORD(wParam) == IDC_NEXT_BTN)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			HWND hwndList = GetDlgItem(hDlg, IDC_SPRITE_LIST);

			int pos = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			int count = SendMessage(hwndList, LB_GETCOUNT, 0, 0);

			if (pos < count - 1)
			{
				SendMessage(hwndList, LB_SETCURSEL, pos + 1, 0);

				int index = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				wchar_t buffer[50] = {};
				SendMessage(hwndList, LB_GETTEXT, index, (LPARAM)buffer);

				pEditorLevel->SetCurSprite(CAssetMgr::GetInst()->FindSprite(buffer));
				SetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_X, pEditorLevel->GetCurSprite()->GetOffset().x, FALSE);
				SetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_Y, pEditorLevel->GetCurSprite()->GetOffset().y, FALSE);
				pEditorLevel->SetDrawSprite(true);
			}
		}

		else if (LOWORD(wParam) == IDC_SPRITE_LIST)
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLevel_Editor_Sprite* pEditorLevel = dynamic_cast<CLevel_Editor_Sprite*>(pLevel);
			assert(pEditorLevel);

			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			case LB_SETCURSEL:
			{
				HWND hwndList = GetDlgItem(hDlg, IDC_SPRITE_LIST);

				int index = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				wchar_t buffer[50] = {};
				SendMessage(hwndList, LB_GETTEXT, index, (LPARAM)buffer);
				
				pEditorLevel->SetCurSprite(CAssetMgr::GetInst()->FindSprite(buffer));
				SetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_X, pEditorLevel->GetCurSprite()->GetOffset().x, FALSE);
				SetDlgItemInt(hDlg, IDC_FLIPBOOK_OFFSET_Y, pEditorLevel->GetCurSprite()->GetOffset().y, FALSE);
				pEditorLevel->SetDrawSprite(true);
			}
			break;
			}

		}

		break;
	}

	return (INT_PTR)FALSE;
}