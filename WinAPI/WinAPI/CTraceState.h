#pragma once
#include "CState.h"
class CTraceState :
    public CState
{
private:
    CObj* m_TargetObject;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CTraceState();
    ~CTraceState();
};
