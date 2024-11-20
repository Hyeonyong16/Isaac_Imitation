#pragma once
#include "CState.h"
class CMonstroDeathState :
    public CState
{
private:
    float m_curTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroDeathState();
    ~CMonstroDeathState();
};

