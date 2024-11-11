#pragma once
#include "CState.h"

class CMonstroIdleState :
    public CState
{
private:
    CObj* m_TargetObject;

    float m_curTime;    // ���� �ð� �� ���� ������ ���� �ð�

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroIdleState();
    ~CMonstroIdleState();
};

