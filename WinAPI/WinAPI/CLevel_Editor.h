#pragma once
#include "CLevel.h"

class CLevel_Editor :
    public CLevel
{
private:
    class CMap* m_MapObj;
    HMENU       m_hMenu;

public:
    CMap* GetMapObject() { return m_MapObj; }


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void End() override;

public:
    void SaveTileMap();
    void LoadTileMap();

public:
    CLevel_Editor();
    ~CLevel_Editor();
};

