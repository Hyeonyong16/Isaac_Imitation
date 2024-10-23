#pragma once
#include "CBase.h"

#include "CFSM.h"

class CState :
    public CBase
{
private:
    class CFSM* m_Owner;

public:
    CFSM* GetFSM() { return m_Owner; }
    CObj* GetOwnerObj() { return m_Owner->GetOwner(); }


public:
    virtual void Enter() = 0;
    virtual void FinalTick() = 0;
    virtual void Exit() = 0;

public:
    CState();
    ~CState();

    friend class CFSM;
};
