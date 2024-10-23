#pragma once

class CTexture;
class CSprite;
class CFlipbook;
class CSound;

class CAssetMgr
{
	SINGLE(CAssetMgr);
private:
	map<wstring, CTexture*> m_mapTex;
	map<wstring, CSprite*> m_mapSprite;
	map<wstring, CFlipbook*> m_mapFlipbook;
	map<wstring, CSound*>		m_mapSound;

public:
	void Init();

	CTexture* FindTexture(const wstring& _Key);
	CTexture* LoadTexture(const wstring& _Key, const wstring& _RelativePath);
	CTexture* CreateTexture(const wstring& _Key, UINT _Width, UINT _Height);

	CSprite* FindSprite(const wstring& _Key);
	CSprite* LoadSprite(const wstring& _Key, const wstring& _RelativePath);
	void AddSprite(const wstring& _Key, CSprite* _Sprite);

	CFlipbook* FindFlipbook(const wstring& _Key);
	CFlipbook* LoadFlipbook(const wstring& _Key, const wstring& _RelativePath);
	void AddFlipbook (const wstring& _Key, CFlipbook* _Flipbook);

	CSound* FindSound(const wstring& _Key);
	CSound* LoadSound(const wstring& _Key, const wstring& _RelativePath);
};

