#pragma once
#include "CUI.h"

class CPanelUI :
    public CUI
{
private:
    Vec2    m_DownPos; //자신이 마우스 왼쪽 클릭 되었을 때 좌표 기록

    // 확인용 변수
    Vec2 m_vMousePosTemp;
    Vec2 m_vDiffTemp;
    Vec2 m_vPosTemp;

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    virtual void MouseLBtnDown() override;
    virtual void MouseLBtnClikced() override;


public:
    CPanelUI();
    ~CPanelUI();

};

