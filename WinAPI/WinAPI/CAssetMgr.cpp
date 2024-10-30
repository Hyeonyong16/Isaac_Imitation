#include "pch.h"
#include "CAssetMgr.h"

#include "CTexture.h"
#include "CSprite.h"
#include "CFlipbook.h"
#include "CSound.h"

CAssetMgr::CAssetMgr()
{}

CAssetMgr::~CAssetMgr()
{
    Delete_Map(m_mapTex);
    Delete_Map(m_mapSprite);
    Delete_Map(m_mapFlipbook);
    Delete_Map(m_mapSound);
}

void CAssetMgr::Init()
{
    
}

CTexture* CAssetMgr::FindTexture(const wstring& _Key)
{
    map<wstring, CTexture*>::iterator iter = m_mapTex.find(_Key);

    if (iter == m_mapTex.end())
    {
        return nullptr;
    }

    return iter->second;
}

CTexture* CAssetMgr::LoadTexture(const wstring& _Key, const wstring& _RelativePath)
{
    CAsset* pTex = FindTexture(_Key);

    // 이미 로딩한 적이 있는 Texture 일 경우
    if (nullptr != pTex)
    {
        // 앞에 로딩했던 텍스쳐 반환
        return (CTexture*)pTex;
    }

    pTex = new CTexture;
    pTex->Load(_RelativePath);

    // 에셋매니저에 등록될 때 사용된 키값과 경로를 세팅
    pTex->SetKey(_Key);
    pTex->SetRelativePath(_RelativePath);

    // 컨테이너 (map) 에 텍스쳐 등록
    m_mapTex.insert(make_pair(_Key, (CTexture*)pTex));

    return (CTexture*)pTex;
}

CTexture* CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height)
{
    CTexture* pTex = FindTexture(_Key);
    assert(!pTex);

    pTex = new CTexture;
    pTex->Create(_Width, _Height);

    // 에셋에, 자신이 에셋매니저에 등록될때 사용된 키값을 세팅해준다.
    pTex->SetKey(_Key);

    // 컨테이너에 텍스쳐 등록
    m_mapTex.insert(make_pair(_Key, (CTexture*)pTex));

    return pTex;
}

CSprite* CAssetMgr::FindSprite(const wstring& _Key)
{
    map<wstring, CSprite*>::iterator iter = m_mapSprite.find(_Key);

    if (iter == m_mapSprite.end())
    {
        return nullptr;
    }

    return iter->second;
}

CSprite* CAssetMgr::LoadSprite(const wstring& _Key, const wstring& _RelativePath)
{
    CAsset* pSprite = FindSprite(_Key);

    // 이미 로딩한 적이 있는 Texture 일 경우
    if (nullptr != pSprite)
    {
        // 앞에 로딩했던 텍스쳐 반환
        return (CSprite*)pSprite;
    }

    pSprite = new CSprite;
    pSprite->Load(_RelativePath);

    // 에셋매니저에 등록될 때 사용된 키값과 경로를 세팅
    pSprite->SetKey(_Key);
    pSprite->SetRelativePath(_RelativePath);

    // 컨테이너 (map) 에 텍스쳐 등록
    m_mapSprite.insert(make_pair(_Key, (CSprite*)pSprite));

    return (CSprite*)pSprite;
}

void CAssetMgr::AddSprite(const wstring& _Key, CSprite* _Sprite)
{
    assert(!FindSprite(_Key));

    _Sprite->SetKey(_Key);
    m_mapSprite.insert(make_pair(_Key, _Sprite));
}

CFlipbook* CAssetMgr::FindFlipbook(const wstring& _Key)
{
    map<wstring, CFlipbook*>::iterator iter = m_mapFlipbook.find(_Key);

    if (iter == m_mapFlipbook.end())
    {
        return nullptr;
    }

    return iter->second;
}

CFlipbook* CAssetMgr::LoadFlipbook(const wstring& _Key, const wstring& _RelativePath)
{
    CAsset* pFlipbook = FindFlipbook(_Key);

    // 이미 로딩한 적이 있는 Texture 라면
    if (nullptr != pFlipbook)
    {
        // 로딩한 텍스쳐를 반환
        return (CFlipbook*)pFlipbook;
    }

    pFlipbook = new CFlipbook;
    pFlipbook->Load(_RelativePath);

    // 에셋에, 자신이 에셋매니저에 등록될때 사용된 키값과 로딩할 때 사용한 경로를 세팅해준다.
    pFlipbook->SetKey(_Key);
    pFlipbook->SetRelativePath(_RelativePath);

    // 컨테이너에 텍스쳐 등록
    m_mapFlipbook.insert(make_pair(_Key, (CFlipbook*)pFlipbook));

    return (CFlipbook*)pFlipbook;
}

void CAssetMgr::AddFlipbook(const wstring& _Key, CFlipbook* _Flipbook)
{
    assert(!FindFlipbook(_Key));

    _Flipbook->SetKey(_Key);
    m_mapFlipbook.insert(make_pair(_Key, _Flipbook));
}

CSound* CAssetMgr::FindSound(const wstring& _Key)
{
    map<wstring, CSound*>::iterator iter = m_mapSound.find(_Key);

    if (iter == m_mapSound.end())
    {
        return nullptr;
    }

    return iter->second;
}

CSound* CAssetMgr::LoadSound(const wstring& _Key, const wstring& _RelativePath)
{
    CAsset* pSound = FindSound(_Key);

    // 이미 로딩한 적이 있는 Texture 라면
    if (nullptr != pSound)
    {
        // 로딩한 텍스쳐를 반환
        return (CSound*)pSound;
    }

    pSound = new CSound;
    pSound->Load(_RelativePath);

    // 에셋에, 자신이 에셋매니저에 등록될때 사용된 키값과 로딩할 때 사용한 경로를 세팅해준다.
    pSound->SetKey(_Key);
    pSound->SetRelativePath(_RelativePath);

    // 컨테이너에 텍스쳐 등록
    m_mapSound.insert(make_pair(_Key, (CSound*)pSound));

    return (CSound*)pSound;
}