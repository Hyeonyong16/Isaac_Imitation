#pragma once
#include "CUI.h"

class CSprite;
class CPlayer;

class CHPUI :
    public CUI
{
private:
    int m_maxHP;
    int m_curHP;

    CPlayer* m_TargetPlayer;

    CSprite* m_FullHPSprite;
    CSprite* m_HalfHPSprite;
    CSprite* m_EmptyHPSprite;

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

public:
    CHPUI();
    ~CHPUI();

};
