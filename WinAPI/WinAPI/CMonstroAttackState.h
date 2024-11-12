#pragma once
#include "CState.h"
class CMonstroAttackState :
    public CState
{
private:
    CObj* m_TargetObject;

    bool  m_isPlay;
    bool  m_isAttack;
    float m_curTime;


public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroAttackState();
    ~CMonstroAttackState();
};

