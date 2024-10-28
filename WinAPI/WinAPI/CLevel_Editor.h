#pragma once
#include "CLevel.h"

class CSprite;
class CRoom;

class CLevel_Editor :
    public CLevel
{
private:
    class CMap* m_MapObj;
    HMENU       m_hMenu;

    // 벽 표시할 sprite
    CSprite*    m_wallSprite;

    // 방 종류
    ROOM_INFO   m_roomInfo;

    // 만들 방
    CRoom*      m_room;

    // 그려질 obj 스프라이트
    list<CSprite*>  m_SpriteList;
    CSprite*        m_curSprite;

    HWND            m_hDlgHandle;   // 모달리스 다이얼로그 핸들 받기

public:
    CMap* GetMapObject() { return m_MapObj; }


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void End() override;

public:
    void SetDlgHandle(HWND _hwnd) { m_hDlgHandle = _hwnd; }
    HWND GetDlgHandle() { return m_hDlgHandle; }

public:
    void SaveTileMap();
    void LoadTileMap();

public:
    CLevel_Editor();
    ~CLevel_Editor();
};

