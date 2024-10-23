#pragma once
#include "CComponent.h"

class CFlipbook;

class CFlipbookPlayer :
    public CComponent
{
private:
    vector<CFlipbook*>  m_vecFlipbook;
    CFlipbook*          m_CurFlipbook;
    int                 m_SpriteIdx;

    float       m_FPS;
    float       m_Time;
    bool        m_Repeat;
    bool        m_Finish;

public:
    void AddFlipbook(CFlipbook* _Flipbook) { m_vecFlipbook.push_back(_Flipbook); }
    void AddFlipbook(int _Idx, CFlipbook* _Flipbook)
    {
        if (m_vecFlipbook.size() <= _Idx)
        {
            m_vecFlipbook.resize(_Idx + 1);
        }

        m_vecFlipbook[_Idx] = _Flipbook;
    }

    void Play(int _FlipbookIdx, float _FPS, bool _Repeat)
    {
        m_CurFlipbook = m_vecFlipbook[_FlipbookIdx];
        m_SpriteIdx = 0;
        m_FPS = _FPS;
        m_Repeat = _Repeat;
        m_Finish = false;
        m_Time = 0.f;
    }

    bool isFinish() { return m_Finish; }

    void Reset()
    {
        m_Finish = false;
        m_SpriteIdx = 0;
    }

public:
    virtual void FinalTick() override;
    void Render();

public:
    CFlipbookPlayer();
    ~CFlipbookPlayer();
};

