#include "pch.h"
#include "CAsset.h"

CAsset::CAsset(ASSET_TYPE _Type)
	: m_Type(_Type)
{
}

CAsset::~CAsset()
{
}

void CAsset::CheckExt(const wstring& _Ext, wstring& _strFilePath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	// 확장자가 올바르지 않다면
	if (wcscmp(szExt, _Ext.c_str()))
	{
		// 확장자가 세팅되어있지 않았다면
		if (!wcscmp(szExt, L""))
		{
			_strFilePath += _Ext;
		}

		// 확장자가 잘못 세팅되어있다면
		else
		{
			_strFilePath = _strFilePath.substr(0, _strFilePath.find(szExt));
			_strFilePath += _Ext;
		}
	}
}