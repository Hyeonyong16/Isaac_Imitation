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
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

public:
    int GetCol() { return m_Col; }
    int GetRow() { return m_Row; }
    void SetRowCol(int _Row, int _Col);

    tObjInfo GetIdxObjInfo(int _idx) { return m_vecRoomObjList[_idx]; }
    tObjInfo* GetObjInfo(Vec2 _MousePos);

public:
    bool Save(const wstring& _FullPath);
    bool Load(const wstring& _FullPath);


public:
    CRoom();
    ~CRoom();
};

