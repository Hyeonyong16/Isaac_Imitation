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


// ���� ������� ����
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

    // Pen �� Brush ����
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

    // HWND ������ ID Ÿ��
    // Ŀ�� ������Ʈ ( OS �������� �����Ǵ� ��ü )
    m_hWnd = CreateWindowW(L"Key", L"MyGame", WS_OVERLAPPEDWINDOW,   // ���� wcex�� lpszClassName �� �����ؾ���, ������ Ÿ��Ʋ �̸�, ...
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);

    if (!m_hWnd)
        return E_FAIL;

    ShowWindow(m_hWnd, true);
    UpdateWindow(m_hWnd);

    // ������ ũ�⸦ �ػ󵵿� �°� ����
    ChangeWindowSize(m_Resolution);

    // DC ����
    m_hDC = GetDC(m_hWnd);

    // GDIObject ����
    CreateGDIObject();

    // Manager ���� �� �ʱ�ȭ
    CPathMgr::Init();
    CSoundMgr::GetInst()->Init();
    CTimeMgr::GetInst()->Init();
    CKeyMgr::GetInst()->Init();
    CAssetMgr::GetInst()->Init();
    CLevelMgr::GetInst()->Init();
    CCamera::GetInst()->Init();


    // ���� ���۸��� ���� �߰� ���� ����
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

    // ���� ����
    CLevelMgr::GetInst()->Progress();

    // �浹 �˻� ����
    CCollisionMgr::GetInst()->Tick();

    // UI �Ŵ��� üũ
    CUIMgr::GetInst()->Tick();

    // ������
    // ȭ�� Clear
    {
        SELECT_BRUSH(BRUSH_TYPE::GRAY);
        Rectangle(m_BackBuffer->GetDC(), -1, -1, (int)m_Resolution.x + 1, (int)m_Resolution.y + 1);
    }

    // ���� ������
    CLevelMgr::GetInst()->Render();

    // Camera ������
    CCamera::GetInst()->Render();

    // ����� ���� ������
    CDbgRender::GetInst()->Render();

    // SecondBitmap �� ����� MainWindowBitmap ���� ����
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

    // ���� �����쿡 Menu�� �ִ��� Ȯ��
    HMENU hMenu = GetMenu(m_hWnd);

    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!hMenu);

    SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}