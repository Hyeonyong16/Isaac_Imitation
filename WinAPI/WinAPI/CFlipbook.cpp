#include "pch.h"
#include "CFlipbook.h"

#include "CPathMgr.h"
#include "CSprite.h"

CFlipbook::CFlipbook()
	: CAsset(ASSET_TYPE::FLIPBOOK)
{
}

CFlipbook::~CFlipbook()
{
}

int CFlipbook::Save(const wstring& _RelativePath)
{
    wstring RelativePath = _RelativePath;
    CheckExt(L".flip", RelativePath);

    // 에셋이 자신이 어디에 저장되는지를 알게 함
    SetRelativePath(RelativePath);

    // 최종 저장 경로
    wstring strFilePath = CPathMgr::GetContentPath() + RelativePath;

    // Sprite 의 Key, RelativePath 를 저장
    FILE* File = nullptr;
    _wfopen_s(&File, strFilePath.c_str(), L"wb");

    size_t SpriteCount = m_Sprites.size();
    fwrite(&SpriteCount, sizeof(size_t), 1, File);

    for (size_t i = 0; i < m_Sprites.size(); ++i)
    {
        SaveAssetRef(m_Sprites[i], File);
    }

    fclose(File);

    return 0;
}

int CFlipbook::Load(const wstring& _RelativePath)
{
    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _RelativePath;

    // Sprite 의 Key, RelativePath 를 저장
    FILE* File = nullptr;
    _wfopen_s(&File, strFilePath.c_str(), L"rb");

    size_t SpriteCount = 0;
    fread(&SpriteCount, sizeof(size_t), 1, File);

    for (size_t i = 0; i < SpriteCount; ++i)
    {
        CSprite* pSprite = (CSprite*)LoadAssetRef(File);
        m_Sprites.push_back(pSprite);
    }

    fclose(File);

    return S_OK;
}