#pragma once
#include "CComponent.h"


class CState;

// 유한 상태 기계
// Finite State Machine
class CFSM :
    public CComponent
{
private:
    map<wstring, CState*>   m_mapState;
    CState* m_CurState;
    CState* m_PrevState;

public:
    void AddState(const wstring& _Key, CState* _State);
    CState* FindState(const wstring& _Key);
    void ChangeState(const wstring& _NextState);


public:
    virtual void FinalTick() override;

public:
    CFSM();
    ~CFSM();
};
