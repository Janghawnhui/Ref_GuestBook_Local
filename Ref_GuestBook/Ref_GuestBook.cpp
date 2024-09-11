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
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
}




///  함수: MyRegisterClass()
///  용도: 창 클래스를 등록합니다.

ATOM Window::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = StaticWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_REFGUESTBOOK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_REFGUESTBOOK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
/// 다른 파일에서 사용할 시 앞에 extern을 쓰고 선언한다.
/// ex) extern vector<PEN_INFO> penMemory;

vector<PEN_INFO> penMemory;             /// 펜 구조체 정보 저장 벡터 변수 전역변수 정의
PEN_INFO g_Pen_Info;                    /// 펜 정보 구조체 전역변수 정의

COLORREF pen_Color = RGB(0, 0, 0);      /// 펜 기본 색상 BLACK
HWND g_Hwnd;                            /// HWND 전역변수 정의
int pen_Width = 10;                     /// 펜 기본 굵기 10으로 정의

File_Manager fileManager;               /// File_Manager 클래스의 인스턴스 생성
Eraser eraser;                          /// Eraser 클래스의 인스턴스 생성


int stamp_Size = 80;                    // 스탬프 크기 가로, 세로 80으로 정의
int stampIcon = 132;                    // 스탬프 아이콘 초기값
bool stampActive = false;               // 스탬프 버틀 활성화 확인
static Stamp* stampInfo = nullptr;      // Stamp 객체를 저장할 포인터

/// <summary>
/// 버튼 구현 인스턴스 선언은 전역변수로 선언한다.
/// 생성자의 인자로는 x좌표, y좌표, 너비, 높이, func(해당 기능), 버튼 텍스트(이미지 삽입으로 변경 예정)
/// 버튼 생성 메서드는 WM_CREATE 레이블에서 선언하며
/// 해당 기능 작동은 WM_COMMAND에서 정의한다.
/// 또한 기능 상수는 Ref_GuestBook 헤더파일에 정의한다.
/// </summary>
MakeButton bt_Clear(120, 10, 100, 100, ERASE, L"ERASE");
MakeButton bt_Replay(10, 10, 100, 100, REPLAY, L"REPLAY");
MakeButton bt_SAVE(230, 10, 100, 45, SAVE, L"SAVE");
MakeButton bt_Load(230, 65, 100, 45, LOAD, L"LOAD");
MakeButton bt_Widthup(375, 10, 30, 30, W_DOWN, L"-");
MakeButton bt_Widthdown(450, 10, 30, 30, W_UP, L"+");
MakeButton bt_ColorChange(660, 10, 30, 30, COLOR_CHANGE, L" 색");

/*
///펜 색상 변경 버튼 
*/
MakeButton bt_ColorRed(500, 10, 30, 30, C_RED, L"빨");
MakeButton bt_ColorOrange(540, 10, 30, 30, C_ORANGE, L"주");
MakeButton bt_ColorYellow(580, 10, 30, 30, C_YELLOW, L"노");
MakeButton bt_ColorGreen(620, 10, 30, 30, C_GREEN, L"초");
MakeButton bt_ColorBlue(500, 50, 30, 30, C_BLUE, L"파");
MakeButton bt_ColorNavy(540, 50, 30, 30, C_NAVY, L"남");
MakeButton bt_ColorPurple(580, 50, 30, 30, C_PURPLE, L"보");
MakeButton bt_ColorBlack(620, 50, 30, 30, C_BLACK, L"검");


MakeButton bt_Change_Pen(780, 10, 50, 50, CHANGE_PEN, L"PEN");
MakeButton bt_Heart_Stamp(840, 10, 50, 50, HEART_STAMP, L"HEART");
MakeButton bt_Uh_Stamp(900, 10, 50, 50, UH_STAMP, L"UH");
MakeButton bt_Yuhan_Stamp(960, 10, 50, 50, YUHAN_STAMP, L"YUHAN");
MakeButton bt_Yongbin_Stamp(1020, 10, 50, 50, YONGBIN_STAMP, L"YONGBIN");

LRESULT Window::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return sinTonIns->WndProc(hWnd, message, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{    /**
    * static을 사용해서 불필요한 메모리 낭비 줄이기
    */
    static WindowSizeManager windowSizeManager(Window_Size_Width, Window_Size_Height);

    switch (message)
    { 
    case WM_GETMINMAXINFO:
        // 창 크기 고정을 위해 HandleMinMaxInfo 호출
        windowSizeManager.HandleMinMaxInfo(lParam);
        break;

    case WM_CREATE:
        g_Hwnd = hWnd;
        if (stampInfo) {
            delete stampInfo;
        }
        stampInfo = new Stamp(stamp_Size, stampIcon, penMemory);

        /// 윈도우 창 생성시 버튼 생성 메서드 실행
        /// 인자 관련 설명은 button.cpp 파일 주석 참고
        bt_Replay.mkButton(IDI_REPLAY_ICON);
        bt_Clear.mkButton(IDI_ERASE_ICON);                        // 지우기 버튼

        bt_SAVE.mkButton();
        bt_Load.mkButton();

        bt_Widthup.mkButton();
        bt_Widthdown.mkButton();


        bt_ColorRed.mkButton(IDI_RED_ICON);
        bt_ColorOrange.mkButton(IDI_ORANGE_ICON);
        bt_ColorYellow.mkButton(IDI_YELLOW_ICON);
        bt_ColorGreen.mkButton(IDI_GREEN_ICON);
        bt_ColorBlue.mkButton(IDI_BLUE_ICON);
        bt_ColorNavy.mkButton(IDI_NAVY_ICON);
        bt_ColorPurple.mkButton(IDI_PURPLE_ICON);
        bt_ColorBlack.mkButton(IDI_BLACK_ICON);
        bt_ColorChange.mkButton();
        

        bt_Change_Pen.mkButton();
        bt_Heart_Stamp.mkButton(IDI_HEART_ICON);
        bt_Uh_Stamp.mkButton(IDI_UH_ICON);
        bt_Yuhan_Stamp.mkButton(IDI_YUHAN_ICON);
        bt_Yongbin_Stamp.mkButton(IDI_YONGBIN_ICON);


        /// 버튼으로 구현한 func 상수 기능은 여기서 정의한다.
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            /// 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            /// 지우기 기능 
            case COLOR_CHANGE:
                if (penNum == 0)
                    colorPalette->colorSelect(hWnd, penNum);
                else penNum = 0;
                break;
            case ERASE:
                eraser.erase(hWnd);                         /// 지우기                  
                break;

            /// 리플레이 기능
            case REPLAY:
                // 리플레이 기능은 스레드화
                CreateThread(NULL, 0, replay, (LPVOID)lParam, 0, NULL);
                break;
            
            case CHANGE_PEN:
                if (!stampActive) {
                    stampActive = true;
                    MakeButton bt_Change_Pen(780, 10, 50, 50, CHANGE_PEN, L"STAMP");
                }
                else {
                    stampActive = false;
                    MakeButton bt_Change_Pen(780, 10, 50, 50, CHANGE_PEN, L"PEN");
                }
                bt_Change_Pen.mkButton();
                break;

            // 스탬프 기능 추가
            case HEART_STAMP:
            case UH_STAMP:
            case YUHAN_STAMP:
            case YONGBIN_STAMP:
                switch (wParam) {
                case HEART_STAMP:
                    stampIcon = IDI_HEART_ICON;
                    break;
                case UH_STAMP:
                    stampIcon = IDI_UH_ICON;
                    break;
                case YUHAN_STAMP:
                    stampIcon = IDI_YUHAN_ICON;
                    break;
                case YONGBIN_STAMP:
                    stampIcon = IDI_YONGBIN_ICON;
                    break;
                }
                if (!stampActive) { stampActive = true; }
                break;

            // SAVE, LOAD 기능

            case SAVE:
                fileManager.SaveFile(hWnd);         /// 저장하기
                break;
            case LOAD:
                fileManager.LoadFile(hWnd);         /// 불러오기
                break;

            /// 펜 굵기 관련 기능
            case W_DOWN:
                w_Control(g_Hwnd, W_DOWN);
                break;
            case W_UP:
                w_Control(g_Hwnd, W_UP);
                break;

            // 펜 색상 변경 기능
            case C_RED:
                Change_Color(C_RED);
                break;
            case C_ORANGE:
                Change_Color(C_ORANGE);
                break;
            case C_YELLOW:
                Change_Color(C_YELLOW);
                break;
            case C_GREEN:
                Change_Color(C_GREEN);
                break;
            case C_BLUE:
                Change_Color(C_BLUE);
                break;
            case C_NAVY:
                Change_Color(C_NAVY);
                break;
            case C_PURPLE:
                Change_Color(C_PURPLE);
                break;
            case C_BLACK:
                Change_Color(C_BLACK); 
                break;
            
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
        WCHAR szPenWidth[10] = {};

        wsprintf(szPenWidth, L"%d", stampActive ? (stamp_Size / 10) : pen_Width);
        TextOut(hdc, 420, 15, szPenWidth, lstrlen(szPenWidth)); // 위치 (420, 15)에 출력

        Square(hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        if (stampInfo) {
            delete stampInfo;
            stampInfo = nullptr;
        }
        PostQuitMessage(0);
        break;

    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:

    case WM_LBUTTONUP:
        // 그리기 함수
        if (stampActive) {
            stampInfo = new Stamp(stamp_Size, stampIcon, penMemory);
            stampInfo->handleStamp(hWnd, message, lParam);
        }
        else {
            drawLine(hWnd, message, lParam);
        }
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

Window* Window::GetInstance()
{
    call_once(flag, []          ///익명함수
        {
            sinTonIns.reset(new Window);
        });
    return sinTonIns.get();
}

