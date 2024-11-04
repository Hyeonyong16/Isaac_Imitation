#pragma once
#include "CObj.h"

class CCollider;

class CPlatform :
    public CObj
{
private:
    class CCollider* m_Collider;

public:
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);

    void SetColliderScale(Vec2 _scale);


public:
    CPlatform();
    ~CPlatform();
};

