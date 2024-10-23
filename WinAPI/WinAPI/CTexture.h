#pragma once
#include "CAsset.h"
#include <string>


class CTexture :
    public CAsset
{
private:
    HBITMAP     m_hBit;         // 비트맵 핸들
    HDC         m_DC;           // 비트맵을 목적지로 지정하는 DC
    BITMAP      m_BitmapInfo;   // 비트맵 정보

    Image* m_Image;

public:
    HDC GetDC() { return m_DC; }
    UINT GetWidth() { return m_BitmapInfo.bmWidth; }
    UINT GetHeight() { return m_BitmapInfo.bmHeight; }

private:
    virtual int Load(const wstring& _FilePath) override;
    void Create(UINT _Width, UINT _Height);

public:
    CTexture();
    ~CTexture();

    friend class CAssetMgr;
};

