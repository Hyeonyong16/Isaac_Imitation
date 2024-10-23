#include "pch.h"
#include "CMap.h"

#include "CAssetMgr.h"
#include "CTexture.h"
#include "CTileMap.h"


CMap::CMap()
	: m_TileMap(nullptr)
{
	m_TileMap = (CTileMap*)AddComponent(new CTileMap);
	m_TileMap->SetRowCol(1, 1);
	m_TileMap->SetAtlasTexture(CAssetMgr::GetInst()->LoadTexture(L"TileMapAtlas", L"Texture\\TILE.bmp"));
}

CMap::~CMap()
{
}

void CMap::Tick()
{
}

void CMap::Render()
{
	m_TileMap->Render();
}

