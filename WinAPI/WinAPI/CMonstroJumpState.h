#pragma once
#include "CState.h"
class CMonstroJumpState :
    public CState
{
private:
    CObj*   m_TargetObject;
    bool    m_isPlay;
    Vec2    tempPlayerPos;

    float m_curTime;    // 일정 시간 후 패턴 진입을 위한 시간

    bool    m_isAttack;   // 착지 후 미사일 발사 여부

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroJumpState();
    ~CMonstroJumpState();
};

