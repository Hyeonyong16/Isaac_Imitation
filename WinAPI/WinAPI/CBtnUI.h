#pragma once
#include "CUI.h"
class CBtnUI :
    public CUI
{
private:
    PARAM_0             m_Func;

    // Delegate
    CBase* m_Inst;
    DELEGATE_0          m_MemFunc;

public:
    void AddCallBack(PARAM_0 _Func) { m_Func = _Func; }
    void AddDelegate(CBase* _Inst, DELEGATE_0 _memFunc) { m_Inst = _Inst; m_MemFunc = _memFunc; }


public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    // Mouse Click
    virtual void MouseLBtnClikced() override;

public:
    CBtnUI();
    ~CBtnUI();    
};

