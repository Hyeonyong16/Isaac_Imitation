#pragma once
#include "CLevel.h"

class CRoom;
class CMonster;
class CSprite;

struct roomInfo
{
public:
    int RoomPos;
    bool IsClear;
    bool IsActive;
    int monCount;
};

class CLevel_Start :
    public CLevel
{
    // 벽 표시할 sprite
    CSprite* m_wallSprite;

    int m_PlayerRoomLocation;
    
    vector<roomInfo*> m_levelRoomInfo;
    bool m_roomMap[4];

    int m_curLevelMonsterObjectCount;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void End() override;

public:
    int monCountInRoom(int _roomPos)
    {
        return m_levelRoomInfo[_roomPos]->monCount;
    }

public:
    CLevel_Start();
    ~CLevel_Start();
};

