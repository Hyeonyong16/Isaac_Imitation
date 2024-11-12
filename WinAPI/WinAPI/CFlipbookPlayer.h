#pragma once
#include "CComponent.h"

class CFlipbook;
class CTexture;

struct tHeartEffect
{
    float			Time;		// 효과 진행시간 계산
    float			Duration;	// 효과 최대 진행 시간
};

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

    bool        m_Inversion;    // 플립북 렌더링 시 좌우 반전

    Vec2        m_renderSize;
    Vec2        m_renderOffset;

    tHeartEffect m_heartEffect;
    bool         m_isHitted;

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
        m_Inversion = false;
    }

    void Play(int _FlipbookIdx, float _FPS, bool _Repeat, bool _Inversion)
    {
        m_CurFlipbook = m_vecFlipbook[_FlipbookIdx];
        m_SpriteIdx = 0;
        m_FPS = _FPS;
        m_Repeat = _Repeat;
        m_Finish = false;
        m_Time = 0.f;
        m_Inversion = _Inversion;
    }

    bool isFinish() { return m_Finish; }

    void Reset()
    {
        m_Finish = false;
        m_SpriteIdx = 0;
    }

    int GetPlayFlipbookIdx()
    {
        vector<CFlipbook*>::iterator iter = m_vecFlipbook.begin();
        int i = 0;

        for (; iter != m_vecFlipbook.end(); ++iter)
        {
            if (*iter == m_CurFlipbook)
                return i;
            ++i;
        }

        return -1;
    }

    void SetRenderSize(Vec2 _renderSize) { m_renderSize = _renderSize; }
    void SetRenderOffset(Vec2 _renderOffset) { m_renderOffset = _renderOffset; }

    int GetCurSpriteIdx() { return m_SpriteIdx; }
    CFlipbook* GetCurFlipbook() { return m_CurFlipbook; }

    void SetIsHitted(bool _isHitted) { m_heartEffect.Time = 0.f; m_isHitted = _isHitted; }

public:
    virtual void FinalTick() override;
    void Render();

public:
    CFlipbookPlayer();
    ~CFlipbookPlayer();
};

