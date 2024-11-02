#pragma once
#include "CObj.h"

class CFlipbookPlayer;

class CPlayerHead :
    public CObj
{
private:
    //CPlayer* 

public:
    virtual void Tick() override;
    virtual void Render() override;
};

