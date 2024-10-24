#include "pch.h"

#include "WinAPI.h"
#include "CEngine.h"

#define MAX_LOADSTRING 100

// 전역 변수
HINSTANCE   g_hInst = nullptr;                      // 현재 인스턴스입니다.
HWND        g_hDlg = nullptr;


WCHAR       szTitle[MAX_LOADSTRING];                // 제목 표시줄 텍스트입니다.
WCHAR       szWindowClass[MAX_LOADSTRING];          // 기본 창 클래스 이름입니다.

// 함수 선언
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// SAL : 주석언어 (크게 중요하지 않음) - _In_, _In_opt_ 같은거
// 각 인자가 어떤 역할인지 알려주는 용도, 안붙여도 영향없음, 요즘 잘 안씀
int APIENTRY wWinMain(_In_ HINSTANCE    hInstance,      // 프로세스 주소(ID)
                     _In_opt_ HINSTANCE hPrevInstance,  // 입력은 들어오나 쓰이지 않는 인자
                     _In_ LPWSTR        lpCmdLine,      //
                     _In_ int           nCmdShow)
{    
    // 메모리 누수 확인
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(18);

    g_hInst = hInstance;    // 프로세스 시작 주소

    // 프로그램 실행시 나오는 윈도우의 기본 설정
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = &WndProc;         // 해당 윈도우에 문제가 생겼을 시 처리할 처리기 함수. 각 윈도우에는 각각의 처리기 함수가 있다(처리기 함수는 같을 수도 다를 수도 있음.)
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = nullptr;//MAKEINTRESOURCEW(IDC_WINAPI);
    wcex.lpszClassName  = L"Key";    // 윈도우 초기화 시 설정값을 넘겨주는 일종의 키값
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex); // 위에서 설정한 세팅을 윈도우에 등록

   // Engine 초기화
    if (FAILED(CEngine::GetInst()->Init(g_hInst, POINT{ 1920, 1080 })))
        return FALSE;


    // 단축키
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI));

    // 메세지 변수
    MSG msg = {};

    /*
    // GetMessage
    // - 메세지 큐에 있는 메세지를 받아온다.
    // - 메세지가 큐에 없으면 함수가 반환되지 않는다.
    // - 꺼낸 메세지가 WM_QUIT 이면 false 를 반환, 그 외에는 true 를 반환
    
    //SetTimer(hWnd, 0, 50, 0);       // 비효율적임

    // 메세지가 없으면, 프로그램이 동작하지 않는 구조이다.
    // 게임을 만들기에 적합하지 않음.
    // 강제로 일정시간마다 일정한 메세지를 발생시킨다. 
    // - Timer 를 이용해서 WM_TIMER 메세지를 일정시간마다 발생시킨다.
    //      - 해당 구조는 매우 느리고 비효율적임
    // - 메세지 큐에 메세지가 없어도 프로그램이 중단되어있지 않는 구조가 필요.
    //      - GetMessage -> PeekMessage

    // GetMessage 가 반환되었다. == 메세지가 있었다.
    // 반환값이 false == 메세지가 WM_QUIT 이다.
    // 반환값이 true == 메세지가 WM_QUIT 아니다.

    // PeekMessage 가 반환되었다. == 메세지가 있었을 수도 있고, 없었을 수도 있다.
    // 반환값이 true == 메세지가 있었다.
    // 반환값이 false == 메세지가 없었다.
    */

    while (true)
    {
        // 메세지 큐에 메세지가 있다.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))    // 메세지 확인 후 보기만 할거면 PM_NOREMOVE, 확인 후 메세지 처리할거면 PM_REMOVE
        {
            if (WM_QUIT == msg.message)
                break;

            // 메세지 처리
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        // 메세지큐에 메세지가 없다.
        else
        {
            // 게임 실행
            CEngine::GetInst()->Progress();
        }        
    }

    //KillTimer(hWnd, 0);
    return (int) msg.wParam;
}

// 1. 전역변수
// 2. 동적할당

bool TileMapEditorMenu(HINSTANCE _inst, HWND _wnd, int wParam);
bool SpriteEditorMenu(HINSTANCE _inst, HWND _wnd, int wParam);

// 메세지를 처리하는 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            if (TileMapEditorMenu(g_hInst, hWnd, wmId))
            {
                break;
            }

            if (SpriteEditorMenu(g_hInst, hWnd, wmId))
            {
                break;
            }

            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, &About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:  // 윈도우에서 화면을 그려야 할 것 같다고 판단될 때 ( 맨처음 윈도우 생성, 창 최소화 등 )
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // 무효화 영역 ( 화면을 다시 갱신해야 하는 영역 )

            // 사각형 그리기
           /* Rectangle(hdc
                    , g_X - g_Width / 2.f
                    , g_Y - g_Height / 2.f
                    , g_X + g_Width / 2.f
                    , g_Y + g_Height / 2.f);*/

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
