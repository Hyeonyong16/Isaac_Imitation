#pragma once
#include "CState.h"
class CChargerAttackState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CChargerAttackState();
    ~CChargerAttackState();
};

