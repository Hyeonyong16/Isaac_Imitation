#pragma once
#include "CAsset.h"

class CTexture;

class CSprite :
    public CAsset
{
private:
    CTexture* m_Atlas;  // 스프라이트가 타겟으로 지정한 아틀라스 텍스쳐
    Vec2 m_LeftTop;     // 좌상단 픽셀 위치
    Vec2 m_Slice;       // 조각 크기
    Vec2 m_Offset;

public:
    CTexture* GetAtlas() { return m_Atlas; }
    Vec2 GetLeftTop() { return m_LeftTop; }
    Vec2 GetSlice() { return m_Slice; }
    Vec2 GetOffset() { return m_Offset; }

    void SetOffset(Vec2 _Offset) { m_Offset = _Offset; }

public:
    virtual int Save(const wstring& _RelativePath) override;
    virtual int Load(const wstring& _RelativePath) override;

    void Create(CTexture* _Atlas, Vec2 _LeftTop, Vec2 _Slice);

public:
    CSprite();
    ~CSprite();

};

