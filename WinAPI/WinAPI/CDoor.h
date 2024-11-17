#pragma once
#include "CObj.h"

class CSprite;
class CCollider;

class CDoor :
    public CObj
{
private:
    int m_curRoomPos;  // 현재 door 가 속한 방의 Pos
    char m_doorPos;     // u : 위로, d : 아래로

    CCollider* m_doorCollider;

    CSprite* m_doorSprite;
    CSprite* m_doorCloseLSprite;
    CSprite* m_doorCloseRSprite;
    CSprite* m_doorOpenSprite;

    bool m_isOpen;      // 현재 door 가 열려있는가.

public:
    void SetCurRoomPos(int _pos) { m_curRoomPos = _pos; }
    void SetDoorPos(char _pos) { m_doorPos = _pos; }

    void SetIsOpen(bool _isOpen) { m_isOpen = _isOpen; }



public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;

public:
    CDoor();
    ~CDoor();
};

