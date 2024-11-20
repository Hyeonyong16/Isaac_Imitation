#pragma once
#include "CLevel.h"

class EndingObj;
class CSprite;

class CLevel_End :
    public CLevel
{
private:
    EndingObj* endObj;

    // 벽 표시할 sprite
    CSprite* m_wallSprite;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void End() override;

public:
    CLevel_End();
    ~CLevel_End();
};

