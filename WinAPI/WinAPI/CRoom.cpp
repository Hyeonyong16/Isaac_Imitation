#include "pch.h"
#include "CRoom.h"

#include "CDbgRender.h"
#include "CLevelMgr.h"
#include "CLevel_Editor.h"
#include "CEngine.h"

CRoom::CRoom()
	: m_Col(12)
	, m_Row(7)
{
	m_vecRoomObjList.resize(m_Row * m_Col);
	for (int i = 0; i < m_vecRoomObjList.size(); ++i)
		m_vecRoomObjList[i].ObjTypeKey = L"NOTHING";
}

CRoom::~CRoom()
{
}

void CRoom::Tick()
{
	// 에디터
	// 격자 출력
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pLevel);
	if(pEditorLevel)
	{
		for (int row = 0; row < m_Row + 1; ++row)
		{
			DrawDebugLine(PEN_TYPE::GREEN
				, Vec2(210.f, 180.f) + Vec2(0, TILE_SIZE_Y * row)
				, Vec2(210.f, 180.f) + Vec2(m_Col * TILE_SIZE_X, TILE_SIZE_Y * row), 0.f);
		}

		for (int col = 0; col < m_Col + 1; ++col)
		{
			DrawDebugLine(PEN_TYPE::GREEN
				, Vec2(210.f, 180.f) + Vec2(TILE_SIZE_X * col, 0)
				, Vec2(210.f, 180.f) + Vec2(TILE_SIZE_X * col, m_Row * TILE_SIZE_Y), 0.f);
		}
	}
}

void CRoom::Render()
{
	// 에디터가 아니면 그냥 암것도 안함
	// map 에 정보를 넘겨주고 레벨에서 그 map 정보로 오브젝트 생성할 예정
	// 위치는 해당 타일 + 룸 자체의 위치
	// 오브젝트 자체는 구현 후 추후 각자의 정보로 레벨에서 생성
}

