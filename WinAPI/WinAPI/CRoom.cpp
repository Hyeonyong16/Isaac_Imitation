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
	// ������
	// ���� ���
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
	// �����Ͱ� �ƴϸ� �׳� �ϰ͵� ����
	// map �� ������ �Ѱ��ְ� �������� �� map ������ ������Ʈ ������ ����
	// ��ġ�� �ش� Ÿ�� + �� ��ü�� ��ġ
	// ������Ʈ ��ü�� ���� �� ���� ������ ������ �������� ����
}

