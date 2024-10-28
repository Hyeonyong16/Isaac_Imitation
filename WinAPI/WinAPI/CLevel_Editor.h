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
    CSprite* m_tempSprite;

    HWND            m_hDlgHandle;   // 모달리스 다이얼로그 핸들 받기

    bool            m_bDrawSprite;          // Picture Control 에 Sprite 를 다시 그려야 하는가

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

    list<CSprite*> GetSpriteList() { return m_SpriteList; }

    void SetCurSprite(CSprite* _sprite) { m_curSprite = _sprite; }
    CSprite* GetCurSprite() { return m_curSprite; }

    bool GetDrawSprite() { return m_bDrawSprite; }
    void SetDrawSprite(bool _drawSprite) { m_bDrawSprite = _drawSprite; }

    void DeleteIdxSpriteList(int _idx)
    {
        list<CSprite*>::iterator iter = m_SpriteList.begin();

        for (int i = 0; i < _idx; ++i)
        {
            ++iter;
        }
        m_SpriteList.erase(iter);
    }

public:
    void SaveTileMap();
    void LoadTileMap();

    void LoadSprite();
    void SaveRoom();
    void LoadRoom();


public:
    CLevel_Editor();
    ~CLevel_Editor();
};

