#include "pch.h"
#include "CLevel_Editor.h"

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

#include "CSprite.h"
#include "CTexture.h"

#include "CRoom.h"

#include "CSelectGDI.h"

CLevel_Editor::CLevel_Editor()
	: m_MapObj(nullptr)
	, m_hMenu(nullptr)
	, m_roomInfo(ROOM_INFO::ROOM)
	, m_room(nullptr)
	, m_curSprite(nullptr)
	, m_hDlgHandle(nullptr)
	, m_bDrawSprite(false)
{
	m_wallSprite = CAssetMgr::GetInst()->LoadSprite(L"MAP_WALL", L"Sprite\\MAP_WALL.sprite");
}

CLevel_Editor::~CLevel_Editor()
{
}

void CLevel_Editor::Begin()
{
	// ����� ����
	CSound* pSound = CAssetMgr::GetInst()->LoadSound(L"DM_Opening", L"Sound\\DM.wav");
	if (nullptr != pSound)
	{
		pSound->SetVolume(0.f);
		pSound->PlayToBGM(true);
	}

	// �޴��� �ε� �� ���� �����쿡 ����
	if (nullptr == m_hMenu)
	{
		m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPI_TILEMAP));
	}
	SetMenu(CEngine::GetInst()->GetMainWndHwnd(), m_hMenu);

	// �޴��ٰ� �߰��Ǿ��� ������, ������ ũ�⸦ �������Ѵ�.
	CEngine::GetInst()->ChangeWindowSize(CEngine::GetInst()->GetResolution());

	Vec2 vResolution = CEngine::GetInst()->GetResolution();


	// PanelUI ����
	{
		//CPanelUI* pPanel = new CPanelUI;
		//pPanel->SetName(L"Panel 1");
		//Vec2 vScale = Vec2(380.f, 500.f);

		//pPanel->SetPos(Vec2(vResolution.x - vScale.x - 10, 10.f));
		//pPanel->SetScale(vScale);

		//// Panel �� ���� �ڽ� UI
		//CBtnUI* pBtn = new CBtnUI;
		//pBtn->SetScale(Vec2(150.f, 100.f));
		//pBtn->SetPos(Vec2(10.f, 10.f));

		//void SaveTileMap();
		////pBtn->AddCallBack(&SaveTileMap);
		//pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_Editor::SaveTileMap);

		//pPanel->AddChildUI(pBtn);
		//AddObject(pPanel, LAYER_TYPE::UI);

		//// PanelUI ����
		//pPanel = new CPanelUI;
		//pPanel->SetName(L"Panel 2");

		//pPanel->SetPos(Vec2(vResolution.x - vScale.x - 10.f - 500.f, 10.f));
		//pPanel->SetScale(vScale);

		//// Panel �� ���� �ڽ� UI
		//pBtn = new CBtnUI;
		//pBtn->SetScale(Vec2(150.f, 100.f));
		//pBtn->SetPos(Vec2(10.f, 10.f));

		//void LoadTileMap();
		////pBtn->AddCallBack(&LoadTileMap);
		//pBtn->AddDelegate(this, (DELEGATE_0)&CLevel_Editor::LoadTileMap);

		//pPanel->AddChildUI(pBtn);
		//AddObject(pPanel, LAYER_TYPE::UI);
	}


	// ���ÿ� Map ������Ʈ ����
	m_MapObj = new CMap;
	m_MapObj->SetPos(210.f, 180.f);
	m_MapObj->GetComponent<CTileMap>()->SetRowCol(0, 0);
	AddObject(m_MapObj, LAYER_TYPE::TILE);
	
	m_room = new CRoom;
	m_room->SetPos(210.f, 180.f);
	AddObject(m_room, LAYER_TYPE::DEFAULT);




	// ���� �Ҽ� ��� ������Ʈ�� Begin �� ȣ����� �� �ֵ��� �Ѵ�
	CLevel::Begin();
}

void CLevel_Editor::End()
{
	DeleteAllObject();

	// �޴��� ����
	SetMenu(CEngine::GetInst()->GetMainWndHwnd(), nullptr);

	// �޴��� ���ŵǾ����Ƿ�, �ٽ� ������ ũ�� ����
	CEngine::GetInst()->ChangeWindowSize(CEngine::GetInst()->GetResolution());
}


void CLevel_Editor::Tick()
{
	CLevel::Tick();

	// Ư�� Ű�� ������ Start ������ ����
	if (KEY_TAP(KEY::M))
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		//ChangeLevel(LEVEL_TYPE::START);
	}

	// ���콺 Ŭ������ CMap ������Ʈ�� Ÿ�� �̹��� �ε��� ����
	// �Ϲ����� ������ : ���� ��ǥ -> Render ��ǥ ����
	// ���콺 ��ǥ : Render ��ǥ(���콺��ǥ) -> ���� ��ǥ�� ����
	if (KEY_TAP(KEY::LBTN))
	{
		if (nullptr == m_room)
			return;
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		tObjInfo* objInfo = m_room->GetObjInfo(vMousePos);

		if (nullptr != objInfo && nullptr != m_curSprite)
		{
			objInfo->ObjTypeKey = m_curSprite->GetKey();
		}
	}

	// ���콺 ��Ŭ�� �� ����
	if (KEY_TAP(KEY::RBTN))
	{
		if (nullptr == m_room)
			return;
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		tObjInfo* objInfo = m_room->GetObjInfo(vMousePos);

		if (nullptr != objInfo && nullptr != m_curSprite)
		{
			objInfo->ObjTypeKey = L"NOTHING";
		}
	}
}

void CLevel_Editor::Render()
{
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	HDC hBackDC = CEngine::GetInst()->GetSecondDC();

	// ��� �׸���
	{
		StretchBlt(hBackDC
			, 0, 0
			, vResolution.x / 2, vResolution.y / 2
			, m_wallSprite->GetAtlas()->GetDC()
			, m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
			, m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
			, SRCCOPY);

		StretchBlt(hBackDC
			, vResolution.x - 1, 0
			, -1 * (vResolution.x / 2), vResolution.y / 2
			, m_wallSprite->GetAtlas()->GetDC()
			, m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
			, m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
			, SRCCOPY);

		StretchBlt(hBackDC
			, 0, vResolution.y - 1
			, vResolution.x / 2, -1 * (vResolution.y / 2)
			, m_wallSprite->GetAtlas()->GetDC()
			, m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
			, m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
			, SRCCOPY);

		StretchBlt(hBackDC
			, vResolution.x - 1, vResolution.y - 1
			, -1 * (vResolution.x / 2), -1 * (vResolution.y / 2)
			, m_wallSprite->GetAtlas()->GetDC()
			, m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
			, m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
			, SRCCOPY);
	}

	CLevel::Render();

	BLENDFUNCTION blend = {};

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	// Ÿ�� ���� �޴��� �����ִٸ�
	if (m_hDlgHandle)
	{
		// picture Control �κ� draw
		if(m_bDrawSprite)
		{
			HWND findhandle = GetDlgItem(m_hDlgHandle, IDC_TILE_EXAMPLE);
			HDC m_hDC = GetDC(findhandle);
			RECT rect;
			GetClientRect(findhandle, &rect);

			SELECT_BRUSH(BRUSH_TYPE::GRAY);
			Rectangle(m_hDC, -1, -1, (int)(rect.right - rect.left) + 1, (int)(rect.bottom - rect.top) + 1);


			AlphaBlend(m_hDC
				, (((rect.right - rect.left) / 2) - (m_curSprite->GetSlice().x / 2) + m_curSprite->GetOffset().x)
				, (((rect.bottom - rect.top) / 2) - (m_curSprite->GetSlice().y / 2) + m_curSprite->GetOffset().y)
				, m_curSprite->GetSlice().x
				, m_curSprite->GetSlice().y
				, m_curSprite->GetAtlas()->GetDC()
				, m_curSprite->GetLeftTop().x
				, m_curSprite->GetLeftTop().y
				, m_curSprite->GetSlice().x
				, m_curSprite->GetSlice().y
				, blend);

			SELECT_PEN(PEN_TYPE::BLUE);
			MoveToEx(m_hDC, rect.right / 2, 0.f, nullptr);
			LineTo(m_hDC, rect.right / 2, rect.bottom);
			MoveToEx(m_hDC, 0.f, rect.bottom / 2, nullptr);
			LineTo(m_hDC, rect.right, rect.bottom / 2);

			m_bDrawSprite = false;
		}
	}

	// ���� �� ���� ���
	for (int Row = 0; Row < m_room->GetRow(); ++Row)
	{
		for (int Col = 0; Col < m_room->GetCol(); ++Col)
		{
			// �ݺ��� ȸ���� �´� ��Ŀ� ���ؼ� �̰� ���° Ÿ���������� 1���� �ε����� ��ȯ
			// �ش� Ÿ�������� �����Ѵ�.
			int TileIdx = m_room->GetCol() * Row + Col;
			tObjInfo ImgIdx = m_room->GetIdxObjInfo(TileIdx);

			// �ش� Ÿ���� ImgIdx �� -1 �� ���, Blank Tile
			if (ImgIdx.ObjTypeKey == L"NOTHING")
			{
				continue;
			}
				
			else
			{
				m_tempSprite = CAssetMgr::GetInst()->LoadSprite(ImgIdx.ObjTypeKey, L"Sprite\\" + ImgIdx.ObjTypeKey + L".sprite");

				/*StretchBlt(hBackDC
					, m_room->GetPos().x + Col * TILE_SIZE_X
					, m_room->GetPos().y + Row * TILE_SIZE_Y
					, TILE_SIZE_X, TILE_SIZE_Y
					, pSprite->GetAtlas()->GetDC()
					, pSprite->GetLeftTop().x, pSprite->GetLeftTop().y
					, pSprite->GetSlice().x, pSprite->GetSlice().y
					, SRCCOPY);*/

				AlphaBlend(hBackDC
					, m_room->GetPos().x + Col * TILE_SIZE_X
					, m_room->GetPos().y + Row * TILE_SIZE_Y
					, TILE_SIZE_X, TILE_SIZE_Y
					, m_tempSprite->GetAtlas()->GetDC()
					, m_tempSprite->GetLeftTop().x, m_tempSprite->GetLeftTop().y
					, m_tempSprite->GetSlice().x, m_tempSprite->GetSlice().y
					, blend);
			}
		}
	}

	Vec2 m_mousePos = CKeyMgr::GetInst()->GetMousePos();

	wchar_t str1[255];
	swprintf_s(str1, 255, L" x: %d, y: %d", (int)m_mousePos.x, (int)m_mousePos.y);
	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 30, str1, wcslen(str1));

	TextOut(CEngine::GetInst()->GetSecondDC(), 10, 10, L"Editor Level", wcslen(L"Editor Level"));

}

void CLevel_Editor::SaveTileMap()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"TileMap";


	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;	// ���������� �� ��θ� �޾Ƴ� ������
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

void CLevel_Editor::LoadTileMap()
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

void CLevel_Editor::LoadSprite()
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
		wstring tempKey = tempString.substr(1, tempString.find(L".") - 1);
		//m_AtlasTexture = CAssetMgr::GetInst()->LoadTexture(tempString, (L"Texture\\" + tempString));

		//wchar_t Key[50] = {};
		//swprintf_s(Key, 50, (tempString).c_str());
		CSprite* pSprite = CAssetMgr::GetInst()->LoadSprite(tempKey, L"Sprite" + tempString);

		m_SpriteList.push_back(pSprite);
		m_curSprite = pSprite;

		m_bDrawSprite = true;
	}
}

void CLevel_Editor::SaveRoom()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Room";

	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;	// ���������� �� ��θ� �޾Ƴ� ������
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Room\0*.room\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		m_room->Save(szFilePath);
	}
}

void CLevel_Editor::LoadRoom()
{
	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += L"Room";

	// ���� ��� ���ڿ�
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Room\0*.room\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		m_room->Load(szFilePath);
	}
}






// =======
// �����Լ�
// =======
INT_PTR CALLBACK    TileMapInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void SaveTileMap()
{
	DialogBox(nullptr, MAKEINTRESOURCE(DLG_TILEMAP_INFO), CEngine::GetInst()->GetMainWndHwnd(), &TileMapInfoProc);

	/*CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
	if (nullptr == pEditorLevel)
		return;

	pEditorLevel->SaveTileMap();*/
}

void LoadTileMap()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
	if (nullptr == pEditorLevel)
		return;

	pEditorLevel->LoadTileMap();
}


bool TileMapEditorMenu(HINSTANCE _inst, HWND _wnd, int wParam)
{
	switch (wParam)
	{
	case ID_TILE_SELECT:
	{
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
		assert(pEditorLevel);

		HWND g_hDlg = nullptr;
		if (nullptr == g_hDlg)
			g_hDlg = CreateDialog(_inst, MAKEINTRESOURCE(DLG_TILEMAP_INFO), _wnd, &TileMapInfoProc);

		pEditorLevel->SetDlgHandle(g_hDlg);
		ShowWindow(g_hDlg, true);

		return true;
	}
	case ID_TILEMAP_SAVE:
	{
		// CLevel_Editor �� �ִ� MapObject �� Ÿ�ϸ� ������Ʈ�� �� ���� �������־�� ��
		// ���� ������ �˾Ƴ���. ��Ȳ�� ���� ������ �ݵ�� CLevel_Editor ���� �Ѵ�.
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
		assert(pEditorLevel);

		pEditorLevel->SaveRoom();
	}
	return true;
	case ID_TILEMAP_LOAD:
	{
		// CLevel_Editor �� �ִ� MapObject �� Ÿ�ϸ� ������Ʈ�� �� ���� �������־�� ��
		// ���� ������ �˾Ƴ���. ��Ȳ�� ���� ������ �ݵ�� CLevel_Editor ���� �Ѵ�.
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
		assert(pEditorLevel);

		pEditorLevel->LoadRoom();
	}
	return true;
	case ID_EDIT_CHANGE_SPRITE:
		ChangeLevel(LEVEL_TYPE::EDITOR_SPRITE);
	};

	return false;
}

// ============================
// Tile_Map_Info Dialog ���ν���
// ============================
INT_PTR CALLBACK TileMapInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
	assert(pEditorLevel);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		if(0 != pEditorLevel->GetSpriteList().size())
		{
			HWND hwndList = GetDlgItem(hDlg, IDC_TILE_LIST);
			list<CSprite*> pList = pEditorLevel->GetSpriteList();
			list<CSprite*>::iterator iter = pList.begin();

			for (; iter != pList.end(); ++iter)
			{
				CSprite* pSprite = *iter;
				wstring spriteKey = pSprite->GetKey();
				SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)spriteKey.c_str());
			}
			//SendMessage(hwndList, LB_SETCURSEL, 0, 0);
			//pEditorLevel->SetDrawSprite(true);
		}

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			/*
			// ����Ʈ ��Ʈ�ѿ� �Է��� ���ڸ� �˾Ƴ�����
			//int Row = GetDlgItemInt(hDlg, IDC_ROW, nullptr, true);
			//int Col = GetDlgItemInt(hDlg, IDC_COL, nullptr, true);

			//// CLevel_Editor �� �ִ� MapObject �� Ÿ�ϸ� ������Ʈ�� �� ���� �������־�� ��
			//// ���� ������ �˾Ƴ���. ��Ȳ�� ���� ������ �ݵ�� CLevel_Editor ���� �Ѵ�.
			//CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			//CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
			//assert(pEditorLevel);

			//// Editor ������ ������ ���� CMap ������Ʈ�� ������
			//CMap* pMapObj = pEditorLevel->GetMapObject();

			//// Ÿ�ϸ� ������Ʈ�� ��ġ�� ����Ʈ��Ʈ�ѿ� �Էµ� ���ڷ� ������
			//CTileMap* pTileMap = pMapObj->GetComponent<CTileMap>();
			//pTileMap->SetRowCol(Row, Col);

			//EndDialog(hDlg, LOWORD(wParam));
			*/
			//pEditorLevel->SaveRoom();

			pEditorLevel->SetDlgHandle(nullptr);
			pEditorLevel->SetCurSprite(nullptr);
			EndDialog(hDlg, LOWORD(wParam));
		}

		else if (LOWORD(wParam) == IDCANCEL)
		{
			pEditorLevel->SetDlgHandle(nullptr);
			pEditorLevel->SetCurSprite(nullptr);
			EndDialog(hDlg, LOWORD(wParam));
		}

		else if (LOWORD(wParam) == IDC_ADD_TILE)
		{
			pEditorLevel->LoadSprite();

			HWND hwndList = GetDlgItem(hDlg, IDC_TILE_LIST);
			wstring spriteKey = pEditorLevel->GetSpriteList().back()->GetKey();
			int pos = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			pos = (int)SendMessage(hwndList, LB_INSERTSTRING, pos + 1, (LPARAM)spriteKey.c_str());
			SendMessage(hwndList, LB_SETCURSEL, pos, 0);
		}

		else if (LOWORD(wParam) == IDC_DELETE_TILE)
		{
			HWND hwndList = GetDlgItem(hDlg, IDC_TILE_LIST);
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

		else if (LOWORD(wParam) == IDC_TILE_LIST)
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			case LB_SETCURSEL:
			{
				HWND hwndList = GetDlgItem(hDlg, IDC_TILE_LIST);

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