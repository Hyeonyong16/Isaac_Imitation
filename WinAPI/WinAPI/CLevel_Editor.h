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

    HWND            m_hDlgHandle;   // ��޸��� ���̾�α� �ڵ� �ޱ�

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

