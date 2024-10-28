#pragma once
#include "CObj.h"

class CTileMap;
class CRoom;

class CMap :
    public CObj
{
private:
    CTileMap* m_TileMap;
    vector<CRoom*> m_vecRoomList;


public:
    CTileMap* GetTileMap() { return m_TileMap; }
    virtual void Tick() override;
    virtual void Render() override;

public:
    CMap();
    ~CMap();
};

