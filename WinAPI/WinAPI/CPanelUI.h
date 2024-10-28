#pragma once
#include "CUI.h"

class CSprite;

class CPanelUI :
    public CUI
{
private:
    Vec2    m_DownPos; //자신이 마우스 왼쪽 클릭 되었을 때 좌표 기록

    // 확인용 변수
    Vec2 m_vMousePosTemp;
    Vec2 m_vDiffTemp;
    Vec2 m_vPosTemp;

    CSprite* m_sprite;

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    virtual void MouseLBtnDown() override;
    virtual void MouseLBtnClikced() override;

    void SetSprite(CSprite* _sprite) { m_sprite = _sprite; }

public:
    CPanelUI();
    ~CPanelUI();

};

