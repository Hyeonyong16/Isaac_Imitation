#include "pch.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLevel_Start.h"
#include "CLevel_Editor.h"
#include "CLevel_Editor_Sprite.h"

CLevelMgr::CLevelMgr()
	: m_arrLevel{}
	, m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	Delete_Array(m_arrLevel);
}

void CLevelMgr::Init()
{
    // 레벨 제작
    m_arrLevel[(UINT)LEVEL_TYPE::START] = new CLevel_Start;
    m_arrLevel[(UINT)LEVEL_TYPE::EDITOR_TILE] = new CLevel_Editor;
    m_arrLevel[(UINT)LEVEL_TYPE::EDITOR_SPRITE] = new CLevel_Editor_Sprite;


    // 생성한 레벨을 START 레벨이면서 현재 재생중인 레벨로 설정 > Begin 호출
    m_CurLevel = m_arrLevel[(UINT)LEVEL_TYPE::EDITOR_TILE];
    m_CurLevel->Begin();
    
}

void CLevelMgr::Progress()
{
    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->Tick();
    m_CurLevel->FinalTick();
}

void CLevelMgr::Render()
{
    m_CurLevel->Render();
}

void CLevelMgr::ChangeLevel(LEVEL_TYPE _NextLevel)
{
    // 1. 현재 레벨과 변경하려는 레벨이 동일하면 아무일도 안일어난다.
    if (m_CurLevel == m_arrLevel[(UINT)_NextLevel])
        return;

    // 2. 현재 레벨에 End 를 호출시킨다.
    m_CurLevel->End();

    // 3. 변경 요청한 레벨을 현재 레벨로 지정한다.
    m_CurLevel = m_arrLevel[(UINT)_NextLevel];

    // 3. 변경된 레벨의 Begin 을 호출시킨다.
    m_CurLevel->Begin();
}


CObj* CLevelMgr::FindObjectByName(LAYER_TYPE _Layer, const wstring& _Name)
{
    if (nullptr == m_CurLevel)
        return nullptr;

    return m_CurLevel->FindObjectByName(_Layer, _Name);
}