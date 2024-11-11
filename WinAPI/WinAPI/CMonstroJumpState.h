#pragma once
#include "CState.h"
class CMonstroJumpState :
    public CState
{
private:
    CObj*   m_TargetObject;
    bool    m_isPlay;
    Vec2    tempPlayerPos;

    float m_curTime;    // ���� �ð� �� ���� ������ ���� �ð�

    bool    m_isAttack;   // ���� �� �̻��� �߻� ����

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroJumpState();
    ~CMonstroJumpState();
};

