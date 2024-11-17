#include "pch.h"
#include "CDoor.h"

#include "CEngine.h"

#include "CRigidBody.h"
#include "CCollider.h"
#include "CSprite.h"

#include "CDbgRender.h"

#include "CAssetMgr.h"

#include "CLevelMgr.h"
#include "CLevel_Start.h"

CDoor::CDoor()
    : m_doorCollider(nullptr)
    , m_doorPos('u')
    , m_isOpen(false)
{
    m_doorSprite = CAssetMgr::GetInst()->LoadSprite(L"BASIC_DOOR", L"Sprite\\BASIC_DOOR.sprite");
    m_doorCloseLSprite = CAssetMgr::GetInst()->LoadSprite(L"BASIC_DOOR_LEFT", L"Sprite\\BASIC_DOOR_LEFT.sprite");
    m_doorCloseRSprite = CAssetMgr::GetInst()->LoadSprite(L"BASIC_DOOR_RIGHT", L"Sprite\\BASIC_DOOR_RIGHT.sprite");
    m_doorOpenSprite = CAssetMgr::GetInst()->LoadSprite(L"BASIC_DOOR_OPEN", L"Sprite\\BASIC_DOOR_OPEN.sprite");

    m_doorCollider = new CCollider;
    m_doorCollider->SetScale(Vec2(124.f, 155.f));

    AddComponent(m_doorCollider);
}

CDoor::~CDoor()
{
}

void CDoor::Begin()
{
    m_isOpen = false;

    Vec2 vResolution = CEngine::GetInst()->GetResolution();

    if (m_doorPos == 'u')
    {
        SetPos(Vec2(vResolution.x / 2 - 62.f, vResolution.y * m_curRoomPos + 75.f));
        m_doorCollider->SetOffset(Vec2(0.f, 40.f));
    }
    else
    {
        SetPos(Vec2(vResolution.x / 2 - 62.f, vResolution.y * (m_curRoomPos + 1) - 75.f));
        m_doorCollider->SetOffset(Vec2(0.f, -40.f));
    }
}

void CDoor::Tick()
{
    CLevel_Start* curLevel = dynamic_cast<CLevel_Start*>(CLevelMgr::GetInst()->GetCurrentLevel());
    assert(curLevel);

    if (!m_isOpen && curLevel->monCountInRoom(m_curRoomPos) <= 0)
    {
        m_isOpen = true;
    }
}

void CDoor::Render()
{
    // Sprite 를 화면에 그린다.
    HDC hBackDC = CEngine::GetInst()->GetSecondDC();
    HDC hInverseDC = CEngine::GetInst()->GetInverseDC();

    BLENDFUNCTION blend = {};

    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;

    if (m_doorPos == 'u')
    {
        if (m_isOpen)
        {
            AlphaBlend(hBackDC, GetPos().x - 62.f, GetPos().y - 30.f, 124.f, 150.f
                , m_doorOpenSprite->GetAtlas()->GetDC()
                , m_doorOpenSprite->GetLeftTop().x, m_doorOpenSprite->GetLeftTop().y
                , m_doorOpenSprite->GetSlice().x, m_doorOpenSprite->GetSlice().y
                , blend);
        }

        else
        {
            AlphaBlend(hBackDC, GetPos().x - 62.f, GetPos().y - 30.f, 62.f, 150.f
                , m_doorCloseLSprite->GetAtlas()->GetDC()
                , m_doorCloseLSprite->GetLeftTop().x, m_doorCloseLSprite->GetLeftTop().y
                , m_doorCloseLSprite->GetSlice().x, m_doorCloseLSprite->GetSlice().y
                , blend);

            AlphaBlend(hBackDC, GetPos().x, GetPos().y - 30.f, 62.f, 150.f
                , m_doorCloseRSprite->GetAtlas()->GetDC()
                , m_doorCloseRSprite->GetLeftTop().x, m_doorCloseRSprite->GetLeftTop().y
                , m_doorCloseRSprite->GetSlice().x, m_doorCloseRSprite->GetSlice().y
                , blend);
        }

        AlphaBlend(hBackDC, GetPos().x - 62.f, GetPos().y - 30.f, 124.f, 150.f
            , m_doorSprite->GetAtlas()->GetDC()
            , m_doorSprite->GetLeftTop().x, m_doorSprite->GetLeftTop().y
            , m_doorSprite->GetSlice().x, m_doorSprite->GetSlice().y
            , blend);

    }
    else
    {

        if (m_isOpen)
        {
            StretchBlt(hInverseDC
                , m_doorOpenSprite->GetSlice().x - 1
                , m_doorOpenSprite->GetSlice().y - 1
                , -1 * m_doorOpenSprite->GetSlice().x
                , -1 * m_doorOpenSprite->GetSlice().y
                , m_doorOpenSprite->GetAtlas()->GetDC()
                , m_doorOpenSprite->GetLeftTop().x
                , m_doorOpenSprite->GetLeftTop().y
                , m_doorOpenSprite->GetSlice().x
                , m_doorOpenSprite->GetSlice().y
                , SRCCOPY);

            AlphaBlend(hBackDC, GetPos().x - 62.f, GetPos().y - 115.f, 124.f, 150.f
                , hInverseDC
                , 0.f, 0.f
                , m_doorSprite->GetSlice().x, m_doorSprite->GetSlice().y
                , blend);

            CEngine::GetInst()->ClearInverseBuffer();
        }

        else
        {
            StretchBlt(hInverseDC
                , m_doorCloseLSprite->GetSlice().x - 1
                , m_doorCloseLSprite->GetSlice().y - 1
                , -1 * m_doorCloseLSprite->GetSlice().x
                , -1 * m_doorCloseLSprite->GetSlice().y
                , m_doorCloseLSprite->GetAtlas()->GetDC()
                , m_doorCloseLSprite->GetLeftTop().x
                , m_doorCloseLSprite->GetLeftTop().y
                , m_doorCloseLSprite->GetSlice().x
                , m_doorCloseLSprite->GetSlice().y
                , SRCCOPY);

            AlphaBlend(hBackDC, GetPos().x, GetPos().y - 115.f, 62.f, 150.f
                , hInverseDC
                , 0.f, 0.f
                , m_doorCloseLSprite->GetSlice().x, m_doorCloseLSprite->GetSlice().y
                , blend);

            CEngine::GetInst()->ClearInverseBuffer();

            StretchBlt(hInverseDC
                , m_doorCloseRSprite->GetSlice().x - 1
                , m_doorCloseRSprite->GetSlice().y - 1
                , -1 * m_doorCloseRSprite->GetSlice().x
                , -1 * m_doorCloseRSprite->GetSlice().y
                , m_doorCloseRSprite->GetAtlas()->GetDC()
                , m_doorCloseRSprite->GetLeftTop().x
                , m_doorCloseRSprite->GetLeftTop().y
                , m_doorCloseRSprite->GetSlice().x
                , m_doorCloseRSprite->GetSlice().y
                , SRCCOPY);

            AlphaBlend(hBackDC, GetPos().x - 62.f, GetPos().y - 115.f, 62.f, 150.f
                , hInverseDC
                , 0.f, 0.f
                , m_doorCloseRSprite->GetSlice().x, m_doorCloseRSprite->GetSlice().y
                , blend);

            CEngine::GetInst()->ClearInverseBuffer();
        }

        StretchBlt(hInverseDC
            , m_doorSprite->GetSlice().x - 1
            , m_doorSprite->GetSlice().y - 1
            , -1 * m_doorSprite->GetSlice().x
            , -1 * m_doorSprite->GetSlice().y
            , m_doorSprite->GetAtlas()->GetDC()
            , m_doorSprite->GetLeftTop().x
            , m_doorSprite->GetLeftTop().y
            , m_doorSprite->GetSlice().x
            , m_doorSprite->GetSlice().y
            , SRCCOPY);

        AlphaBlend(hBackDC, GetPos().x - 62.f, GetPos().y - 115.f, 124.f, 150.f
            , hInverseDC
            , 0.f, 0.f
            , m_doorSprite->GetSlice().x, m_doorSprite->GetSlice().y
            , blend);
        CEngine::GetInst()->ClearInverseBuffer();


    }
}

void CDoor::BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider)
{
}

