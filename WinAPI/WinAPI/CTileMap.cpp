#include "pch.h"
#include "CTileMap.h"

#include "CKeyMgr.h"

#include "CEngine.h"
#include "CTexture.h"

#include "CCamera.h"

#include "CPathMgr.h"

CTileMap::CTileMap()
	: CComponent(COMPONENT_TYPE::TILEMAP)
	, m_Col(1)
	, m_Row(1)
	, m_Atlas(nullptr)
	, m_AtlasTileCol(0)
	, m_AtlasTileRow(0)
{
}

CTileMap::~CTileMap()
{
}

void CTileMap::FinalTick()
{
	Vec2 OwnerPos = GetOwner()->GetRenderPos();

	for (int row = 0; row < m_Row + 1; ++row)
	{
		DrawDebugLine(PEN_TYPE::GREEN
			, OwnerPos + Vec2(0, TILE_SIZE_Y * row)
			, OwnerPos + Vec2(m_Col * TILE_SIZE_X, TILE_SIZE_Y * row), 0.f);
	}

	for (int col = 0; col < m_Col + 1; ++col)
	{
		DrawDebugLine(PEN_TYPE::GREEN
			, OwnerPos + Vec2(TILE_SIZE_X * col, 0)
			, OwnerPos + Vec2(TILE_SIZE_X * col, m_Row * TILE_SIZE_Y), 0.f);
	}
}

void CTileMap::Render()
{
	if (nullptr == m_Atlas)
		return;

	Vec2 OwnerRenderPos = GetOwner()->GetRenderPos();
	HDC dc = CEngine::GetInst()->GetSecondDC();

	// 카메라 영역 안에 들어오는 행, 열을 계산하기
	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vCamLeftTop = vCamLook - (vResolution / 2.f);
	Vec2 vCamRightBot = vCamLook + (vResolution / 2.f);

	Vec2 vOwnerPos = GetOwner()->GetPos();
	vCamLeftTop = vCamLeftTop - vOwnerPos;

	int LeftTopCol = vCamLeftTop.x / TILE_SIZE_X;
	int LeftTopRow = vCamLeftTop.y / TILE_SIZE_Y;

	if (LeftTopCol < 0)
		LeftTopCol = 0;
	if (LeftTopRow < 0)
		LeftTopRow = 0;

	vCamRightBot = vCamRightBot - vOwnerPos;
	int RightBotCol = (vCamRightBot.x / TILE_SIZE_X) + 1;
	int RightBotRow = (vCamRightBot.y / TILE_SIZE_Y) + 1;

	if (m_Col < RightBotCol)
		RightBotCol = m_Col;
	if (m_Row < RightBotRow)
		RightBotRow = m_Row;

	for (int Row = LeftTopRow; Row < RightBotRow; ++Row)
	{
		for (int Col = LeftTopCol; Col < RightBotCol; ++Col)
		{
			// 반복문 회차에 맞는 행렬에 대해서 이게 몇번째 타일정보인지 1차원 인덱스로 변환
			// 해당 타일정보에 접근한다.
			int TileIdx = m_Col * Row + Col;
			int ImgIdx = m_vecTileInfo[TileIdx].ImgIdx;

			// 해당 타일의 ImgIdx 가 -1 인 경우, Blank Tile
			if (ImgIdx == -1)
				continue;

			int ImgRow = ImgIdx / m_AtlasTileCol; //  1 행
			int ImgCol = ImgIdx % m_AtlasTileCol; //   6 열

			assert(!(ImgIdx < 0 || m_AtlasTileCol * m_AtlasTileRow <= ImgIdx));

			StretchBlt(dc
				, (int)OwnerRenderPos.x + Col * TILE_SIZE_X
				, (int)OwnerRenderPos.y + Row * TILE_SIZE_Y
				, TILE_SIZE_X, TILE_SIZE_Y
				, m_Atlas->GetDC()
				, ImgCol * TILE_SIZE, ImgRow * TILE_SIZE
				, TILE_SIZE, TILE_SIZE
				, SRCCOPY);

			/*BitBlt(dc
				, (int)OwnerRenderPos.x + Col * TILE_SIZE_X
				, (int)OwnerRenderPos.y + Row * TILE_SIZE_Y
				, TILE_SIZE_X, TILE_SIZE_Y
				, m_Atlas->GetDC()
				, ImgCol * TILE_SIZE_X, ImgRow * TILE_SIZE_Y
				, SRCCOPY);*/
		}
	}
}

void CTileMap::SetRowCol(int Row, int Col)
{
	m_Row = Row;
	m_Col = Col;

	if (m_vecTileInfo.size() < m_Row * m_Col)
		m_vecTileInfo.resize(m_Row * m_Col);

	for (int i = 0; i < m_vecTileInfo.size(); ++i)
		m_vecTileInfo[i].ImgIdx = -1;
}

void CTileMap::SetAtlasTexture(CTexture* _Atlas)
{
	m_Atlas = _Atlas;

	if (nullptr == m_Atlas)
		return;

	// 아틀라스 텍스쳐 정보 가져오기
	m_AtlasResolution = Vec2((float)m_Atlas->GetWidth(), (float)m_Atlas->GetHeight());
	m_AtlasTileCol = m_AtlasResolution.x / TILE_SIZE;
	m_AtlasTileRow = m_AtlasResolution.y / TILE_SIZE;
}

tTile* CTileMap::GetTileInfo(Vec2 _MousePos)
{
	if (CKeyMgr::GetInst()->IsMouseOffScreen())
		return nullptr;

	_MousePos = CCamera::GetInst()->GetRealPos(_MousePos);
	Vec2 vOffset = _MousePos - GetOwner()->GetPos();

	// 마우스 위치가 타일맵이 보유한 타일 중에서 몇행 몇열 위치인지 알아낸다.
	int Col = (int)vOffset.x / TILE_SIZE_X;
	int Row = (int)vOffset.y / TILE_SIZE_Y;

	if (Col < 0 || Row < 0 || m_Col <= Col || m_Row <= Row)
		return nullptr;

	int idx = Row * m_Col + Col;
	return &m_vecTileInfo[idx];
}

bool CTileMap::SaveTileMap(wstring _FullPath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FullPath.c_str(), L"wb");
	assert(pFile);

	// 타일 행, 렬 개수 정보
	fwrite(&m_Col, sizeof(int), 1, pFile);
	fwrite(&m_Row, sizeof(int), 1, pFile);

	// 참조하던 아틀라스 텍스쳐 정보
	SaveAssetRef(m_Atlas, pFile);

	// 각각의 타일이 사용하던 이미지 인덱스 정보
	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fwrite(&m_vecTileInfo[i], sizeof(tTile), 1, pFile);
	}

	fclose(pFile);

	return true;
}

bool CTileMap::LoadTileMap(wstring _FullPath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FullPath.c_str(), L"rb");
	assert(pFile);

	// 타일 행, 렬 개수 정보
	fread(&m_Col, sizeof(int), 1, pFile);
	fread(&m_Row, sizeof(int), 1, pFile);
	SetRowCol(m_Row, m_Col);

	// 참조하던 아틀라스 텍스쳐 정보
	m_Atlas = (CTexture*)LoadAssetRef(pFile);
	SetAtlasTexture(m_Atlas);

	// 각각의 타일이 사용하던 이미지 인덱스 정보
	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fread(&m_vecTileInfo[i], sizeof(tTile), 1, pFile);
	}

	fclose(pFile);

	return true;
}
