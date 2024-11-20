#pragma once
#include "CObj.h"

class CFlipbookPlayer;


enum ENDING_ANIM_STATE
{
    ENDING_POLAROID,
    THE_END,
};


class EndingObj :
    public CObj
{
private:
    CFlipbookPlayer* m_flipbookPlayer;

    float m_curTime;

    bool m_isFlipbookEnd;
    bool m_isFadeIn;
    bool m_sceneChange;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

public:
    bool GetSceneChange() { return m_sceneChange; }


public:
    EndingObj();
    ~EndingObj();
};

