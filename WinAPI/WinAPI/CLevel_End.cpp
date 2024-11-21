#include "pch.h"
#include "CLevel_End.h"

#include "CEngine.h"
#include "CSprite.h"

#include "EndingObj.h"


#include "CPathMgr.h"
#include "CCamera.h"

#include "CAssetMgr.h"
#include "CSound.h"

CLevel_End::CLevel_End()
	: endObj(nullptr)
{
	m_wallSprite = CAssetMgr::GetInst()->LoadSprite(L"MAP_WALL", L"Sprite\\MAP_WALL.sprite");
}

CLevel_End::~CLevel_End()
{
}

void CLevel_End::Begin()
{
    // 배경음 지정
    CSound* pBGM = CAssetMgr::GetInst()->LoadSound(L"EndingBGM", L"Sound\\Isaac_Ending_BGM.wav");
    if (nullptr != pBGM)
    {
        pBGM->SetVolume(70.f);
        pBGM->PlayToBGM(true);
    }

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
    CCamera::GetInst()->SetLookAt(Vec2(vResolution.x / 2, vResolution.y / 2));


	CObj* endingObj = new EndingObj;
	endingObj->SetPos(vResolution.x / 2.f, vResolution.y / 2.f);
	endingObj->SetScale(1.f, 1.f);

	AddObject(endingObj, LAYER_TYPE::ROCK);

	if (!endObj) endObj = dynamic_cast<EndingObj*>(endingObj);
}

void CLevel_End::Tick()
{
	CLevel::Tick();

	if (endObj->GetSceneChange())
	{
		ChangeLevel(LEVEL_TYPE::START);
	}
}

void CLevel_End::Render()
{
    Vec2 vResolution = CEngine::GetInst()->GetResolution();
    HDC hBackDC = CEngine::GetInst()->GetSecondDC();


    // 배경 그리기
    {
        Vec2 renderPos = CCamera::GetInst()->GetRenderPos(Vec2(0.f, 0.f));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , vResolution.x / 2, vResolution.y / 2
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);

        renderPos = CCamera::GetInst()->GetRenderPos(Vec2((vResolution.x) - 1, 0.f));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , -1 * (vResolution.x / 2), vResolution.y / 2
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);

        renderPos = CCamera::GetInst()->GetRenderPos(Vec2(0.f, vResolution.y - 1));
        StretchBlt(hBackDC
            , renderPos.x
            , renderPos.y
            , vResolution.x / 2, -1 * (vResolution.y / 2)
            , m_wallSprite->GetAtlas()->GetDC()
            , m_wallSprite->GetLeftTop().x, m_wallSprite->GetLeftTop().y
            , m_wallSprite->GetSlice().x, m_wallSprite->GetSlice().y
            , SRCCOPY);

        renderPos = CCamera::GetInst()->GetRenderPos(Vec2((vResolution.x) - 1, vResolution.y - 1));
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
}

void CLevel_End::End()
{
    endObj = nullptr;
	DeleteAllObject();
}
