#pragma once
#include "CState.h"
class CMonstroChaseState :
    public CState
{
private:
    CObj*   m_TargetObject;

    bool    m_isPlay;
    Vec2    tempPlayerPos;
    int     m_jumpCount;

    Vec2    m_RenderOffset; // 점프하는것 처럼 보이기 위한 오프셋

    float   m_curTime;    // 일정 시간 후 패턴 진입을 위한 시간


public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroChaseState();
    ~CMonstroChaseState();
};

