#pragma once
#include "CLevel.h"
class CLevel_Start :
    public CLevel
{
public:
    virtual void Begin() override;
    virtual void Render() override;
    virtual void End() override;

public:
    CLevel_Start();
    ~CLevel_Start();
};

