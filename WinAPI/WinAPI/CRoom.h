#pragma once
#include "CObj.h"

class CRoom :
    public CObj
{
private:
    int m_Col;
    int m_Row;

    vector<tObjInfo>    m_vecRoomObjList;   // 현재 방에 있는 Object List
    tRoomInfo           m_roomInfo;

public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    CRoom();
    ~CRoom();
};

