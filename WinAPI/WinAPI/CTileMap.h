#pragma once
#include "CComponent.h"

class CTexture;

class CTileMap :
    public CComponent
{
private:
    int         m_Col;              // 타일 열 개수
    int         m_Row;              // 타일 행 개수    

    CTexture*   m_Atlas;
    int         m_AtlasTileCol;     // 아틀라스 텍스쳐 안에 있는 타일 열 개수
    int         m_AtlasTileRow;     // 아틀라스 텍스쳐 안에 있는 타일 행 개수
    Vec2        m_AtlasResolution;  // 아틀라스 해상도

    // 각 타일마다 자신이 원하는 이미지를 고를 수 있어야함
    vector<tTile>   m_vecTileInfo;

public:
    void SetRowCol(int Row, int Col);
    void SetAtlasTexture(CTexture* _Atlas);
    tTile* GetTileInfo(Vec2 _MousePos);

public:
    bool SaveTileMap(wstring _FullPath);
    bool LoadTileMap(wstring _FullPath);


public:
    virtual void FinalTick() override;
    void Render();

public:
    CTileMap();
    ~CTileMap();
};

