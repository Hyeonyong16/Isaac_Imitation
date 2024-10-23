#include "pch.h"
#include "CTexture.h"

#include "CEngine.h"
#include "CPathMgr.h"

CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE)
	, m_hBit(nullptr)
	, m_DC(nullptr)
	, m_BitmapInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_DC);
	DeleteObject(m_hBit);


	if (nullptr != m_Image)
		delete m_Image;
}

int CTexture::Load(const wstring& _FilePath)
{
	wstring strContent = CPathMgr::GetContentPath();
	strContent += _FilePath;
	
	wchar_t ext[50] = {};
	_wsplitpath_s(strContent.c_str(), 0, 0, 0, 0, 0, 0, ext, 50);

	if (!wcscmp(ext, L".bmp") || !wcscmp(ext, L".BMP"))
	{
		m_hBit = (HBITMAP)LoadImage(nullptr
			, strContent.c_str()
			, IMAGE_BITMAP, 0, 0
			, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	}

	else if (!wcscmp(ext, L".png") || !wcscmp(ext, L".PNG"))
	{
		// png 로딩
		ULONG_PTR gdiplusToken = 0;
		GdiplusStartupInput gidstartupinput = {};

		GdiplusStartup(&gdiplusToken, &gidstartupinput, nullptr);
		m_Image = Image::FromFile(strContent.c_str(), false);
		Bitmap* pBit = (Bitmap*)m_Image->Clone();

		Status stat = pBit->GetHBITMAP(Color(0, 0, 0, 0), &m_hBit);
		if (stat != Status::Ok)
			assert(nullptr);
	}

	// 비트맵을 타겟으로 지정할 DC 생성
	m_DC = CreateCompatibleDC(CEngine::GetInst()->GetMainDC());

	// 비트맵과 DC 연결, SelectObject 하면서 나오는 m_DC 기존 비트맵은 제거
	DeleteObject(SelectObject(m_DC, m_hBit));

	// m_hBit 의 비트맵 정보를 받아온다.
	GetObject(m_hBit, sizeof(BITMAP), &m_BitmapInfo);

	return S_OK;
}

void CTexture::Create(UINT _Width, UINT _Height)
{
	// 윈도우 비트맵과 동일한 해상도의 비트맵 생성
	m_hBit = CreateCompatibleBitmap(CEngine::GetInst()->GetMainDC(), _Width, _Height);

	// 더블 버퍼링용 DC 생성
	m_DC = CreateCompatibleDC(CEngine::GetInst()->GetMainDC());

	// 새로 만든 DC로 Bitmap을 렌더 타겟 지정 > 기존 DC(새로만든)비트맵은 사용 X(더블버퍼링) > 삭제
	DeleteObject(SelectObject(m_DC, m_hBit));

	// 비트맵의 정보를 받아옴
	GetObject(m_hBit, sizeof(BITMAP), &m_BitmapInfo);
}


