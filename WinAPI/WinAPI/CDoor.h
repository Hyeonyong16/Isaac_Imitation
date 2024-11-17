#pragma once
#include "CObj.h"

class CSprite;
class CCollider;

class CDoor :
    public CObj
{
private:
    int m_curRoomPos;  // ���� door �� ���� ���� Pos
    char m_doorPos;     // u : ����, d : �Ʒ���

    CCollider* m_doorCollider;

    CSprite* m_doorSprite;
    CSprite* m_doorCloseLSprite;
    CSprite* m_doorCloseRSprite;
    CSprite* m_doorOpenSprite;

    bool m_isOpen;      // ���� door �� �����ִ°�.

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

