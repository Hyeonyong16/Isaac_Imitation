#include "pch.h"
#include "CEngine.h"

#include "CAssetMgr.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CDbgRender.h"
#include "CCollisionMgr.h"

#include "CSelectGDI.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"


// 정적 멤버변수 구현
//CEngine* CEngine::m_Inst = nullptr;

CEngine::CEngine()
    : m_hInst(nullptr)
    , m_hWnd(nullptr)
    , m_Resolution{}
    , m_hDC(nullptr)
    , m_BackBuffer(nullptr)
    , m_Pen{}
    , m_Brush{}
{
}


CEngine::~CEngine()
{
    ReleaseDC(m_hWnd, m_hDC);

    // Pen 과 Brush 해제
    for (UINT i = 0; i < (UINT)PEN_TYPE::END; ++i)
    {
        DeleteObject(m_Pen[i]);
    }

    for (UINT i = 0; i < (UINT)BRUSH_TYPE::END; ++i)
    {
        if (BRUSH_TYPE::HOLLOW == (BRUSH_TYPE)i)
            continue;

        DeleteObject(m_Brush[i]);
    }
}


int CEngine::Init(HINSTANCE _hInst, POINT _Resolution)
{
	m_hInst = _hInst;
	m_Resolution = _Resolution;

    // HWND 윈도우 ID 타입
    // 커널 오브젝트 ( OS 차원에서 관리되는 객체 )
    m_hWnd = CreateWindowW(L"Key", L"MyGame", WS_OVERLAPPEDWINDOW,   // 위에 wcex의 lpszClassName 과 동일해야함, 윈도우 타이틀 이름, ...
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);

    if (!m_hWnd)
        return E_FAIL;

    ShowWindow(m_hWnd, true);
    UpdateWindow(m_hWnd);

    // 윈도우 크기를 해상도에 맞게 설정
    ChangeWindowSize(m_Resolution);

    // DC 생성
    m_hDC = GetDC(m_hWnd);

    // GDIObject 생성
    CreateGDIObject();

    // Manager 생성 및 초기화
    CPathMgr::Init();
    CSoundMgr::GetInst()->Init();
    CTimeMgr::GetInst()->Init();
    CKeyMgr::GetInst()->Init();
    CAssetMgr::GetInst()->Init();
    CLevelMgr::GetInst()->Init();
    CCamera::GetInst()->Init();


    // 더블 버퍼링을 위한 추가 버퍼 생성
    CreateSecondBuffer();

	return S_OK;
}


void CEngine::Progress()
{
    // Manager Tick
    CTimeMgr::GetInst()->Tick();
    CKeyMgr::GetInst()->Tick();
    CDbgRender::GetInst()->Tick();
    CCamera::GetInst()->Tick();

    // 레벨 실행
    CLevelMgr::GetInst()->Progress();

    // 충돌 검사 실행
    CCollisionMgr::GetInst()->Tick();

    // UI 매니저 체크
    CUIMgr::GetInst()->Tick();

    // 렌더링
    // 화면 Clear
    {
        SELECT_BRUSH(BRUSH_TYPE::GRAY);
        Rectangle(m_BackBuffer->GetDC(), -1, -1, (int)m_Resolution.x + 1, (int)m_Resolution.y + 1);
    }

    // 레벨 렌더링
    CLevelMgr::GetInst()->Render();

    // Camera 렌더링
    CCamera::GetInst()->Render();

    // 디버그 정보 렌더링
    CDbgRender::GetInst()->Render();

    // SecondBitmap 의 장면을 MainWindowBitmap 으로 복사
    BitBlt(m_hDC, 0, 0, (int)m_Resolution.x, (int)m_Resolution.y, m_BackBuffer->GetDC(), 0, 0, SRCCOPY);


    CTaskMgr::GetInst()->Tick();
}


void CEngine::CreateGDIObject()
{
    // Pen
    m_Pen[(UINT)PEN_TYPE::RED]      = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    m_Pen[(UINT)PEN_TYPE::GREEN]    = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    m_Pen[(UINT)PEN_TYPE::BLUE]     = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

    // Brush
    m_Brush[(UINT)BRUSH_TYPE::RED]      = CreateSolidBrush(RGB(255, 0, 0));
    m_Brush[(UINT)BRUSH_TYPE::GREEN]    = CreateSolidBrush(RGB(0, 255, 0));
    m_Brush[(UINT)BRUSH_TYPE::BLUE]     = CreateSolidBrush(RGB(0, 0, 255));
    m_Brush[(UINT)BRUSH_TYPE::GRAY]     = CreateSolidBrush(RGB(100, 100, 100));
    m_Brush[(UINT)BRUSH_TYPE::HOLLOW]   = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

void CEngine::CreateSecondBuffer()
{
    m_BackBuffer = CAssetMgr::GetInst()->CreateTexture(L"BackBuffer", (int)m_Resolution.x, (int)m_Resolution.y);
}


void CEngine::ChangeWindowSize(Vec2 _vResolution)
{
    m_Resolution = _vResolution;

    RECT rt = { 0, 0, m_Resolution.x, m_Resolution.y };

    // 메인 윈도우에 Menu가 있는지 확인
    HMENU hMenu = GetMenu(m_hWnd);

    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!hMenu);

    SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}