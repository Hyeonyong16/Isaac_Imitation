#include "pch.h"
#include "CLevel_Start.h"

#include "CEngine.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCharger.h"
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

CLevel_Start::CLevel_Start()
{
}

CLevel_Start::~CLevel_Start()
{
}

void CLevel_Start::Begin()
{
    // ����� ����
    CSound* pBGM = CAssetMgr::GetInst()->LoadSound(L"BattleBGM", L"Sound\\BGM_Stage1.wav");
    if (nullptr != pBGM)
    {
        pBGM->SetVolume(70.f);
        pBGM->PlayToBGM(true);
    }

    // ȭ�� �ػ� 
    Vec2 vResolution = CEngine::GetInst()->GetResolution();

    // Player ����
    CObj* pObject = new CPlayer;
    pObject->SetName(L"Player");
    pObject->SetPos(vResolution.x / 2.f, 100.f);
    pObject->SetScale(50.f, 50.f);

    AddObject(pObject, LAYER_TYPE::PLAYER);

    // Monster ����
    CCharger* pMonster = new CCharger;
    pMonster->SetName(L"CCharger");
    pMonster->SetPos(300.f, 120.f);
    pMonster->SetScale(30.f, 30.f);
    AddObject(pMonster, LAYER_TYPE::MONSTER);

    /*pMonster = new CMonster;
    pMonster->SetName(L"Monster");
    pMonster->SetPos(1000.f, 500.f);
    pMonster->SetScale(100.f, 100.f);
    pMonster->SetDistance(200.f);
    pMonster->SetSpeed(300.f);
    AddObject(pMonster, LAYER_TYPE::MONSTER);*/

    // Platform Object �߰�
    CPlatform* pPlatform = new CPlatform;
    pPlatform->SetPos(vResolution.x / 2.f, 0.f);
    pPlatform->SetColliderScale(Vec2(vResolution.x, 30.f));
    AddObject(pPlatform, LAYER_TYPE::TILE);

    pPlatform = new CPlatform;
    pPlatform->SetPos(vResolution.x / 2.f, vResolution.y);
    pPlatform->SetColliderScale(Vec2(vResolution.x, 30.f));

    AddObject(pPlatform, LAYER_TYPE::TILE);

    pPlatform = new CPlatform;
    pPlatform->SetPos(0, vResolution.y / 2.f);
    pPlatform->SetColliderScale(Vec2(30.f, vResolution.y));
    AddObject(pPlatform, LAYER_TYPE::TILE);

    pPlatform = new CPlatform;
    pPlatform->SetPos(vResolution.x, vResolution.y / 2.f);
    pPlatform->SetColliderScale(Vec2(30.f, vResolution.y));
    AddObject(pPlatform, LAYER_TYPE::TILE);

    // TileMap Object �߰�
    //CMap* pTileMap = new CMap;
    //pTileMap->SetPos(Vec2(0.f, 0.f));
    //wstring FilePath = CPathMgr::GetContentPath();
    //pTileMap->GetTileMap()->LoadTileMap(FilePath + L"TileMap\\tempMap1.tile");
    //AddObject(pTileMap, LAYER_TYPE::TILE);

    //// TileMap Object �߰�
    //pTileMap = new CMap;
    //pTileMap->SetPos(Vec2(1000.f, 1000.f));
    //pTileMap->GetTileMap()->LoadTileMap(FilePath + L"TileMap\\tempMap1.tile");
    //AddObject(pTileMap, LAYER_TYPE::TILE);

    // �浹 ����
    CCollisionMgr::GetInst()->CollisionCheckClear();
    CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER_OBJECT, LAYER_TYPE::MONSTER);
    CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::MONSTER);
    CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::PLAYER, LAYER_TYPE::TILE);
    CCollisionMgr::GetInst()->CollisionCheck(LAYER_TYPE::MONSTER, LAYER_TYPE::TILE);

    // Camera ȿ��
    CCamera::GetInst()->PostProcessEffect(HEART, 0.5f);

    // �θ� CLevel �� Begin ȣ��
    CLevel::Begin();
}

void CLevel_Start::Render()
{
    CLevel::Render();

    TextOut(CEngine::GetInst()->GetSecondDC(), 10, 10, L"Start Level", wcslen(L"Start Level"));
}

void CLevel_Start::End()
{
    DeleteAllObject();
}
