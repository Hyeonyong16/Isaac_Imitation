#pragma once
#include "CObj.h"

typedef void (*PARAM_0)(void);
typedef void (*PARAM_1)(DWORD_PTR);


typedef void(CBase::* DELEGATE_0)(void);
typedef void(CBase::* DELEGATE_1)(DWORD_PTR);
typedef void(CBase::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class CUI :
    public CObj
{
private:
    CUI*            m_ParentUI;    // 자신을 소유한 부모UI 를 가리키는 포인터, nullptr 인 경우 본인이 최상위 부모 UI 라는 뜻
    vector<CUI*>    m_vecChildUI;  // 소유하고 있는 자식 UI 들 포인터

    Vec2            m_FinalPos;    // 최종 계산된 위치

    bool            m_MouseHover;  // UI 위에 마우스가 올라왔는지 체크
    bool            m_LBtnDown;    // UI 위에서 마우스 왼쪽 버튼이 눌렸는지 체크


public:
    bool IsMouseHover() { return m_MouseHover; }
    bool IsLBtnDown() { return m_LBtnDown; }

    Vec2 GetFinalPos() { return m_FinalPos; }

    void AddChildUI(CUI* _ChildUI)
    {
        m_vecChildUI.push_back(_ChildUI);
        _ChildUI->m_ParentUI = this;
    }


public:
    virtual void Tick() override;
    virtual void Tick_UI() = 0;

    virtual void Render() override;
    virtual void Render_UI();

protected:
    virtual void MouseHoverCheck();

    // 마우스 왼쪽 버튼이 눌렸을 때
    virtual void MouseLBtnDown() {}

    // 해당 UI 가 눌렸고, 다시 UI 위에서 버튼이 떼졌을 때 클릭으로 판정한다.
    virtual void MouseLBtnClikced() {}

    // 해당 UI 가 눌리진 않았지만, UI 위에서 버튼이 떼졌을 때, Drag Drop 될 때 활용
    virtual void MouseReleased() {}

public:
    CUI();
    ~CUI();

    friend class CUIMgr;

};

