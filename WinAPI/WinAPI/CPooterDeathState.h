#pragma once
#include "CState.h"
class CPooterDeathState :
    public CState
{
private:
    float m_curTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CPooterDeathState();
    ~CPooterDeathState();
};

