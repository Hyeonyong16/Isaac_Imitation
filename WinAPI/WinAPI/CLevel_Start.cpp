#include "pch.h"
#include "CLevel_Start.h"

#include "CEngine.h"
#include "CSprite.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCharger.h"
#include "CPooter.h"
#include "CMonstro.h"
#include "CRock.h"
#include "CMissile.h"
#include "CPlatform.h"
#include "CMap.h"

#include "CCollisionMgr.h"
#include "CCollider.h"

#include "CTileMap.h"
#include "CPathMgr.h"
#include "CCamera.h"

#include "CAssetMgr.h"
#include "CSound.h"

#include "CRoom.h"
#include "CDoor.h"
#include "CHPUI.h"

CLevel_Start::CLevel_Start()
    : m_PlayerRoomLocation(0)
    , m_isCameraMove(false)
{
    m_wallSprite = CAssetMgr::GetInst()->LoadSprite(L"MAP_WALL", L"Sprite\\MAP_WALL.sprite");

    for (int i = 0; i < 4; ++i)
    {
        m_roomMap[i] = false;
    }
}

CLevel_Start::~CLevel_Start()
{
    Delete_Vector(m_levelRoomInfo);
}

void CLevel_Start::Begin()
{
    m_roomMap[0] = true;

    {
        // 배경음 지정
        CSound* pBGM = CAssetMgr::GetInst()->LoadSound(L"BattleBGM", L"Sound\\BGM_Stage1.wav");
        if (nullptr != pBGM)
        {
            pBGM->SetVolume(70.f);
            pBGM->PlayToBGM(true);
        }

        // 화면 해상도 
        Vec2 vResolution = CEngine::GetInst()->GetResolution();

        // Player 생성
        CObj* pObject = new CPlayer;
        pObject->SetName(L"Player");
        pObject->SetPos(vResolution.x / 2.f, vResolution.y / 2.f);
        pObject->SetScale(50.f, 50.f);

        AddObject(pObject, LAYER_TYPE::PLAYER);

        roomInfo* pRoom = new roomInfo;

        Vec2 tempMonPos = Vec2(210.f, 180.f + (pRoom->RoomPos * (vResolution.y)));
        //0 번 방
        {
            pRoom->RoomPos = 0;
            pRoom->IsClear = true;
            pRoom->IsActive = true;

            CDoor* pDoor = new CDoor;
            pDoor->SetCurRoomPos(0);
            pDoor->SetDoorPos('d');
            AddObject(pDoor, LAYER_TYPE::DOOR);

            pRoom->monCount = 0;

            m_levelRoomInfo.push_back(pRoom);
        }

        // 1번방
        {
            pRoom = new roomInfo;
            pRoom->RoomPos = 1;
            pRoom->IsClear = false;
            pRoom->IsActive = false;

            CDoor* pDoor = new CDoor;
            pDoor->SetCurRoomPos(1);
            pDoor->SetDoorPos('u');
            AddObject(pDoor, LAYER_TYPE::DOOR);

            pDoor = new CDoor;
            pDoor->SetCurRoomPos(1);
            pDoor->SetDoorPos('d');
            AddObject(pDoor, LAYER_TYPE::DOOR);

            // Monster 생성
            // Charger
            tempMonPos = Vec2(210.f, 180.f + (pRoom->RoomPos * (vResolution.y)));

            CMonster* pMonster = new CCharger;
            pMonster->SetName(L"CCharger");
            pMonster->SetPos(tempMonPos.x + 323.f, tempMonPos.y + 257.f);
            pMonster->SetScale(100.f, 100.f);
            pMonster->SetIsActive(pRoom->IsActive);
            pMonster->SetRoomNum(1);
            AddObject(pMonster, LAYER_TYPE::MONSTER);

            pMonster = new CCharger;
            pMonster->SetName(L"CCharger");
            pMonster->SetPos(tempMonPos.x + 573.f, tempMonPos.y + 360.f);
            pMonster->SetScale(100.f, 100.f);
            pMonster->SetIsActive(pRoom->IsActive);
            pMonster->SetRoomNum(1);
            CCharger* pCharger = dynamic_cast<CCharger*>(pMonster);
            pCharger->SetTurnDir('R');
            AddObject(pMonster, LAYER_TYPE::MONSTER);

            pMonster = new CCharger;
            pMonster->SetName(L"CCharger");
            pMonster->SetPos(tempMonPos.x + 823.f, tempMonPos.y + 463.f);
            pMonster->SetScale(100.f, 100.f);
            pMonster->SetIsActive(pRoom->IsActive);
            pMonster->SetRoomNum(1);
            AddObject(pMonster, LAYER_TYPE::MONSTER);

            pMonster = new CRock;
            pMonster->SetName(L"CRock");
            pMonster->SetPos(tempMonPos.x + 860.f, tempMonPos.y + 52.f);
            pMonster->SetScale(103.f, 103.f);
            AddObject(pMonster, LAYER_TYPE::ROCK);

            pMonster = new CRock;
            pMonster->SetName(L"CRock");
            pMonster->SetPos(tempMonPos.x + 63.f, tempMonPos.y + 257.f);
            pMonster->SetScale(103.f, 103.f);
            AddObject(pMonster, LAYER_TYPE::ROCK);

            pMonster = new CRock;
            pMonster->SetName(L"CRock");
            pMonster->SetPos(tempMonPos.x + 860.f, tempMonPos.y + 670.f);
            pMonster->SetScale(103.f, 103.f);
            AddObject(pMonster, LAYER_TYPE::ROCK);

            pRoom->monCount = 3;

            m_levelRoomInfo.push_back(pRoom);
        }

        // 2번방
        {
            pRoom = new roomInfo;
            pRoom->RoomPos = 2;
            pRoom->IsClear = false;
            pRoom->IsActive = false;

            CDoor* pDoor = new CDoor;
            pDoor->SetCurRoomPos(2);
            pDoor->SetDoorPos('u');
            AddObject(pDoor, LAYER_TYPE::DOOR);

            pDoor = new CDoor;
            pDoor->SetCurRoomPos(2);
            pDoor->SetDoorPos('d');
            AddObject(pDoor, LAYER_TYPE::DOOR);

            // Monster 생성
            // Charger
            tempMonPos = Vec2(210.f, 180.f + (pRoom->RoomPos * (vResolution.y)));

            CMonster* pMonster = new CPooter;
            pMonster->SetName(L"CPooter");
            pMonster->SetPos(tempMonPos.x + 323.f, tempMonPos.y + 257.f);
            pMonster->SetScale(100.f, 100.f);
            pMonster->SetIsActive(pRoom->IsActive);
            pMonster->SetRoomNum(2);
            AddObject(pMonster, LAYER_TYPE::MONSTER);

            pMonster = new CPooter;
            pMonster->SetName(L"CPooter");
            pMonster->SetPos(tempMonPos.x + 573.f, tempMonPos.y + 360.f);
            pMonster->SetScale(100.f, 100.f);
            pMonster->SetIsActive(pRoom->IsActive);
            pMonster->SetRoomNum(2);
            AddObject(pMonster, LAYER_TYPE::MONSTER);

            pMonster = new CPooter;
            pMonster->SetName(L"CPooter");
            pMonster->SetPos(tempMonPos.x + 823.f, tempMonPos.y + 463.f);
            pMonster->SetScale(100.f, 100.f);
            pMonster->SetIsActive(pRoom->IsActive);
            pMonster->SetRoomNum(2);
            AddObject(pMonster, LAYER_TYPE::MONSTER);

            pRoom->monCount = 3;

            m_levelRoomInfo.push_back(pRoom);
        }

        // Pooter
        /*CMonster* pMonster = new CPooter;
        pMonster->SetName(L"CPooter");
        pMonster->SetPos(500.f, 500.f);
        pMonster->SetScale(50.f, 50.f);
        AddObject(pMonster, LAYER_TYPE::MONSTER);*/

        // Monstro
        //CMonster* pMonster = new CMonstro;
        //pMonster->SetName(L"CMonstro");
        //pMonster->SetPos(500.f, 500.f);
        //pMonster->SetScale(150.f, 150.f);
        //AddObject(pMonster, LAYER_TYPE::MONSTER);

        // Rock
        /*pMonster = new CRock;
        pMonster->SetName(L"CRock");
        pMonster->SetPos(210.f + (TILE_SIZE_X / 2), 180.f + (TILE_SIZE_Y / 2));
        pMonster->SetScale(125.f, 103.f);
        AddObject(pMonster, LAYER_TYPE::ROCK);*/

        // Platform Object 추가
        CPlatform* pPlatform = new CPlatform;
        pPlatform->SetName(L"UP_WALL");
        pPlatform->SetPos(vResolution.x / 2.f, 0.f);
        pPlatform->SetColliderScale(Vec2(vResolution.x, 360.f));
        AddObject(pPlatform, LAYER_TYPE::TILE);

        pPlatform = new CPlatform;
        pPlatform->SetName(L"DOWN_WALL");
        pPlatform->SetPos(vResolution.x / 2.f, vResolution.y);
        pPlatform->SetColliderScale(Vec2(vResolution.x, 360.f));
        AddObject(pPlatform, LAYER_TYPE::TILE);

        pPlatform = new CPlatform;
        pPlatform->SetName(L"LEFT_WALL");
        pPlatform->SetPos(0, vResolution.y / 2.f);
        pPlatform->SetColliderScale(Vec2(420.f, vResolution.y));
        AddObject(pPlatform, LAYER_TYPE::TILE);

        pPlatform = new CPlatform;
        pPlatform->SetName(L"RIGHT_WALL");
        pPlatform->SetPos(vResolution.x, vResolution.y / 2.f);
        pPlatform->SetColliderScale(Vec2(420.f, vResolution.y));
        AddObject(pPlatform, LAYER_TYPE::TILE);

        // TileMap Object 추가
        //CMap* pTileMap = new CMap;
        //pTileMap->SetPos(Vec2(0.f, 0.f));
        //wstring FilePath = CPathMgr::GetContentPath();
        //pTileMap->GetTileMap()->LoadTileMap(FilePath + L"TileMap\\tempMap1.tile");
        //AddObject(pTileMap, LAYER_TYPE::TILE);

        //// TileMap Object 추가
        //pTileMap = new CMap;
        //pTileMap->SetPos(Vec2(1000.f, 1000.f));
        //pTileMap->GetTileMap()->LoadTileMap(FilePath + L"TileMap\\tempMap1.tile");
        //AddObject(pTileMap, LAYER_TYPE::TILE);

        CHPUI* pHPUI = new CHPUI;
        pHPUI->SetScale(Vec2(1.f, 1.f));
        pHPUI->SetPos(Vec2(20.f, 20.f));
        AddObject(pHPUI, LAYER_TYPE::UI);


        // 충돌 설정
        CCollisionMgr::GetInst()->CollisionCheckClear();
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER_OBJECT, LAYER_TYPE::MONSTER);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::MONSTER_OBJECT, LAYER_TYPE::PLAYER);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER_OBJECT, LAYER_TYPE::TILE);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::MONSTER);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TILE);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::MONSTER, LAYER_TYPE::TILE);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::ROCK);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::MONSTER, LAYER_TYPE::ROCK);
        CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::DOOR);

        // Camera 효과
        CCamera::GetInst()->PostProcessEffect(HEART, 0.5f);

        // 부모 CLevel 의 Begin 호출
        CLevel::Begin();

        m_curLevelMonsterObjectCount = GetObjectCount(LAYER_TYPE::MONSTER);
    }
}

void CLevel_Start::Tick()
{
    CLevel::Tick();

    vector<roomInfo*>::iterator iter = m_levelRoomInfo.begin();

    if (!m_isCameraMove)
    {
        Vec2 vResolution = CEngine::GetInst()->GetResolution();
        Vec2 camPos(vResolution.x / 2
            , (vResolution.y / 2) + (vResolution.y * m_PlayerRoomLocation));
        CCamera::GetInst()->SetLookAt(camPos);
        m_isCameraMove = true;

        CObj* pPlatform = FindObjectByName(LAYER_TYPE::TILE, L"UP_WALL");
        pPlatform->SetPos(vResolution.x / 2.f, m_PlayerRoomLocation * vResolution.y);
        pPlatform = FindObjectByName(LAYER_TYPE::TILE, L"DOWN_WALL");
        pPlatform->SetPos(vResolution.x / 2.f, (m_PlayerRoomLocation + 1) * vResolution.y);
        pPlatform = FindObjectByName(LAYER_TYPE::TILE, L"LEFT_WALL");
        pPlatform->SetPos(0.f, (vResolution.y * m_PlayerRoomLocation) + (vResolution.y / 2.f));
        pPlatform = FindObjectByName(LAYER_TYPE::TILE, L"RIGHT_WALL");
        pPlatform->SetPos(vResolution.x, (vResolution.y * m_PlayerRoomLocation) + (vResolution.y / 2.f));
    
        vector<CObj*>pMonsterList = GetObjects(LAYER_TYPE::MONSTER);
        vector<CObj*>::iterator iter = pMonsterList.begin();

        for (; iter != pMonsterList.end(); ++iter)
        {
            CMonster* pMonster = dynamic_cast<CMonster*>(*iter);
            if (pMonster->GetRoomNum() > m_PlayerRoomLocation) break;
            if (pMonster->GetRoomNum() == m_PlayerRoomLocation) pMonster->SetIsActive(true);
        }
    }

    for (; iter != m_levelRoomInfo.end(); ++iter)
    {
        if ((*iter)->RoomPos == m_PlayerRoomLocation)
        {
            if ((*iter)->IsClear == false && (*iter)->monCount == 0)
            {
                (*iter)->IsClear = true;
            }

            if (m_curLevelMonsterObjectCount > GetObjectCount(LAYER_TYPE::MONSTER))
            {
                (*iter)->monCount -= 1;
                m_curLevelMonsterObjectCount = GetObjectCount(LAYER_TYPE::MONSTER);
            }
        }
    }
}

void CLevel_Start::Render()
{
    Vec2 vResolution = CEngine::GetInst()->GetResolution();
    HDC hBackDC = CEngine::GetInst()->GetSecondDC();
    
    
    // 배경 그리기
    {
        Vec2 renderPos = CCamera::GetInst()->GetRenderPos(Vec2(0.f, m_PlayerRoomLocation * (vResolution.y)));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , vResolution.x / 2, vResolution.y / 2
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);

        renderPos = CCamera::GetInst()->GetRenderPos(Vec2((vResolution.x) - 1, m_PlayerRoomLocation * (vResolution.y)));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , -1 * (vResolution.x / 2), vResolution.y / 2
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);

        renderPos = CCamera::GetInst()->GetRenderPos(Vec2(0.f, (m_PlayerRoomLocation + 1) * (vResolution.y) - 1));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , vResolution.x / 2, -1 * (vResolution.y / 2)
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);

        renderPos = CCamera::GetInst()->GetRenderPos(Vec2((vResolution.x) - 1, (m_PlayerRoomLocation + 1) * (vResolution.y) - 1));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , -1 * (vResolution.x / 2), -1 * (vResolution.y / 2)
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);


    }

    CLevel::Render();

    TextOut(CEngine::GetInst()->GetSecondDC(), 10, 10, L"Start Level", wcslen(L"Start Level"));

    wchar_t str[255];
    swprintf_s(str, 255, L"monsterListSize: %d", m_levelRoomInfo[0]->monCount);
    TextOut(CEngine::GetInst()->GetSecondDC(), 10, 300, str, wcslen(str));
}

void CLevel_Start::End()
{
    DeleteAllObject();
}
