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

    // �� ǥ���� sprite
    CSprite*    m_wallSprite;

    // �� ����
    ROOM_INFO   m_roomInfo;

    // ���� ��
    CRoom*      m_room;

    // �׷��� obj ��������Ʈ
    list<CSprite*>  m_SpriteList;
    CSprite*        m_curSprite;
    CSprite* m_tempSprite;

    HWND            m_hDlgHandle;   // ��޸��� ���̾�α� �ڵ� �ޱ�

    bool            m_bDrawSprite;          // Picture Control �� Sprite �� �ٽ� �׷��� �ϴ°�

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

