#include "pch.h"
#include "CSprite.h"

#include "CPathMgr.h"
#include "CTexture.h"

CSprite::CSprite()
	: CAsset(ASSET_TYPE::SPRITE)
	, m_Atlas(nullptr)
	, m_Slice{0.f, 0.f}
	, m_Offset{0.f, 0.f}
{
}

CSprite::~CSprite()
{
}

void CSprite::Create(CTexture* _Atlas, Vec2 _LeftTop, Vec2 _Slice)
{
	m_Atlas = _Atlas;
	m_LeftTop = _LeftTop;
	m_Slice = _Slice;
}

int CSprite::Save(const wstring& _RelativePath)
{
	wstring RelativePath = _RelativePath;
	CheckExt(L".sprite", RelativePath);

	// 에셋이 자신이 어디에 저장되는지를 알게 함
	SetRelativePath(RelativePath);

	wstring strFilePath = CPathMgr::GetContentPath() + RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"wb");

	fwrite(&m_LeftTop, sizeof(Vec2), 1, File);
	fwrite(&m_Slice, sizeof(Vec2), 1, File);
	fwrite(&m_Offset, sizeof(Vec2), 1, File);

	// 가리키던 Atlas 텍스쳐의 키, 경로를 저장한다.
	SaveAssetRef(m_Atlas, File);

	fclose(File);

	return S_OK;
}

int CSprite::Load(const wstring& _RelativePath)
{
	wstring strFilePath = CPathMgr::GetContentPath() + _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"rb");

	fread(&m_LeftTop, sizeof(Vec2), 1, File);
	fread(&m_Slice, sizeof(Vec2), 1, File);
	fread(&m_Offset, sizeof(Vec2), 1, File);

	// 가리키던 Atlas 텍스쳐의 키, 경로를 저장한다.
	m_Atlas = (CTexture*)LoadAssetRef(File);

	fclose(File);

	return S_OK;
}

