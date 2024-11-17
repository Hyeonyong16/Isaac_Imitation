#include "pch.h"
#include "CRoom.h"

#include "CPathMgr.h"

#include "CDbgRender.h"
#include "CLevelMgr.h"
#include "CLevel_Editor.h"
#include "CEngine.h"
#include "CKeyMgr.h"
#include "CCamera.h"

#include "CAssetMgr.h"


CRoom::CRoom()
	: m_Col(12)
	, m_Row(7)
{

}

CRoom::~CRoom()
{
	for (int i = 0; i < m_vecRoomObjList.size(); ++i)
	{
		m_vecRoomObjList[i].ObjTypeKey = L"\0";
	}
}

void CRoom::Begin()
{
	m_vecRoomObjList.resize(m_Row * m_Col);
	for (int i = 0; i < m_vecRoomObjList.size(); ++i)
		m_vecRoomObjList[i].ObjTypeKey = L"NOTHING";
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

void CRoom::SetRowCol(int _Row, int _Col)
{
	m_Row = _Row;
	m_Col = _Col;

	if(m_vecRoomObjList.size() < m_Row * m_Col)
		m_vecRoomObjList.resize(m_Row * m_Col);

	for (int i = 0; i < m_vecRoomObjList.size(); ++i)
		m_vecRoomObjList[i].ObjTypeKey = L"NOTHING";
}

tObjInfo* CRoom::GetObjInfo(Vec2 _MousePos)
{
	if (CKeyMgr::GetInst()->IsMouseOffScreen())
		return nullptr;

	_MousePos = CCamera::GetInst()->GetRealPos(_MousePos);
	Vec2 vOffset = _MousePos - GetPos();

	// 마우스 위치가 타일맵이 보유한 타일 중에서 몇행 몇열 위치인지 알아낸다.
	int Col = (int)vOffset.x / TILE_SIZE_X;
	int Row = (int)vOffset.y / TILE_SIZE_Y;

	if (Col < 0 || Row < 0 || m_Col <= Col || m_Row <= Row)
		return nullptr;

	int idx = Row * m_Col + Col;
	return &m_vecRoomObjList[idx];
}

bool CRoom::Save(const wstring& _FullPath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FullPath.c_str(), L"wb");
	assert(pFile);

	// 타일 행, 렬 개수 정보
	fwrite(&m_Col, sizeof(int), 1, pFile);
	fwrite(&m_Row, sizeof(int), 1, pFile);

	// 각각의 타일이 사용하던 이미지 인덱스 정보
	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fwrite(&m_vecRoomObjList[i], sizeof(tObjInfo), 1, pFile);
	}

	fwrite(&m_roomInfo, sizeof(tRoomInfo), 1, pFile);

	fclose(pFile);

	return true;
}

bool CRoom::Load(const wstring& _FullPath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FullPath.c_str(), L"rb");
	assert(pFile);

	// 타일 행, 렬 개수 정보
	fread(&m_Col, sizeof(int), 1, pFile);
	fread(&m_Row, sizeof(int), 1, pFile);
	SetRowCol(m_Row, m_Col);

	// 각각의 타일이 사용하던 이미지 인덱스 정보
	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fread(&m_vecRoomObjList[i], sizeof(tObjInfo), 1, pFile);
	}

	fread(&m_roomInfo, sizeof(tRoomInfo), 1, pFile);

	fclose(pFile);

	return true;
}

