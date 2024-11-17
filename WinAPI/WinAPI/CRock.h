#pragma once
#include "CMonster.h"

class CCollider;
class CSprite;

class CRock :
    public CMonster
{
private:
    class CCollider* m_Collider;
    CSprite* m_Sprite;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;

public:
    void SetColliderScale(Vec2 _scale);

public:
    CRock();
    ~CRock();
};

