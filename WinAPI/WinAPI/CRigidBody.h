#pragma once
#include "CComponent.h"

enum class RIGIDBODY_MODE
{
    TOPVIEW,
    BELTSCROLL,
};

class CRigidBody :
    public CComponent
{
private:
    Vec2            m_Force;        // 방향, 힘의 크기
    bool            m_Self;         // 힘을 준 주체가 자기자신인지 확인

    Vec2            m_Velocity;     // 방향, 속력
    float           m_Mass;         // F = M x A
    float           m_InitialSpeed; // 초기 이동속력
    float           m_MaxSpeed;     // 최대 속력 제한
    float           m_Friction;     // 마찰력 크기
    Vec2            m_JumpVelocity; // 점프 속도   

    bool            m_bPrevMove;    // 이전에 이동중인지 아닌지
    bool            m_bMove;        // 이동중인지 아닌지

    Vec2            m_VelocityX;    // 좌,우 방향 속도
    Vec2            m_VelocityY;    // 위,아래 방향 속도
    float           m_MaxGravitySpeed;  // 중력에 의한 최대속도 제한
    Vec2            m_GravityAccel; // 중력 가속도
    bool            m_bGround;      // 땅위에 서있는지

    int             m_JumpStack;
    int             m_MaxJumpStack;

    RIGIDBODY_MODE  m_Mode;     // 동작 모드


public:
    Vec2 GetForce() { return m_Force; }
    void AddForce(Vec2 _Force, bool _Self = false) { m_Force += _Force; m_Self = _Self; }

    float GetMass() { return m_Mass; }
    void SetMass(float _Mass) { m_Mass = _Mass; }

    float GetMaxSpeed() { return m_MaxSpeed; }
    void SetMaxSpeed(float _Speed) { m_MaxSpeed = _Speed; }

    float GetFriction() { return m_Friction; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }

    float GetInitialSpeed() { return m_InitialSpeed; }
    void SetInitialSpeed(float _Init) { m_InitialSpeed = _Init; }

    void SetVelocity(Vec2 _Velocity) { m_Velocity = _Velocity; }
    void AddVelocity(Vec2 _Velocity) { m_Velocity += _Velocity; }

    void SetGravityAccel(Vec2 _GravityAccel) { m_GravityAccel = _GravityAccel; }
    Vec2 GetGravityAccel() { return m_GravityAccel; }

    bool IsGround() { return m_bGround; }
    void SetGround(bool _Ground) {
        m_bGround = _Ground;

        if (m_bGround)
        {
            m_JumpStack = m_MaxJumpStack;
        }
    }

    void SetJumpVelocity(Vec2 _Veloticy) { m_JumpVelocity = _Veloticy; }

    void Jump();

    RIGIDBODY_MODE GetMode() { return m_Mode; }
    void SetMode(RIGIDBODY_MODE _Mode) { m_Mode = _Mode; }


public:
    virtual void FinalTick() override;

private:
    void FinalTick_TopView();
    void FinalTick_BeltScroll();

    void CheckState();

    void CalcInitSpeed_TopView();
    void CalcInitSpeed_BeltScroll();

    void CalcFriction_TopView();
    void CalcFriction_BeltScroll();

    void CalcMaxSpeed_TopView();
    void CalcMaxSpeed_BeltScroll();

public:
    CRigidBody();
    ~CRigidBody();
};

