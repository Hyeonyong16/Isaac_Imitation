#pragma once
#include "CState.h"

class CPooterIdleState :
    public CState
{
private:
    CObj* m_TargetObject;

    bool  m_IsTurnRight;    // 시계방향 회전 여부
    float m_curTime;    // 시간 누적
    float m_turnTime;   // 해당 시간 후 회전

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CPooterIdleState();
    ~CPooterIdleState();

};

