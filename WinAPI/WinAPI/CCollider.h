#pragma once
#include "CComponent.h"

class CCollider :
    public CComponent
{
private:
    Vec2 m_Scale;       // 크기

    Vec2 m_Offset;      // 상대 좌표
    Vec2 m_FinalPos;    // 충돌체의 실제 위치

    bool m_Active;      // 콜라이더 활성화 여부

    int m_OverlapCount;   // 현재 충돌중인 물체의 개수

public:
    void SetScale(Vec2 _Scale) { m_Scale = _Scale; }
    Vec2 GetScale() { return m_Scale; }

    void SetOffset(Vec2 _Offset) { m_Offset = _Offset; }
    Vec2 GetOffset() { return m_Offset; }

    Vec2 GetFinalPos() { return m_FinalPos; }

public:
    void BeginOverlap(CCollider* _Other);   // 다른 충돌체와 최초 충돌 시
    void Overlap(CCollider* _Other);        // 다른 충돌체와 겹쳐 있을 때 (BeginOverlap 이후)
    void EndOverlap(CCollider* _Other);     // 겹쳐있던 다른 충돌체와 최초로 떨어졌을 때


public:
    virtual void FinalTick() override;

public:
    CCollider();
    ~CCollider();
};

