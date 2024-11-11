#pragma once
#include "CState.h"

class CMonstroIdleState :
    public CState
{
private:
    CObj* m_TargetObject;

    float m_curTime;    // 일정 시간 후 패턴 진입을 위한 시간

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroIdleState();
    ~CMonstroIdleState();
};

