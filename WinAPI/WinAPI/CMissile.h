#pragma once
#include "CObj.h"

class CFlipbookPlayer;

enum BULLET_ANIM_STATE
{
    BULLET_IDLE,
    BULLET_DESTROY,
};

class CMissile :
    public CObj
{
private:
    Vec2        m_Velocity;
    float       m_VelocityScale;

    CCollider*  m_Collider;
    float       m_Mass;     // 질량

    Vec2        m_Force;    // 크기, 방향

    float       m_curTime;      // 현재 생존 시간
    float       m_existTime;    // 최대 생존 시간

    bool        m_isDamaged;    // 데미지를 입혔는가?
    bool        m_isDeleted;    // 삭제되어야하는가?
    bool        m_isDeletePlay; // 삭제애니메이션 재생여부

    CFlipbookPlayer* m_BulletFlipbook;

public:
    void SetVelocity(Vec2 _Velocity) { m_Velocity = _Velocity; }
    Vec2 GetVelocity() { return m_Velocity; }

    void SetVelocityScale(float _Scale) { m_VelocityScale = _Scale; }
    float GetVelocityScale() { return m_VelocityScale; }

    CCollider* GetCollider() { return m_Collider; }

    void SetMass(float _Mass) { m_Mass = _Mass; }
    float GetMass() { return m_Mass; }

    void AddForce(Vec2 _Force) { m_Force += _Force; }
    Vec2 GetForce() { return m_Force; }


public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);

public:
    CMissile();
    ~CMissile();
};

