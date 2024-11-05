#pragma once
#include "CState.h"

class CPooterIdleState :
    public CState
{
private:
    CObj* m_TargetObject;

    bool  m_IsTurnRight;    // �ð���� ȸ�� ����
    float m_curTime;    // �ð� ����
    float m_turnTime;   // �ش� �ð� �� ȸ��

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CPooterIdleState();
    ~CPooterIdleState();

};

