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

    Vec2    m_RenderOffset; // �����ϴ°� ó�� ���̱� ���� ������

    float   m_curTime;    // ���� �ð� �� ���� ������ ���� �ð�


public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonstroChaseState();
    ~CMonstroChaseState();
};

