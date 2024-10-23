#pragma once
#include "CAsset.h"

class CSprite;

class CFlipbook :
    public CAsset
{
private:
    vector<CSprite*> m_Sprites;

public:
    void AddSprite(CSprite* _Sprite) { m_Sprites.push_back(_Sprite); }
    CSprite* GetSprite(int _Idx) { return m_Sprites[_Idx]; }

    int GetMaxSpriteCount() { return m_Sprites.size(); }

public:
    virtual int Save(const wstring& _RelativePath) override;
    virtual int Load(const wstring& _RelativePath) override;

public:
    CFlipbook();
    ~CFlipbook();
};

