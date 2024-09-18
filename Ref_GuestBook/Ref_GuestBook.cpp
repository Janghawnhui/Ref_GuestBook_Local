﻿/// Ref_GuestBook.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "Ref_GuestBook.h"

#define MAX_LOADSTRING 100

/// 전역 변수:
HINSTANCE hInst;                                /// 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  /// 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            /// 기본 창 클래스 이름입니다.

/// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    /// TODO: 여기에 코드를 입력합니다.

    /// 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_REFGUESTBOOK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    /// 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_REFGUESTBOOK));

    MSG msg;

    /// 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

///  함수: MyRegisterClass()
///  용도: 창 클래스를 등록합니다.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_REFGUESTBOOK));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_REFGUESTBOOK);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

///   함수: InitInstance(HINSTANCE, int)
///   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
///   주석:
///        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
///        주 프로그램 창을 만든 다음 표시합니다.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

///  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
///  용도: 주 창의 메시지를 처리합니다.

///  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
///  WM_PAINT    - 주 창을 그립니다.
///  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.

/// 전역변수 정의
std::vector<PEN_INFO> penMemory;             /// 펜 구조체 정보 저장 벡터 변수 전역변수 정의
PEN_INFO g_Pen_Info;                    /// 펜 정보 구조체 전역변수 정의
COLORREF pen_Color = RGB(0, 0, 0);      /// 펜 기본 색상 BLACK, RGB(0, 0, 0)
ColorPalette g_colorPalette;

///  윈도우 핸들 전역변수
HWND g_Hwnd;                            /// HWND 전역변수 정의
PenDraw drawInstance;                   /// 그리기 관련 클래스 인스턴스 선언
            /// 펜 색상 관련 클래스 인스턴스 선언
PaintAreaSquare paintSquare;            /// 그리기 영역 사각형 인스턴스 선언
PenWidthControl penWidthControl;        /// 펜 굵기 조절 관련 인스턴스 선언
File_Manager fileManager;               /// File_Manager 클래스의 인스턴스 생성
Eraser eraser;                          /// Eraser 클래스의 인스턴스 생성

int pen_Width = 10;                     /// 펜 기본 굵기 10으로 정의
int stamp_Size = 100;                   /// 스탬프 크기 가로, 세로 80으로 정의
int stampIcon = 132;                    /// 스탬프 아이콘 초기값
bool stampActive = false;               /// 스탬프 버튼 활성화 확인
static Stamp* stampInfo = nullptr;      /// Stamp 객체를 저장할 포인터

/// 기능 기본 버튼
MakeButton bt_Clear(120, 10, 100, 100, ERASE, L"ERASE");
MakeButton bt_Replay(10, 10, 100, 100, REPLAY, L"REPLAY");
MakeButton bt_SAVE(230, 10, 100, 45, SAVE, L"SAVE");
MakeButton bt_Load(230, 65, 100, 45, LOAD, L"LOAD");
MakeButton bt_Widthup(375, 10, 30, 30, W_DOWN, L"-");
MakeButton bt_Widthdown(450, 10, 30, 30, W_UP, L"+");

/// 펜 색상 변경 버튼

MakeButton bt_ColorPalette(680, 10, 50, 50, PALETTE, L"PALETTE");

/// 펜 색상 변경 버튼
MakeButton bt_Change_Pen(780, 10, 50, 50, CHANGE_PEN, L"PEN");
MakeButton bt_Heart_Stamp(840, 10, 50, 50, HEART_STAMP, L"HEART");
MakeButton bt_Uh_Stamp(900, 10, 50, 50, UH_STAMP, L"UH");
MakeButton bt_Yuhan_Stamp(960, 10, 50, 50, YUHAN_STAMP, L"YUHAN");
MakeButton bt_Yongbin_Stamp(1020, 10, 50, 50, YONGBIN_STAMP, L"YONGBIN");


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /// 윈도우 크기 고정 정적 변수로 선언
    static WindowSizeManager windowSizeManager(Window_Size_Width, Window_Size_Height);

    switch (message)
    {
    case WM_GETMINMAXINFO:
        // 창 크기 고정을 위해 HandleMinMaxInfo 호출
        windowSizeManager.HandleMinMaxInfo(lParam);
        break;

    case WM_CREATE:
        g_Hwnd = hWnd;

        /// stamp 관련 객체 초기화
        stampInfo = new Stamp(NULL, NULL);


        /// 윈도우 창 생성시 버튼 생성 메서드 실행
        /// 인자 관련 설명은 button.cpp 파일 주석 참고
        /// 리플레이, 지우개 버튼 생성
        bt_Replay.mkButton(g_Hwnd, IDI_REPLAY_ICON);
        bt_Clear.mkButton(g_Hwnd, IDI_ERASE_ICON);

        /// 세이브 로드 버튼 생성
        bt_SAVE.mkButton(g_Hwnd);
        bt_Load.mkButton(g_Hwnd);

        /// 굵기 조절 버튼 생성
        bt_Widthup.mkButton(g_Hwnd);
        bt_Widthdown.mkButton(g_Hwnd);

        /// 색상 변경 버튼 생성

        bt_ColorPalette.mkButton(g_Hwnd);

        /// 스탬프 관련 버튼 생성
        bt_Change_Pen.mkButton(g_Hwnd);
        bt_Heart_Stamp.mkButton(g_Hwnd, IDI_HEART_ICON);
        bt_Uh_Stamp.mkButton(g_Hwnd, IDI_UH_ICON);
        bt_Yuhan_Stamp.mkButton(g_Hwnd, IDI_YUHAN_ICON);
        bt_Yongbin_Stamp.mkButton(g_Hwnd, IDI_YONGBIN_ICON);

        /// 그리기 관련 분기
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        /// 스탬프 버튼이 활성화 되어있을시 스탬프 기능 실행
        if (stampActive) {
            stampInfo = new Stamp(stamp_Size, stampIcon);
            stampInfo->handleStamp(hWnd, message, lParam, &penMemory);

            /// 버튼 비활성화시 그리기 함수 실행
        }
        else {
            drawInstance.drawLine(&pen_Width, g_Hwnd, message, lParam, &pen_Color, &g_Pen_Info, &penMemory);
        }
        break;

        /// 버튼으로 구현한 func 기능은 COMMAND 에서 정의한다.
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        /// 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
            /// 지우기 기능
        case ERASE:
            eraser.erase(hWnd, &penMemory);
            break;

            /// 리플레이 기능 
        case REPLAY:
            drawInstance.replayThread(g_Hwnd, &penMemory);
            break;

            /// 펜 모드 (펜, 스탬프) 
        case CHANGE_PEN:
            stampInfo->changeModeToPen(g_Hwnd, &stampActive);
            break;

        case PALETTE:
            g_colorPalette.colorSelect(g_Hwnd, 0);
            pen_Color = g_colorPalette.getColor(0);
            break;

            /// 스탬프 관련 case 
        case HEART_STAMP:
        case UH_STAMP:
        case YUHAN_STAMP:
        case YONGBIN_STAMP:
            stampInfo->changeModeToStamp(&stampActive, g_Hwnd, &stampIcon, wParam);
            break;

            /// SAVE, LOAD 기능 
        case SAVE:
        case LOAD:
            fileManager.selectFileMode(wmId, g_Hwnd, &penMemory);
            break;

            /// 펜 굵기 관련 기능
        case W_DOWN:
        case W_UP:
            penWidthControl.widthControl(g_Hwnd, wmId, &pen_Width, &stamp_Size, &stampActive);            /// 펜 굵기 조절
            break;

            /// 펜 색상 변경 기능
 
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        /// 펜 굵기 표시 메서드
        penWidthControl.penWidthDisplay(hdc, &stampActive, &stamp_Size, &pen_Width);

        /// 그리기 영역 사각형 그리기
        paintSquare.makeSquare(hdc);

        /// 그리기 한 벡터 데이터 그리기 유지
        drawInstance.drawStay(hdc, g_Hwnd, &penMemory);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
/// 정보 대화 상자의 메시지 처리기입니다.
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

