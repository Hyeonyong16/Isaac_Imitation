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

	// ���콺 ��ġ�� Ÿ�ϸ��� ������ Ÿ�� �߿��� ���� � ��ġ���� �˾Ƴ���.
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

	// Ÿ�� ��, �� ���� ����
	fwrite(&m_Col, sizeof(int), 1, pFile);
	fwrite(&m_Row, sizeof(int), 1, pFile);

	// ������ Ÿ���� ����ϴ� �̹��� �ε��� ����
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

	// Ÿ�� ��, �� ���� ����
	fread(&m_Col, sizeof(int), 1, pFile);
	fread(&m_Row, sizeof(int), 1, pFile);
	SetRowCol(m_Row, m_Col);

	// ������ Ÿ���� ����ϴ� �̹��� �ε��� ����
	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fread(&m_vecRoomObjList[i], sizeof(tObjInfo), 1, pFile);
	}

	fread(&m_roomInfo, sizeof(tRoomInfo), 1, pFile);

	fclose(pFile);

	return true;
}

